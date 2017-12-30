#pragma once

#define MAX_LAYER_RECORDS 15
#define MAX_PLAYERS 64

struct LayerRecord
{
	int m_nSequence;
	float m_flCycle;
	float m_flWeight;
	int m_nOrder;

	LayerRecord()
	{
		m_nSequence = 0;
		m_flCycle = 0;
		m_flWeight = 0;
		m_nOrder = 0;
	}

	LayerRecord( const LayerRecord& src )
	{
		m_nSequence = src.m_nSequence;
		m_flCycle = src.m_flCycle;
		m_flWeight = src.m_flWeight;
		m_nOrder = src.m_nOrder;
	}
};

struct LagRecord
{
	LagRecord()
	{
		m_fFlags = 0;
		m_vecOrigin.Init();
		m_vecAbsAngles.Init();
		m_flSimulationTime = -1;
		m_bIsBreakingLagComp = false;
		m_flLowerBodyYawTarget = 0.f;
		m_masterCycle = 0.f;
		m_masterSequence = 0;

	}

	LagRecord( const LagRecord& src )
	{
		m_fFlags = src.m_fFlags;
		m_vecOrigin = src.m_vecOrigin;
		m_vecAbsAngles = src.m_vecAbsAngles;
		m_flSimulationTime = src.m_flSimulationTime;
		m_bIsBreakingLagComp = src.m_bIsBreakingLagComp;
		m_flLowerBodyYawTarget = src.m_flLowerBodyYawTarget;
		m_masterCycle = src.m_masterCycle;
		m_masterSequence = src.m_masterSequence;
		for ( int poseparams = 0; poseparams < 24; poseparams++ )
		{
			m_flPoseParameter[ poseparams ] = src.m_flPoseParameter[ poseparams ];
		}
	}

	// Did player die this frame
	int						m_fFlags;
	Vector					m_vecOrigin;
	QAngle					m_vecAbsAngles;
	float					m_flLowerBodyYawTarget;

	float					m_flSimulationTime;
	bool					m_bIsBreakingLagComp;

	float					m_flPoseParameter[ 24 ];

	float					m_masterCycle;
	int						m_masterSequence;

	LayerRecord				m_layerRecords[ MAX_LAYER_RECORDS ];
};


class VarMapEntry_t
{
public:
	unsigned short type;
	unsigned short m_bNeedsToInterpolate; // Set to false when this var doesn't
										  // need Interpolate() called on it anymore.
	void *data;
	void *watcher;
};

struct VarMapping_t
{
	VarMapping_t()
	{
		m_nInterpolatedEntries = 0;
	}

	VarMapEntry_t* m_Entries;
	int m_nInterpolatedEntries;
	float m_lastInterpolationTime;
};


class CBacktrack
{
public:
	bool StartLagCompensation( C_BasePlayer * player, CUserCmd * cmd, int bestrecord );
	int FixTickcount( C_BasePlayer * player, int bestrecord );
	void FrameUpdatePostEntityThink( ClientFrameStage_t stage );

	bool ValidTick( float simTime );

	void BacktrackPlayer( C_BasePlayer *player, LagRecord record );
	bool IsValid( C_BasePlayer * pPlayer );
	int GetBestTick( C_BasePlayer* pPlayer );

	void BackupPlayer( C_BasePlayer* pPlayer );
	void RestorePlayer( C_BasePlayer* pPlayer );

	std::deque< LagRecord >	m_PlayerTrack[ MAX_PLAYERS ];
	LagRecord m_PrevRecords[ MAX_PLAYERS ];

	void ClearHistory()
	{
		for ( int i = 0; i < MAX_PLAYERS; i++ )
			m_PlayerTrack[ i ].clear();
	}


	VarMapping_t* GetVarMap( void* pBaseEntity )
	{
		return reinterpret_cast<VarMapping_t*>( ( DWORD ) pBaseEntity + 0x24 ); //0x4C );
	}
public:
	void DisableInterpolation( C_BaseEntity* pEntity )
	{
		VarMapping_t* map = GetVarMap( pEntity );
		if ( !map ) return;
		for ( int i = 0; i < map->m_nInterpolatedEntries; i++ )
		{
			VarMapEntry_t *e = &map->m_Entries[ i ];
			e->m_bNeedsToInterpolate = false;
		}
	}

};