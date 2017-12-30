#include "..\CheatInclude.h"

void CHvH::Run( CUserCmd * cmd )
{
	if ( !g_pEngine->IsInGame() )
		return;

	m_pLocalPlayer = reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( g_pEngine->GetLocalPlayer() ) );
	if ( !m_pLocalPlayer )
		return;

	m_pCmd = cmd;

	NoRecoil();
}

void CHvH::NoRecoil()
{
	if ( !Vars.HvH.Removals.bRecoil )
		return;

	auto weapon = m_pLocalPlayer->GetActiveWeapon();
	if ( weapon )
		weapon->UpdateAccuracyPenalty();
	m_pCmd->viewangles -= m_pLocalPlayer->GetAimPunchAngle() * 2;
}

void CHvH::NoVisRecoil( CViewSetup& vsView )
{
	if ( !m_pLocalPlayer || !Vars.HvH.Removals.bRecoil || g_pInput->m_fCameraInThirdPerson )
		return;

	vsView.angles -= m_pLocalPlayer->GetAimPunchAngle() * 0.9f + m_pLocalPlayer->GetViewPunchAngle();
}