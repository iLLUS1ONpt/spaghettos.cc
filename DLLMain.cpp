#pragma once 

#include "MainInclude.h"

uintptr_t __stdcall Memes( HMODULE hModule )
{
	U::Startup();

	while ( !GetAsyncKeyState( VK_END ) )
		Sleep( 10 );

	Vars.Menu.bOpened = false;

	G::unload = true;

	Sleep( 100 );

	Hook->ResetHooks();

	FreeLibraryAndExitThread( hModule, NULL );

	return 0;
}

BOOL APIENTRY DllMain( HMODULE hModule, uintptr_t  dwReasonForCall, LPVOID lpReserved )
{
	switch ( dwReasonForCall )
	{
		case DLL_PROCESS_ATTACH:

			CreateThread( NULL, NULL, ( LPTHREAD_START_ROUTINE ) Memes, hModule, NULL, NULL );

			return true;

		case DLL_PROCESS_DETACH:
			if ( lpReserved == nullptr )
			{

			}
			return true;

		default:
			return true;
	}
	return TRUE;
}