#include "..\CheatInclude.h"
#include "..\Math.h"

#define TICK_INTERVAL			( g_pGlobalVarsBase->interval_per_tick )


#define TIME_TO_TICKS( dt )		( (int)( 0.5f + (float)(dt) / TICK_INTERVAL ) )
#define TICKS_TO_TIME( t )		( TICK_INTERVAL *( t ) )
#define ROUND_TO_TICKS( t )		( TICK_INTERVAL * TIME_TO_TICKS( t ) )

template<class T> const T&
clamp( const T& x, const T& upper, const T& lower ) { return min( upper, max( x, lower ) ); }

#define LC_NONE				0
#define LC_ALIVE			(1<<0)

#define LC_ORIGIN_CHANGED	(1<<8)
#define LC_ANGLES_CHANGED	(1<<9)
#define LC_SIZE_CHANGED		(1<<10)
#define LC_ANIMATION_CHANGED (1<<11)

float GetLerpTime()
{
	auto cl_updaterate = g_pCvar->FindVar( "cl_updaterate" );
	auto sv_minupdaterate = g_pCvar->FindVar( "sv_minupdaterate" );
	auto sv_maxupdaterate = g_pCvar->FindVar( "sv_maxupdaterate" );

	auto cl_interp = g_pCvar->FindVar( "cl_interp" );
	auto sv_client_min_interp_ratio = g_pCvar->FindVar( "sv_client_min_interp_ratio" );
	auto sv_client_max_interp_ratio = g_pCvar->FindVar( "sv_client_max_interp_ratio" );

	auto updateRate = cl_updaterate->GetInt();
	auto interpRatio = cl_interp->GetFloat();
	auto minInterpRatio = sv_client_min_interp_ratio->GetFloat();
	auto maxInterpRatio = sv_client_max_interp_ratio->GetFloat();
	auto minUpdateRate = sv_minupdaterate->GetInt();
	auto maxUpdateRate = sv_maxupdaterate->GetInt();

	auto clampedUpdateRate = clamp( updateRate, minUpdateRate, maxUpdateRate );
	auto clampedInterpRatio = clamp( interpRatio, minInterpRatio, maxInterpRatio );

	auto lerp = clampedInterpRatio / clampedUpdateRate;

	if ( lerp <= cl_interp->GetFloat() )
		lerp = cl_interp->GetFloat();

	return lerp;
}


bool CBacktrack::StartLagCompensation( C_BasePlayer * player, CUserCmd * cmd, int bestrecord )
{
	if ( !IsValid( player ) )
		return false;

	int idx = player->GetIClientEntity()->EntIndex();

	std::deque< LagRecord > *track = &m_PlayerTrack[ idx - 1 ];

	LagRecord &record = track->at( bestrecord );

	INetChannelInfo *nci = g_pEngine->GetNetChannelInfo();
	auto lerpTime = GetLerpTime();

	auto predictedCmdArrivalTick = g_pGlobalVarsBase->tickcount + 1 + TIME_TO_TICKS( nci->GetAvgLatency( FLOW_INCOMING ) + nci->GetAvgLatency( FLOW_OUTGOING ) );
	auto deltaTime = clamp( lerpTime + nci->GetLatency( FLOW_OUTGOING ), 0.f, 1.f ) - TICKS_TO_TIME( predictedCmdArrivalTick + TIME_TO_TICKS( lerpTime ) - TIME_TO_TICKS( record.m_flSimulationTime ) );

	if ( fabs( deltaTime ) > 0.2f )
		return false;

	BacktrackPlayer( player, record );

	return true;
}

int CBacktrack::FixTickcount( C_BasePlayer * player, int bestrecord )
{
	int idx = player->GetIClientEntity()->EntIndex();

	std::deque< LagRecord > *track = &m_PlayerTrack[ idx - 1 ];
	LagRecord &record = track->at( bestrecord );

	if ( record.m_flSimulationTime == 0.0f )
		return TIME_TO_TICKS( player->GetSimulationTime() + GetLerpTime() ) + 1;

	int iLerpTicks = TIME_TO_TICKS( GetLerpTime() );
	int iTargetTickCount = TIME_TO_TICKS( record.m_flSimulationTime ) + iLerpTicks;

	return iTargetTickCount;
}

