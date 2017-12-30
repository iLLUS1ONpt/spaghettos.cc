#include "..\CheatInclude.h"

void CMaterialConfig::Run( MaterialSystem_Config_t * config )
{
	if ( !Vars.Visuals.MaterialConfig.bEnabled )
		return;

	config->m_bDrawGray = Vars.Visuals.MaterialConfig.bDrawGrey;
	config->m_nFullbright = Vars.Visuals.MaterialConfig.bFullbright;
	config->m_bShowLowResImage = Vars.Visuals.MaterialConfig.bShowLowResImage;
	config->m_nShowMipLevels = Vars.Visuals.MaterialConfig.bShowMipLevels;
}