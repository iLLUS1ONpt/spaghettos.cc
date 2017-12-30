#include "..\CheatInclude.h"

BeginFrameFn oBeginFrame;

void __fastcall g_hkBeginFrame( void* thisptr )
{
	if ( G::unload )
		return oBeginFrame( thisptr );
	
	F::Visuals->BulletTracers();

	oBeginFrame( thisptr );
}