void CBacktrack::BacktrackPlayer( C_BasePlayer * player, LagRecord record )
{
	if ( !player || record.m_flSimulationTime == 0.0f )
		return;

	static uintptr_t pInvalidateBoneCache = U::FindSig( charenc( "client.dll" ), charenc( "80 3D ? ? ? ? 00 74 16 A1" ) );
	unsigned long g_iModelBoneCounter = **reinterpret_cast< unsigned long** > ( pInvalidateBoneCache + 10 );
	*reinterpret_cast< unsigned int* >( reinterpret_cast< uintptr_t >( player ) + 0x2914 ) = 0xFF7FFFFF;
	*reinterpret_cast< unsigned int* >( reinterpret_cast< uintptr_t >( player ) + 0x2680 ) = ( g_iModelBoneCounter - 1 );

	player->SetAbsOrigin( record.m_vecOrigin );
	player->SetAbsAngles( record.m_vecAbsAngles );
	player->GetSimulationTime() = record.m_flSimulationTime;
	player->GetFlags() = record.m_fFlags;
	player->GetLowerBodyYawTarget() = record.m_flLowerBodyYawTarget;

	for ( int poseparam = 0; poseparam < 24; poseparam++ )
		player->GetPoseParameter()[ poseparam ] = record.m_flPoseParameter[ poseparam ];

	for ( int animlayer = 2; animlayer < 4; animlayer++ )
	{
		player->GetAnimOverlay( animlayer )->m_flCycle = record.m_layerRecords[ animlayer ].m_flCycle;
		player->GetAnimOverlay( animlayer )->m_nOrder = record.m_layerRecords[ animlayer ].m_nOrder;
		player->GetAnimOverlay( animlayer )->m_nSequence = record.m_layerRecords[ animlayer ].m_nSequence;
		player->GetAnimOverlay( animlayer )->m_flWeight = record.m_layerRecords[ animlayer ].m_flWeight;
	}

	player->UpdateAnimations();
}

void CBacktrack::FrameUpdatePostEntityThink( ClientFrameStage_t stage )
{
	if ( stage != FRAME_NET_UPDATE_POSTDATAUPDATE_START )
		return;

	static auto sv_unlag = g_pCvar->FindVar( "sv_unlag" );
	static auto sv_maxunlag = g_pCvar->FindVar( "sv_maxunlag" );

	if ( ( g_pGlobalVarsBase->maxClients <= 1 ) || !sv_unlag->GetBool() )
	{
		ClearHistory();
		return;
	}

	int flDeadtime = g_pGlobalVarsBase->curtime - sv_maxunlag->GetFloat();

	for ( int i = 1; i <= g_pGlobalVarsBase->maxClients; i++ )
	{
		auto *pPlayer = reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( i ) );

		auto &track = m_PlayerTrack[ i - 1 ];

		if ( !IsValid( pPlayer ) )
		{
			if ( track.size() > 0 )
			{
				track.clear();
			}

			continue;
		}

		*reinterpret_cast< int* > ( reinterpret_cast< uintptr_t >( pPlayer ) + 0xA30 ) = g_pGlobalVarsBase->framecount;
		*reinterpret_cast< int* > ( reinterpret_cast< uintptr_t >( pPlayer ) + 0xA28 ) = 0;

		DisableInterpolation( pPlayer );

		Assert( track->Count() < 1000 ); // insanity check

										 // remove tail records that are too old
		int tailIndex = track.size() - 1;
		while ( track.size() )
		{
			LagRecord &tail = *( track.end() - 1 );

			// if tail is within limits, stop
			if ( tail.m_flSimulationTime >= flDeadtime )
				break;

			// remove tail, get new tail
			track.pop_back();
			tailIndex = track.size() - 1;
		}

		// check if head has same simulation time
		if ( track.size() > 0 )
		{
			LagRecord &head = *track.begin();

			// check if player changed simulation time since last time updated
			if ( head.m_flSimulationTime >= pPlayer->GetSimulationTime() )
				continue; // don't add new entry for same or older time
		}

		// add new record to player track
		track.push_front( LagRecord() );
		LagRecord &record = *track.begin();


		static uintptr_t pInvalidateBoneCache = U::FindSig( charenc( "client.dll" ), charenc( "80 3D ? ? ? ? 00 74 16 A1" ) );
		unsigned long g_iModelBoneCounter = **reinterpret_cast< unsigned long** > ( pInvalidateBoneCache + 10 );
		*reinterpret_cast< unsigned int* >( reinterpret_cast< uintptr_t >( pPlayer ) + 0x2914 ) = 0xFF7FFFFF;
		*reinterpret_cast< unsigned int* >( reinterpret_cast< uintptr_t >( pPlayer ) + 0x2680 ) = ( g_iModelBoneCounter - 1 );

		record.m_flSimulationTime = pPlayer->GetSimulationTime();
		record.m_vecAbsAngles = pPlayer->GetAbsAngles().Clamp();
		record.m_vecOrigin = pPlayer->GetOrigin();
		record.m_fFlags = pPlayer->GetFlags();
		record.m_flLowerBodyYawTarget = pPlayer->GetLowerBodyYawTarget();

		for ( int poseparam = 0; poseparam < 24; poseparam++ )
			record.m_flPoseParameter[ poseparam ] = pPlayer->GetPoseParameter()[ poseparam ];

		for ( int animlayer = 2; animlayer < 4; animlayer++ )
		{
			record.m_layerRecords[ animlayer ].m_flCycle = pPlayer->GetAnimOverlay( animlayer )->m_flCycle;
			record.m_layerRecords[ animlayer ].m_nOrder = pPlayer->GetAnimOverlay( animlayer )->m_nOrder;
			record.m_layerRecords[ animlayer ].m_nSequence = pPlayer->GetAnimOverlay( animlayer )->m_nSequence;
			record.m_layerRecords[ animlayer ].m_flWeight = pPlayer->GetAnimOverlay( animlayer )->m_flWeight;
		}
	}
}

