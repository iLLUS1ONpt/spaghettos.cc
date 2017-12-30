#include "..\CheatInclude.h"

OverrideConfigFn oOverrideConfig;

bool __fastcall g_hkOverrideConfig( IMaterialSystem* this0, int edx, MaterialSystem_Config_t* config, bool forceUpdate )
{
	if ( G::unload )
		return oOverrideConfig( this0, config, forceUpdate );

	F::MaterialConfig->Run( config );

	return oOverrideConfig( this0, config, forceUpdate );
}