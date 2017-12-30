#include "..\CheatInclude.h"
#include "..\Math.h"

RenderViewFn oRenderView;

void __fastcall g_hkRenderView( void* thisptr, void*, CViewSetup& setup, CViewSetup& hudViewSetup, unsigned int nClearFlags, int whatToDraw )
{
	if ( G::unload )
		return oRenderView( thisptr, setup, hudViewSetup, nClearFlags, whatToDraw );

	F::HvH->NoVisRecoil( setup );

	if ( g_pEngine->IsInGame() )
	{
		auto pLocalPlayer = reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( g_pEngine->GetLocalPlayer() ) );
		if ( !pLocalPlayer )
			return;

		F::Thirdperson->Run( setup );

		if ( Vars.Visuals.bFovChanger && !pLocalPlayer->IsScoped() )
			setup.fov = Vars.Visuals.flFov;

		if ( Vars.Visuals.bViewmodelFovChanger )
			setup.fovViewmodel = Vars.Visuals.flViewmodelFov;
	}

	F::GrenadePrediction->Simulate();

	oRenderView( thisptr, setup, hudViewSetup, nClearFlags, whatToDraw );
}