bool CBacktrack::ValidTick( float simTime )
{
	INetChannelInfo *nci = g_pEngine->GetNetChannelInfo();
	if ( !nci )
		return false;

	auto lerpTime = GetLerpTime();

	auto predictedCmdArrivalTick = g_pGlobalVarsBase->tickcount + 1 + TIME_TO_TICKS( nci->GetAvgLatency( FLOW_INCOMING ) + nci->GetAvgLatency( FLOW_OUTGOING ) );
	auto deltaTime = clamp( lerpTime + nci->GetLatency( FLOW_OUTGOING ), 0.f, 1.f ) - TICKS_TO_TIME( predictedCmdArrivalTick + TIME_TO_TICKS( lerpTime ) - TIME_TO_TICKS( simTime ) );

	return fabs( deltaTime ) > 0.2f;
}

int CBacktrack::GetBestTick( C_BasePlayer * pPlayer )
{
	int tickcount = g_pGlobalVarsBase->tickcount;

	std::deque< LagRecord > &track = m_PlayerTrack[ pPlayer->GetIClientEntity()->EntIndex() - 1 ];
	for ( int i = track.size(); i > 0; i-- )
	{
		if ( StartLagCompensation( pPlayer, NULL, i - 1 ) )
		{
			tickcount = FixTickcount( pPlayer, i - 1 );
			break;
		}
	}

	return tickcount;
}

