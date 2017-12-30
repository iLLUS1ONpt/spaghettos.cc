#pragma once

#include "..\MainInclude.h"

VMTHook::VMTHook()
{
	memset( this, 0, sizeof( VMTHook ) );
}

VMTHook::VMTHook( PDWORD* ppdwClassBase )
{
	bInitialize( ppdwClassBase );
}

VMTHook::~VMTHook()
{
	UnHook();
}

bool VMTHook::bInitialize( PDWORD* ppdwClassBase )
{
	m_ClassBase = ppdwClassBase;
	m_OldVT = *ppdwClassBase;
	m_VTSize = GetVTCount( *ppdwClassBase );
	m_NewVT = new DWORD[ m_VTSize + 1 ];
	memcpy( &m_NewVT[ 1 ], m_OldVT, sizeof( DWORD ) * m_VTSize );
	m_NewVT[ 0 ] = ( uintptr_t ) m_OldVT[ -1 ];
	DWORD old;
	VirtualProtect( ppdwClassBase, sizeof( DWORD ), PAGE_EXECUTE_READWRITE, &old );
	*ppdwClassBase = &m_NewVT[ 1 ];
	VirtualProtect( ppdwClassBase, sizeof( DWORD ), old, &old );
	return true;
}
bool VMTHook::bInitialize( PDWORD** pppdwClassBase )
{
	return bInitialize( *pppdwClassBase );
}

void VMTHook::UnHook()
{
	if ( m_ClassBase )
	{
		DWORD old;
		VirtualProtect( m_ClassBase, sizeof( DWORD ), PAGE_EXECUTE_READWRITE, &old );
		*m_ClassBase = m_OldVT;
		VirtualProtect( m_ClassBase, sizeof( DWORD ), old, &old );
	}
}

void VMTHook::ReHook()
{
	if ( m_ClassBase )
	{
		DWORD old;
		VirtualProtect( m_ClassBase, sizeof( DWORD ), PAGE_EXECUTE_READWRITE, &old );
		*m_ClassBase = &m_NewVT[ 1 ];
		VirtualProtect( m_ClassBase, sizeof( DWORD ), old, &old );
	}
}

int VMTHook::iGetFuncCount()
{
	return ( int ) m_VTSize;
}

PDWORD VMTHook::GetOldVT()
{
	return m_OldVT;
}

DWORD VMTHook::HookFunction( DWORD dwNewFunc, unsigned int iIndex )
{
	if ( m_NewVT && m_OldVT && iIndex <= m_VTSize && iIndex >= 0 )
	{
		m_NewVT[ iIndex + 1 ] = dwNewFunc;
		return m_OldVT[ iIndex ];
	}

	return NULL;
}
DWORD VMTHook::GetVTCount( PDWORD pdwVMT )
{
	DWORD dwIndex = 0;

	for ( dwIndex = 0; pdwVMT[ dwIndex ]; dwIndex++ )
	{
		if ( IS_INTRESOURCE( ( FARPROC ) pdwVMT[ dwIndex ] ) )
		{
			break;
		}
	}
	return dwIndex;
}
