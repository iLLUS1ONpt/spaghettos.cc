#include "..\MainInclude.h"

int created = 0;

IMaterial* IMaterialSystem::FindMaterial( char const* pMaterialName, const char *pTextureGroupName, bool complain, const char *pComplainPrefix )
{
	typedef IMaterial*( __thiscall* OriginalFn )( void*, char const* pMaterialName, const char *pTextureGroupName, bool complain, const char *pComplainPrefix );
	return U::GetVFunc<OriginalFn>( this, 84 )( this, pMaterialName, pTextureGroupName, complain, pComplainPrefix );
}

MaterialHandle_t IMaterialSystem::FirstMaterial()
{
	typedef MaterialHandle_t( __thiscall* FirstMaterialFn )( void* );
	return U::GetVFunc<FirstMaterialFn>( this, 86 )( this );
}

MaterialHandle_t IMaterialSystem::NextMaterial( MaterialHandle_t h )
{
	typedef MaterialHandle_t( __thiscall* NextMaterialFn )( void*, MaterialHandle_t );
	return U::GetVFunc<NextMaterialFn>( this, 87 )( this, h );
}

MaterialHandle_t IMaterialSystem::InvalidMaterial()
{
	typedef MaterialHandle_t( __thiscall* InvalidMaterialFn )( void* );
	return U::GetVFunc<InvalidMaterialFn>( this, 88 )( this );
}

IMaterial* IMaterialSystem::GetMaterial( MaterialHandle_t h )
{
	typedef IMaterial*( __thiscall* GetMaterialFn )( void*, MaterialHandle_t );
	return U::GetVFunc<GetMaterialFn>( this, 89 )( this, h );
}

bool IMaterialSystem::OverrideConfig( MaterialSystem_Config_t const& cfg, bool b )
{
	typedef bool( __thiscall* OverrideConfigFn )( void*, MaterialSystem_Config_t const&, bool );
	return U::GetVFunc<OverrideConfigFn>( this, 21 )( this, cfg, b );
}

IMatRenderContext* IMaterialSystem::GetRenderContext()
{
	typedef IMatRenderContext*( __thiscall* GetRenderContextFn )( void* );
	return U::GetVFunc<GetRenderContextFn>( this, 115 )( this );
}