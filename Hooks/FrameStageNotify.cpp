#include "..\CheatInclude.h"

FrameStageNotifyFn oFrameStageNotify;

void __stdcall g_hkFrameStageNotify( ClientFrameStage_t stage )
{
	if ( G::unload )
		return oFrameStageNotify( stage );

	F::SkinChanger->Run( stage );

	F::Thirdperson->Run( stage );

	F::Resolver->Run( stage );

	F::Backtrack->FrameUpdatePostEntityThink( stage );

	oFrameStageNotify( stage );
}