void CBacktrack::BackupPlayer( C_BasePlayer * pPlayer )
{
	int idx = pPlayer->GetIClientEntity()->EntIndex();

	auto &record = m_PrevRecords[ idx - 1 ];

	static uintptr_t pInvalidateBoneCache = U::FindSig( charenc( "client.dll" ), charenc( "80 3D ? ? ? ? 00 74 16 A1" ) );
	unsigned long g_iModelBoneCounter = **reinterpret_cast< unsigned long** > ( pInvalidateBoneCache + 10 );
	*reinterpret_cast< unsigned int* >( reinterpret_cast< uintptr_t >( pPlayer ) + 0x2914 ) = 0xFF7FFFFF;
	*reinterpret_cast< unsigned int* >( reinterpret_cast< uintptr_t >( pPlayer ) + 0x2680 ) = ( g_iModelBoneCounter - 1 );

	record.m_flSimulationTime = pPlayer->GetSimulationTime();
	record.m_vecAbsAngles = pPlayer->GetAbsAngles().Clamp();
	record.m_vecOrigin = pPlayer->GetAbsOrigin();
	record.m_fFlags = pPlayer->GetFlags();
	record.m_flLowerBodyYawTarget = pPlayer->GetLowerBodyYawTarget();

	for ( int poseparam = 0; poseparam < 24; poseparam++ )
		record.m_flPoseParameter[ poseparam ] = pPlayer->GetPoseParameter()[ poseparam ];

	for ( int animlayer = 0; animlayer < MAX_LAYER_RECORDS; animlayer++ )
	{
		record.m_layerRecords[ animlayer ].m_flCycle = pPlayer->GetAnimOverlay( animlayer )->m_flCycle;
		record.m_layerRecords[ animlayer ].m_nOrder = pPlayer->GetAnimOverlay( animlayer )->m_nOrder;
		record.m_layerRecords[ animlayer ].m_nSequence = pPlayer->GetAnimOverlay( animlayer )->m_nSequence;
		record.m_layerRecords[ animlayer ].m_flWeight = pPlayer->GetAnimOverlay( animlayer )->m_flWeight;
	}
}

void CBacktrack::RestorePlayer( C_BasePlayer * pPlayer )
{
	int idx = pPlayer->GetIClientEntity()->EntIndex();

	auto &record = m_PrevRecords[ idx - 1 ];

	static uintptr_t pInvalidateBoneCache = U::FindSig( charenc( "client.dll" ), charenc( "80 3D ? ? ? ? 00 74 16 A1" ) );
	unsigned long g_iModelBoneCounter = **reinterpret_cast< unsigned long** > ( pInvalidateBoneCache + 10 );
	*reinterpret_cast< unsigned int* >( reinterpret_cast< uintptr_t >( pPlayer ) + 0x2914 ) = 0xFF7FFFFF;
	*reinterpret_cast< unsigned int* >( reinterpret_cast< uintptr_t >( pPlayer ) + 0x2680 ) = ( g_iModelBoneCounter - 1 );

	pPlayer->SetAbsOrigin( record.m_vecOrigin );
	pPlayer->SetAbsAngles( record.m_vecAbsAngles );
	pPlayer->GetSimulationTime() = record.m_flSimulationTime;
	pPlayer->GetFlags() = record.m_fFlags;
	pPlayer->GetLowerBodyYawTarget() = record.m_flLowerBodyYawTarget;

	for ( int poseparam = 0; poseparam < 24; poseparam++ )
		pPlayer->GetPoseParameter()[ poseparam ] = record.m_flPoseParameter[ poseparam ];

	for ( int animlayer = 0; animlayer < MAX_LAYER_RECORDS; animlayer++ )
	{
		pPlayer->GetAnimOverlay( animlayer )->m_flCycle = record.m_layerRecords[ animlayer ].m_flCycle;
		pPlayer->GetAnimOverlay( animlayer )->m_nOrder = record.m_layerRecords[ animlayer ].m_nOrder;
		pPlayer->GetAnimOverlay( animlayer )->m_nSequence = record.m_layerRecords[ animlayer ].m_nSequence;
		pPlayer->GetAnimOverlay( animlayer )->m_flWeight = record.m_layerRecords[ animlayer ].m_flWeight;
	}

	pPlayer->UpdateAnimations();
}

bool CBacktrack::IsValid( C_BasePlayer * pPlayer )
{
	if ( !pPlayer )
		return false;

	if ( pPlayer->IsDormant() || !pPlayer->IsAlive() || pPlayer->GetFlags() & FL_FROZEN )
		return false;

	if ( !pPlayer->IsEnemy() && Vars.Visuals.bTeamCheck )
		return false;

	if ( pPlayer->GetClientClass()->m_ClassID != 35 )
		return false;

	if ( pPlayer->GetIClientEntity()->EntIndex() == g_pEngine->GetLocalPlayer() )
		return false;

	if ( pPlayer->GetImmunity() )
		return false;

	return true;
}
