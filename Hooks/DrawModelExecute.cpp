#include "..\CheatInclude.h"

DrawModelExecuteFn oDrawModelExecute;

void __stdcall  g_hkDrawModelExecute( IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld )
{
	if ( G::unload )
		return;

	if ( F::Chams->Run( info ) )
		oDrawModelExecute( g_pVModelRender, context, state, info, pCustomBoneToWorld );

	g_pVModelRender->ForcedMaterialOverride( NULL );
}