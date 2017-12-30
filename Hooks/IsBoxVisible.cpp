#include "..\CheatInclude.h"
#include <intrin.h>

IsBoxVisibleFn oIsBoxVisible;

int	__stdcall g_hkIsBoxVisible( const Vector& mins, const Vector& maxs )
{
	if ( !memcmp( _ReturnAddress(), "\x85\xC0\x74\x2D\x83\x7D\x10\x00\x75\x1C", 10 ) )
		return 1;

	return oIsBoxVisible( mins, maxs );
}