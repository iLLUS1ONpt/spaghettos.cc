#include "..\CheatInclude.h"

void CPrediction::StartPrediction( int &tickbase, CUserCmd* cmd )
{
	static bool bInit = false;
	if ( !bInit )
	{
		m_pPredictionRandomSeed = *reinterpret_cast< int** > ( U::FindSig( "client.dll", "8B 0D ? ? ? ? BA ? ? ? ? E8 ? ? ? ? 83 C4 04" ) + 2 );
		bInit = true;
	}

	pLocalPlayer = reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( g_pEngine->GetLocalPlayer() ) );

	*m_pPredictionRandomSeed = MD5_PseudoRandom( cmd->command_number ) & 0x7FFFFFFF;

	m_flOldCurtime = g_pGlobalVarsBase->curtime;
	m_flOldFrametime = g_pGlobalVarsBase->frametime;

	g_pGlobalVarsBase->curtime = tickbase * g_pGlobalVarsBase->interval_per_tick;
	g_pGlobalVarsBase->frametime = g_pGlobalVarsBase->interval_per_tick;

	g_pGameMovement->StartTrackPredictionErrors( pLocalPlayer );

	memset( &m_MoveData, 0, sizeof( m_MoveData ) );
	g_pMoveHelper->SetHost( pLocalPlayer );
	g_pPrediction->SetupMove( pLocalPlayer, cmd, g_pMoveHelper, &m_MoveData );
	g_pGameMovement->ProcessMovement( pLocalPlayer, &m_MoveData );
	g_pPrediction->FinishMove( pLocalPlayer, cmd, &m_MoveData );
}

void CPrediction::EndPrediction()
{
	g_pGameMovement->FinishTrackPredictionErrors( pLocalPlayer );
	g_pMoveHelper->SetHost( 0 );

	*m_pPredictionRandomSeed = -1;

	g_pGlobalVarsBase->curtime = m_flOldCurtime;
	g_pGlobalVarsBase->frametime = m_flOldFrametime;
}