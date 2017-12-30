#pragma once

#include "..\MainInclude.h"

void COffsets::GrabOffsets()
{
	d3d9Device = **reinterpret_cast< DWORD**> ( U::FindSig( strenc( "shaderapidx9.dll" ), strenc( "A1 ?? ?? ?? ?? 50 8B 08 FF 51 0C" ) ) + 0x1 );
	dwCAM_Think = U::FindSig( strenc( "client.dll" ), strenc( "85 C0 75 30 38 86" ) );
}

COffsets* Offsets = new COffsets;