#include "..\CheatInclude.h"
#include "..\Math.h"

void CThirdperson::Run( ClientFrameStage_t stage )
{
	if ( stage != FRAME_RENDER_START )
		return;

	if ( !g_pEngine->IsInGame() )
		return;

	auto pLocalPlayer = reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( g_pEngine->GetLocalPlayer() ) );
	if ( !pLocalPlayer )
		return;

	static QAngle viewAngles;
	g_pEngine->GetViewAngles( viewAngles );

	if (GetAsyncKeyState(0x54) & 1)
		Vars.Other.bThirdperson = !Vars.Other.bThirdperson;

	if (Vars.Other.bThirdperson && pLocalPlayer->IsAlive())
	{
		g_pInput->m_fCameraInThirdPerson = true;
		g_pInput->m_vecCameraOffset = Vector( viewAngles.x, viewAngles.y, Vars.Other.flThirdpersonDist );
		if ( g_pInput->m_fCameraInThirdPerson && pLocalPlayer->IsAlive() )
			*reinterpret_cast< QAngle* > ( reinterpret_cast< uintptr_t >( pLocalPlayer ) + 0x31C8 ) = m_angle;
	}
	else
	{
		if ( g_pInput->m_fCameraInThirdPerson )
		{
			g_pInput->m_fCameraInThirdPerson = false;
			g_pInput->m_vecCameraOffset = Vector( viewAngles.x, viewAngles.y, 20.f );
		}
	}
}

void CThirdperson::Run( CViewSetup& setup )
{
	auto pLocalPlayer = reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( g_pEngine->GetLocalPlayer() ) );
	if ( !pLocalPlayer )
		return;

	static bool bOnce = false;
	static bool bOnce2 = false;

	if ( Vars.Other.bThirdperson )
	{
		bOnce = true;

		if ( pLocalPlayer->IsAlive() )
		{
			bOnce2 = false;
			return;
		}

		auto specHandle = pLocalPlayer->GetObserverTarget();
		if ( !specHandle )
			return;

		auto pSpec = reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntityFromHandle( specHandle ) );
		if ( !pSpec )
			return;

		static bool bOnce2 = false;

		if ( bOnce2 )
			pLocalPlayer->GetObserverMode() = 5;

		if ( pLocalPlayer->GetObserverMode() == 4 )
			bOnce2 = true;

		static QAngle angles;
		g_pEngine->GetViewAngles( angles );
		setup.angles = angles;

		trace_t tr;
		Ray_t ray;

		Vector camForward, camRight, camUp;
		Math->AngleVectors( angles, &camForward, &camRight, &camUp );

		Vector vecCamOffset = pSpec->GetEyePosition() + ( camForward * -Vars.Other.flThirdpersonDist ) + ( camRight ) +( camUp * 20.f );

		//cast a ray from the Current camera Origin to the Desired 3rd person Camera origin
		ray.Init( pSpec->GetEyePosition(), vecCamOffset );
		CTraceFilter traceFilter;
		traceFilter.pSkip = pSpec;
		g_pEngineTrace->TraceRay( ray, MASK_SHOT, &traceFilter, &tr );

		setup.origin = tr.endpos;
	}
	else if ( bOnce )
	{
		pLocalPlayer->GetObserverMode() = pLocalPlayer->IsAlive() ? 0 : 4;
		bOnce = false;
	}
}