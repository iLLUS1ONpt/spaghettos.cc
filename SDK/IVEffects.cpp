#include "..\CheatInclude.h"

dlight_t* IVEffects::CL_AllocDlight( int key )
{
	typedef dlight_t*( __thiscall* OriginalFn )( PVOID, int );
	return U::GetVFunc<OriginalFn>( this, 4 )( this, key );
}
dlight_t* IVEffects::CL_AllocElight( int key )
{
	typedef dlight_t*( __thiscall* OriginalFn )( PVOID, int );
	return U::GetVFunc<OriginalFn>( this, 5 )( this, key );
}
dlight_t* IVEffects::GetElightByKey( int key )
{
	typedef dlight_t*( __thiscall* OriginalFn )( PVOID, int );
	return U::GetVFunc<OriginalFn>( this, 8 )( this, key );
}