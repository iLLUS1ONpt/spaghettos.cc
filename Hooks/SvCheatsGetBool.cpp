#include "..\CheatInclude.h"
#include <intrin.h>

SvCheatsGetBoolFn oSvCheatsGetBool;

bool __fastcall g_hkSvCheatsGetBool( void* pConVar, void* edx )
{
	if ( G::unload )
		return oSvCheatsGetBool( pConVar );

	if ( ( uintptr_t ) _ReturnAddress() == Offsets->dwCAM_Think )
		return true;

	return oSvCheatsGetBool( pConVar );
}