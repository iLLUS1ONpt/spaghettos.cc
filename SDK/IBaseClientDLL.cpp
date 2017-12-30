#include "..\MainInclude.h"

ClientClass* IBaseClientDll::GetAllClasses()
{
	typedef ClientClass*( __thiscall* OriginalFn )( PVOID );
	return U::GetVFunc<OriginalFn>( this, 8 )( this );
}

void IBaseClientDll::CreateMove( int sequence_number, float frametime, bool active )
{

	typedef void( __thiscall* OriginalFn )( PVOID, int, float, bool );
	return U::GetVFunc<OriginalFn>( this, 21 )( this, sequence_number, frametime, active );
}
