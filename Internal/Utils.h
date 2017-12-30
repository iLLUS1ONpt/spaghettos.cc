#pragma once

#define RandomInt(min, max) (rand() % (max - min + 1) + min)
#define RandomFlt(min, max) (min + 1) + (((float) rand()) / (float) RAND_MAX) * (max - (min + 1));

#define INRANGE(x,a,b)    (x >= a && x <= b) 
#define getBits( x )    (INRANGE((x&(~0x20)),'A','F') ? ((x&(~0x20)) - 'A' + 0xa) : (INRANGE(x,'0','9') ? x - '0' : 0))
#define getByte( x )    (getBits(x[0]) << 4 | getBits(x[1]))

class C_BasePlayer;
namespace U
{
	extern uintptr_t FindSig( std::string moduleName, std::string pattern );

	extern void Startup();

	extern bool IsVisible( const Vector& start, const Vector& end, C_BasePlayer* pPlayer, C_BasePlayer* pLocalPlayer );

	template <typename T>
	T* GetInterface( std::string strModule, std::string strInterface )
	{
		typedef T* ( *CreateInterfaceFn )( const char* szName, int iReturn );
		CreateInterfaceFn CreateInterface = ( CreateInterfaceFn ) GetProcAddress( GetModuleHandleA( strModule.c_str() ), charenc( "CreateInterface" ) );

		return CreateInterface( strInterface.c_str(), 0 );
	}

	template <typename T>
	T GetVFunc( void *vTable, int iIndex ) {
			
		return ( *( T** ) vTable )[ iIndex ];
	}
}
