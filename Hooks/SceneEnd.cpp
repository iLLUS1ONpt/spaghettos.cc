#include "..\CheatInclude.h"

SceneEndFn oSceneEnd;

void __fastcall g_hkSceneEnd( void * ecx, void * edx )
{
	if ( G::unload )
		return oSceneEnd( ecx );

	oSceneEnd( ecx );

	F::Chams->Run();
}