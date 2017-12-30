#pragma once

class VMTHook
{
public:
	VMTHook();

	VMTHook( PDWORD* ppdwClassBase );

	~VMTHook();

	bool bInitialize( PDWORD* ppdwClassBase );
	bool bInitialize( PDWORD** pppdwClassBase );

	void UnHook();

	void ReHook();

	int iGetFuncCount();

	template<typename Fn>
	Fn GetFuncAddress( int Index )
	{
		if ( Index >= 0 && Index <= ( int ) m_VTSize && m_OldVT != NULL )
		{
			return reinterpret_cast<Fn>( m_OldVT[ Index ] );
		}
		return NULL;
	}

	PDWORD GetOldVT();

	DWORD HookFunction( DWORD dwNewFunc, unsigned int iIndex );

private:
	DWORD GetVTCount( PDWORD pdwVMT );

	PDWORD*	m_ClassBase;
	PDWORD	m_NewVT, m_OldVT;
	DWORD	m_VTSize;
};

