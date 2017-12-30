#pragma once

class IMemAlloc
{
public:
	void* Alloc( size_t nSize );
	void *Realloc( void *pMem, size_t nSize );
	void Free( void *pMem );

};
