#pragma once

class CBaseClientState
{
public:
	void ForceFullUpdate()
	{
		*reinterpret_cast< int* >( uintptr_t( this ) + 0x174 ) = -1;
	};
	OFFSET( m_nChokedCommands, int, 0x4CB0 )
};