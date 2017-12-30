#include "..\CheatInclude.h"

void CDlight::Run()
{
	if ( !Vars.Visuals.PlayerEsp.bEnabled || !Vars.Visuals.PlayerEsp.bDlight )
		return;

	auto pLocalPlayer = reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( g_pEngine->GetLocalPlayer() ) );
	if ( !pLocalPlayer )
		return;

	for ( int i = 0; i < g_pGlobalVarsBase->maxClients; i++ )
	{
		auto pPlayer = reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( i ) );

		if ( !pPlayer )
			continue;

		if ( pPlayer->GetClientClass()->m_ClassID != 35 )
			continue;

		if ( pPlayer->IsAlive() && !pPlayer->IsDormant() )
		{
			bool team = !pPlayer->IsEnemy();

			dlight_t* pDlight = g_pVEffects->CL_AllocDlight( i );
			pDlight->origin = pPlayer->GetAbsOrigin() + Vector( 0.0f, 0.0f, 10.0f );
			pDlight->radius = 300.0f;
			pDlight->color.b = ( byte ) team && Vars.Visuals.bTeamCheck ? Vars.Visuals.PlayerEsp.Colors.flTeamBoxes[ 2 ] * 255 : Vars.Visuals.PlayerEsp.Colors.flEnemyBoxes[ 2 ] * 255;
			pDlight->color.g = ( byte ) team && Vars.Visuals.bTeamCheck ? Vars.Visuals.PlayerEsp.Colors.flTeamBoxes[ 1 ] * 255 : Vars.Visuals.PlayerEsp.Colors.flEnemyBoxes[ 1 ] * 255;
			pDlight->color.r = ( byte ) team && Vars.Visuals.bTeamCheck ? Vars.Visuals.PlayerEsp.Colors.flTeamBoxes[ 0 ] * 255 : Vars.Visuals.PlayerEsp.Colors.flEnemyBoxes[ 0 ] * 255;
			pDlight->color.exponent = 3.f;
			pDlight->die = g_pGlobalVarsBase->curtime + 0.05f;
			pDlight->decay = pDlight->radius; // / 5.0f;
			pDlight->key = i;
		}
	}
}
