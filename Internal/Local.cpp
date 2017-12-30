#pragma once

#include "..\MainInclude.h"

C_BasePlayer* L::GetLocalPlayer()
{
	return reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( g_pEngine->GetLocalPlayer() ) );
}
