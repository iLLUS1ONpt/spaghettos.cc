#include "..\MainInclude.h"
#include <fstream>
#include "..\json.h"

CConfig* Config = new CConfig();

nlohmann::json preset;

template<typename T>
void assign( T &value, char* sz )
{
	if ( preset[ sz ].empty() )
		return;

	value = preset[ sz ].get<T>();
}

void assign_array( float value[ 3 ], char* sz )
{
	if ( preset[ sz ][ "0" ].empty() || preset[ sz ][ "1" ].empty() || preset[ sz ][ "2" ].empty() )
		return;

	value[ 0 ] = preset[ sz ][ "0" ].get<float>();
	value[ 1 ] = preset[ sz ][ "1" ].get<float>();
	value[ 2 ] = preset[ sz ][ "2" ].get<float>();
}

template<typename T>
void save( T value, char* sz )
{
	preset[ sz ] = value;
}

void save_array( float value[3], char* sz )
{
	preset[ sz ][ "0" ] = value[ 0 ];
	preset[ sz ][ "1" ] = value[ 1 ];
	preset[ sz ][ "2" ] = value[ 2 ];
}

template<typename T>
void reset( T value, char* sz )
{
	if ( preset[ sz ].empty() )
		return;

	preset[ sz ].clear();
}

void reset_array( float value[3], char* sz )
{
	if ( preset[ sz ][ "0" ].empty() || preset[ sz ][ "1" ].empty() || preset[ sz ][ "2" ].empty() )
		return;

	preset[ sz ][ "0" ].clear();
	preset[ sz ][ "1" ].clear();
	preset[ sz ][ "2" ].clear();
}

const bool CConfig::LoadPreset()
{
	std::ifstream input_file = std::ifstream( config_name );

	if ( !input_file.good() )
		return false;

	try
	{
		preset << input_file;
	}
	catch ( ... )
	{
		input_file.close();
		return false;
	}

	assign( Vars.SkinChanger.bEnabled, "Vars.SkinChanger.bEnabled" );

	assign( Vars.Other.bThirdperson, "Vars.Other.bThirdperson" );
	assign( Vars.Other.flThirdpersonDist, "Vars.Other.flThirdpersonDist" );
	assign(Vars.Other.bHop, "Vars.Other.bHop");
	assign(Vars.Other.bAutoStrafe, "Vars.Other.bAutoStrafer");
	assign( Vars.Other.bRadar, "Vars.Other.bRadar" );
	assign( Vars.Other.bHitsound, "Vars.Other.bHitsound" );

	assign( Vars.Visuals.bTeamCheck, "Vars.Visuals.bTeamCheck" );
	assign( Vars.Visuals.bSniperCrosshair, "Vars.Visuals.bSniperCrosshair" );
	assign( Vars.Visuals.bAngleLines, "Vars.Visuals.bAngleLines" );
	assign( Vars.Visuals.bHitmarker, "Vars.Visuals.bHitmarker" );
	assign_array( Vars.Visuals.Colors.flHitmarker, "Vars.Visuals.Colors.flHitmarker" );
	assign_array(Vars.Visuals.Colors.flTracers, "Vars.Visuals.Colors.flTracers");
	assign( Vars.Visuals.bHitmarkerHitbox, "Vars.Visuals.bHitmarkerHitbox" );
	assign( Vars.Visuals.bBulletImpacts, "Vars.Visuals.bBulletImpacts" );
	assign( Vars.Visuals.bFovChanger, "Vars.Visuals.bFovChanger" );
	assign( Vars.Visuals.flFov, "Vars.Visuals.flFov" );
	assign( Vars.Visuals.bViewmodelFovChanger, "Vars.Visuals.bViewmodelFovChanger" );
	assign( Vars.Visuals.flViewmodelFov, "Vars.Visuals.flViewmodelFov" );
	assign( Vars.Visuals.bSpreadCircle, "Vars.Visuals.bSpreadCircle" );
	assign( Vars.Visuals.bGrenadePrediction, "Vars.Visuals.bGrenadePrediction" );

	assign( Vars.Visuals.PlayerEsp.bEnabled, "Vars.Visuals.PlayerEsp.bEnabled" );
	assign( Vars.Visuals.PlayerEsp.bSkeleton, "Vars.Visuals.PlayerEsp.bSkeleton" );
	assign( Vars.Visuals.PlayerEsp.bBox, "Vars.Visuals.PlayerEsp.bBox" );
	assign( Vars.Visuals.PlayerEsp.bFade, "Vars.Visuals.PlayerEsp.bFade" );
	assign( Vars.Visuals.PlayerEsp.flFadeSpeed, "Vars.Visuals.PlayerEsp.flFadeSpeed" );
	assign( Vars.Visuals.PlayerEsp.bFilled, "Vars.Visuals.PlayerEsp.bFilled" );
	assign( Vars.Visuals.PlayerEsp.bDlight, "Vars.Visuals.PlayerEsp.bDlight" );
	assign_array( Vars.Visuals.PlayerEsp.Colors.flEnemyBoxes, "Vars.Visuals.PlayerEsp.Colors.flEnemyBoxes" );
	assign_array( Vars.Visuals.PlayerEsp.Colors.flEnemyBones, "Vars.Visuals.PlayerEsp.Colors.flEnemyBones" );
	assign_array( Vars.Visuals.PlayerEsp.Colors.flEnemyHitBoxes, "Vars.Visuals.PlayerEsp.Colors.flEnemyHitBoxes" );
	assign_array( Vars.Visuals.PlayerEsp.Colors.flTeamBoxes, "Vars.Visuals.PlayerEsp.Colors.flTeamBoxes" );
	assign_array( Vars.Visuals.PlayerEsp.Colors.flTeamBones, "Vars.Visuals.PlayerEsp.Colors.flTeamBones" );
	assign_array( Vars.Visuals.PlayerEsp.Colors.flTeamHitBoxes, "Vars.Visuals.PlayerEsp.Colors.flTeamHitBoxes" );

	assign( Vars.Visuals.Chams.bEnabled, "Vars.Visuals.Chams.bEnabled" );
	assign( Vars.Visuals.Chams.bPlayers, "Vars.Visuals.Chams.bPlayers" );
	
	assign( Vars.Visuals.Chams.bHands, "Vars.Visuals.Chams.bHands" );
	assign( Vars.Visuals.Chams.iHandMaterial, "Vars.Visuals.Chams.iHandMaterial" );
	assign( Vars.Visuals.Chams.bWeapon, "Vars.Visuals.Chams.bWeapon" );
	assign( Vars.Visuals.Chams.iWeaponMaterial, "Vars.Visuals.Chams.iWeaponMaterial" );
	assign( Vars.Visuals.Chams.bXQZ, "Vars.Visuals.Chams.bXQZ" );
	assign( Vars.Visuals.Chams.bDarkMode, "Vars.Visuals.Chams.bDarkMode" );
	assign( Vars.Visuals.bAsusWalls, "Vars.Visuals.Chams.bAsusWalls" );
	assign( Vars.Visuals.bAsusBlend, "Vars.Visuals.bAsusBlend" );
	assign_array( Vars.Visuals.Chams.Colors.flEnemyVisible, "Vars.Visuals.Chams.Colors.flEnemyVisible" );
	assign_array( Vars.Visuals.Chams.Colors.flEnemyHidden, "Vars.Visuals.Chams.Colors.flEnemyHidden" );
	assign_array( Vars.Visuals.Chams.Colors.flTeamVisible, "Vars.Visuals.Chams.Colors.flTeamVisible" );
	assign_array( Vars.Visuals.Chams.Colors.flTeamHidden, "Vars.Visuals.Chams.Colors.flTeamHidden" );

	assign( Vars.Visuals.MaterialConfig.bEnabled, "Vars.Visuals.MaterialConfig.bEnabled" );
	assign( Vars.Visuals.MaterialConfig.bDrawGrey, "Vars.Visuals.MaterialConfig.bDrawGrey" );
	assign( Vars.Visuals.MaterialConfig.bFullbright, "Vars.Visuals.MaterialConfig.bFullbright" );
	assign( Vars.Visuals.MaterialConfig.bShowLowResImage, "Vars.Visuals.MaterialConfig.bShowLowResImage" );
	assign( Vars.Visuals.MaterialConfig.bShowMipLevels, "Vars.Visuals.MaterialConfig.bShowMipLevels" );

	assign( Vars.HvH.bEnabled, "Vars.HvH.bEnabled" );
	assign( Vars.HvH.bLBYBreaker, "Vars.HvH.bLBYBreaker" );
	assign( Vars.HvH.Removals.bRecoil, "Vars.HvH.Removals.bRecoil" );

	assign( Vars.HvH.Aimbot.bEnabled, "Vars.HvH.Aimbot.bEnabled" );
	assign( Vars.HvH.Aimbot.bAutoFire, "Vars.HvH.Aimbot.bAutoFire" );
	assign( Vars.HvH.Aimbot.flMaxSpread, "Vars.HvH.Aimbot.flMaxSpread" );
	assign( Vars.HvH.Aimbot.flMinDamage, "Vars.HvH.Aimbot.flMinDamage" );

	input_file.close();

	return true;
}

const bool CConfig::SavePreset()
{
	std::ofstream output_file = std::ofstream( config_name );

	if ( !output_file.good() )
		return false;

	save( Vars.SkinChanger.bEnabled, "Vars.SkinChanger.bEnabled" );

	save( Vars.Other.bThirdperson, "Vars.Other.bThirdperson" );
	save( Vars.Other.flThirdpersonDist, "Vars.Other.flThirdpersonDist" );
	save( Vars.Other.bHop, "Vars.Other.bHop" );
	save( Vars.Other.bAutoStrafe, "Vars.Other.bAutoStrafer" );
	save( Vars.Other.bRadar, "Vars.Other.bRadar" );
	save( Vars.Other.bHitsound, "Vars.Other.bHitsound" );

	save( Vars.Visuals.bTeamCheck, "Vars.Visuals.bTeamCheck" );
	save( Vars.Visuals.bSniperCrosshair, "Vars.Visuals.bSniperCrosshair" );
	save( Vars.Visuals.bAngleLines, "Vars.Visuals.bAngleLines" );
	save( Vars.Visuals.bHitmarker, "Vars.Visuals.bHitmarker" );
	save_array( Vars.Visuals.Colors.flHitmarker, "Vars.Visuals.Colors.flHitmarker" );
	save_array(Vars.Visuals.Colors.flTracers, "Vars.Visuals.Colors.flBulletTracers");
	save( Vars.Visuals.bHitmarkerHitbox, "Vars.Visuals.bHitmarkerHitbox" );
	save( Vars.Visuals.bBulletImpacts, "Vars.Visuals.bBulletImpacts" );
	save( Vars.Visuals.bFovChanger, "Vars.Visuals.bFovChanger" );
	save( Vars.Visuals.flFov, "Vars.Visuals.flFov" );
	save( Vars.Visuals.bViewmodelFovChanger, "Vars.Visuals.bViewmodelFovChanger" );
	save( Vars.Visuals.flViewmodelFov, "Vars.Visuals.flViewmodelFov" );
	save( Vars.Visuals.bSpreadCircle, "Vars.Visuals.bSpreadCircle" );
	save( Vars.Visuals.bGrenadePrediction, "Vars.Visuals.bGrenadePrediction" );

	save( Vars.Visuals.PlayerEsp.bEnabled, "Vars.Visuals.PlayerEsp.bEnabled" );
	save( Vars.Visuals.PlayerEsp.bSkeleton, "Vars.Visuals.PlayerEsp.bSkeleton" );
	save( Vars.Visuals.PlayerEsp.bBox, "Vars.Visuals.PlayerEsp.bBox" );
	save( Vars.Visuals.PlayerEsp.bFade, "Vars.Visuals.PlayerEsp.bFade" );
	save( Vars.Visuals.PlayerEsp.flFadeSpeed, "Vars.Visuals.PlayerEsp.flFadeSpeed" );
	save( Vars.Visuals.PlayerEsp.bFilled, "Vars.Visuals.PlayerEsp.bFilled" );
	save( Vars.Visuals.PlayerEsp.bDlight, "Vars.Visuals.PlayerEsp.bDlight" );
	save_array( Vars.Visuals.PlayerEsp.Colors.flEnemyBoxes, "Vars.Visuals.PlayerEsp.Colors.flEnemyBoxes" );
	save_array( Vars.Visuals.PlayerEsp.Colors.flEnemyBones, "Vars.Visuals.PlayerEsp.Colors.flEnemyBones" );
	save_array( Vars.Visuals.PlayerEsp.Colors.flEnemyHitBoxes, "Vars.Visuals.PlayerEsp.Colors.flEnemyHitBoxes" );
	save_array( Vars.Visuals.PlayerEsp.Colors.flTeamBoxes, "Vars.Visuals.PlayerEsp.Colors.flTeamBoxes" );
	save_array( Vars.Visuals.PlayerEsp.Colors.flTeamBones, "Vars.Visuals.PlayerEsp.Colors.flTeamBones" );
	save_array( Vars.Visuals.PlayerEsp.Colors.flTeamHitBoxes, "Vars.Visuals.PlayerEsp.Colors.flTeamHitBoxes" );

	save( Vars.Visuals.Chams.bEnabled, "Vars.Visuals.Chams.bEnabled" );
	save( Vars.Visuals.Chams.bPlayers, "Vars.Visuals.Chams.bPlayers" );
	save( Vars.Visuals.bTeamCheck, "Vars.Visuals.bTeamCheck" );
	save( Vars.Visuals.Chams.bHands, "Vars.Visuals.Chams.bHands" );
	save( Vars.Visuals.Chams.iHandMaterial, "Vars.Visuals.Chams.iHandMaterial" );
	save( Vars.Visuals.Chams.bWeapon, "Vars.Visuals.Chams.bWeapon" );
	save( Vars.Visuals.Chams.iWeaponMaterial, "Vars.Visuals.Chams.iWeaponMaterial" );
	save( Vars.Visuals.Chams.bXQZ, "Vars.Visuals.Chams.bXQZ" );
	save( Vars.Visuals.Chams.bDarkMode, "Vars.Visuals.Chams.bDarkMode" );
	save( Vars.Visuals.bAsusWalls, "Vars.Visuals.Chams.bAsusWalls" );
	save( Vars.Visuals.bAsusBlend, "Vars.Visuals.bAsusBlend" );
	save_array( Vars.Visuals.Chams.Colors.flEnemyVisible, "Vars.Visuals.Chams.Colors.flEnemyVisible" );
	save_array( Vars.Visuals.Chams.Colors.flEnemyHidden, "Vars.Visuals.Chams.Colors.flEnemyHidden" );
	save_array( Vars.Visuals.Chams.Colors.flTeamVisible, "Vars.Visuals.Chams.Colors.flTeamVisible" );
	save_array( Vars.Visuals.Chams.Colors.flTeamHidden, "Vars.Visuals.Chams.Colors.flTeamHidden" );

	save( Vars.Visuals.MaterialConfig.bEnabled, "Vars.Visuals.MaterialConfig.bEnabled" );
	save( Vars.Visuals.MaterialConfig.bDrawGrey, "Vars.Visuals.MaterialConfig.bDrawGrey" );
	save( Vars.Visuals.MaterialConfig.bFullbright, "Vars.Visuals.MaterialConfig.bFullbright" );
	save( Vars.Visuals.MaterialConfig.bShowLowResImage, "Vars.Visuals.MaterialConfig.bShowLowResImage" );
	save( Vars.Visuals.MaterialConfig.bShowMipLevels, "Vars.Visuals.MaterialConfig.bShowMipLevels" );

	save( Vars.HvH.bEnabled, "Vars.HvH.bEnabled" );
	save( Vars.HvH.bLBYBreaker, "Vars.HvH.bLBYBreaker" );
	save( Vars.HvH.Removals.bRecoil, "Vars.HvH.Removals.bRecoil" );

	save( Vars.HvH.Aimbot.bEnabled, "Vars.HvH.Aimbot.bEnabled" );
	save( Vars.HvH.Aimbot.bAutoFire, "Vars.HvH.Aimbot.bAutoFire" );
	save( Vars.HvH.Aimbot.flMaxSpread, "Vars.HvH.Aimbot.flMaxSpread" );
	save( Vars.HvH.Aimbot.flMinDamage, "Vars.HvH.Aimbot.flMinDamage" );

	output_file << std::setw( 4 ) << preset << std::endl;
	output_file.close();

	return true;
}

const bool CConfig::Reset()
{
	std::ofstream output_file = std::ofstream( config_name );

	if ( !output_file.good() )
		return false;

	reset( Vars.SkinChanger.bEnabled, "Vars.SkinChanger.bEnabled" );

	reset( Vars.Other.bThirdperson, "Vars.Other.bThirdperson" );
	reset( Vars.Other.flThirdpersonDist, "Vars.Other.flThirdpersonDist" );
	reset(Vars.Other.bHop, "Vars.Other.bHop");
	reset(Vars.Other.bAutoStrafe, "Vars.Other.bAutoStrafer");
	reset( Vars.Other.bRadar, "Vars.Other.bRadar" );
	reset( Vars.Other.bHitsound, "Vars.Other.bHitsound" );

	reset( Vars.Visuals.bTeamCheck, "Vars.Visuals.bTeamCheck" );
	reset( Vars.Visuals.bSniperCrosshair, "Vars.Visuals.bSniperCrosshair" );
	reset( Vars.Visuals.bAngleLines, "Vars.Visuals.bAngleLines" );
	reset( Vars.Visuals.bHitmarker, "Vars.Visuals.bHitmarker" );
	reset_array( Vars.Visuals.Colors.flHitmarker, "Vars.Visuals.Colors.flHitmarker" );
	reset_array(Vars.Visuals.Colors.flTracers, "Vars.Visuals.Colors.flBulletTracers");
	reset( Vars.Visuals.bHitmarkerHitbox, "Vars.Visuals.bHitmarkerHitbox" );
	reset( Vars.Visuals.bBulletImpacts, "Vars.Visuals.bBulletImpacts" );
	reset( Vars.Visuals.bFovChanger, "Vars.Visuals.bFovChanger" );
	reset( Vars.Visuals.flFov, "Vars.Visuals.flFov" );
	reset( Vars.Visuals.bViewmodelFovChanger, "Vars.Visuals.bViewmodelFovChanger" );
	reset( Vars.Visuals.flViewmodelFov, "Vars.Visuals.flViewmodelFov" );
	reset( Vars.Visuals.bSpreadCircle, "Vars.Visuals.bSpreadCircle" );
	reset( Vars.Visuals.bGrenadePrediction, "Vars.Visuals.bGrenadePrediction" );

	reset( Vars.Visuals.PlayerEsp.bEnabled, "Vars.Visuals.PlayerEsp.bEnabled" );
	reset( Vars.Visuals.PlayerEsp.bSkeleton, "Vars.Visuals.PlayerEsp.bSkeleton" );
	reset( Vars.Visuals.PlayerEsp.bBox, "Vars.Visuals.PlayerEsp.bBox" );
	reset( Vars.Visuals.PlayerEsp.bFade, "Vars.Visuals.PlayerEsp.bFade" );
	reset( Vars.Visuals.PlayerEsp.flFadeSpeed, "Vars.Visuals.PlayerEsp.flFadeSpeed" );
	reset( Vars.Visuals.PlayerEsp.bFilled, "Vars.Visuals.PlayerEsp.bFilled" );
	reset( Vars.Visuals.PlayerEsp.bDlight, "Vars.Visuals.PlayerEsp.bDlight" );
	reset_array( Vars.Visuals.PlayerEsp.Colors.flEnemyBoxes, "Vars.Visuals.PlayerEsp.Colors.flEnemyBoxes" );
	reset_array( Vars.Visuals.PlayerEsp.Colors.flEnemyBones, "Vars.Visuals.PlayerEsp.Colors.flEnemyBones" );
	reset_array( Vars.Visuals.PlayerEsp.Colors.flEnemyHitBoxes, "Vars.Visuals.PlayerEsp.Colors.flEnemyHitBoxes" );
	reset_array( Vars.Visuals.PlayerEsp.Colors.flTeamBoxes, "Vars.Visuals.PlayerEsp.Colors.flTeamBoxes" );
	reset_array( Vars.Visuals.PlayerEsp.Colors.flTeamBones, "Vars.Visuals.PlayerEsp.Colors.flTeamBones" );
	reset_array( Vars.Visuals.PlayerEsp.Colors.flTeamHitBoxes, "Vars.Visuals.PlayerEsp.Colors.flTeamHitBoxes" );

	reset( Vars.Visuals.Chams.bEnabled, "Vars.Visuals.Chams.bEnabled" );
	reset( Vars.Visuals.Chams.bPlayers, "Vars.Visuals.Chams.bPlayers" );
	reset( Vars.Visuals.Chams.bHands, "Vars.Visuals.Chams.bHands" );
	reset( Vars.Visuals.Chams.iHandMaterial, "Vars.Visuals.Chams.iHandMaterial" );
	reset( Vars.Visuals.Chams.bWeapon, "Vars.Visuals.Chams.bWeapon" );
	reset( Vars.Visuals.Chams.iWeaponMaterial, "Vars.Visuals.Chams.iWeaponMaterial" );
	reset( Vars.Visuals.Chams.bXQZ, "Vars.Visuals.Chams.bXQZ" );
	reset( Vars.Visuals.Chams.bDarkMode, "Vars.Visuals.Chams.bDarkMode" );
	reset( Vars.Visuals.bAsusWalls, "Vars.Visuals.Chams.bAsusWalls" );
	reset( Vars.Visuals.bAsusBlend, "Vars.Visuals.bAsusBlend" );
	reset_array( Vars.Visuals.Chams.Colors.flEnemyVisible, "Vars.Visuals.Chams.Colors.flEnemyVisible" );
	reset_array( Vars.Visuals.Chams.Colors.flEnemyHidden, "Vars.Visuals.Chams.Colors.flEnemyHidden" );
	reset_array( Vars.Visuals.Chams.Colors.flTeamVisible, "Vars.Visuals.Chams.Colors.flTeamVisible" );
	reset_array( Vars.Visuals.Chams.Colors.flTeamHidden, "Vars.Visuals.Chams.Colors.flTeamHidden" );

	reset( Vars.Visuals.MaterialConfig.bEnabled, "Vars.Visuals.MaterialConfig.bEnabled" );
	reset( Vars.Visuals.MaterialConfig.bDrawGrey, "Vars.Visuals.MaterialConfig.bDrawGrey" );
	reset( Vars.Visuals.MaterialConfig.bFullbright, "Vars.Visuals.MaterialConfig.bFullbright" );
	reset( Vars.Visuals.MaterialConfig.bShowLowResImage, "Vars.Visuals.MaterialConfig.bShowLowResImage" );
	reset( Vars.Visuals.MaterialConfig.bShowMipLevels, "Vars.Visuals.MaterialConfig.bShowMipLevels" );

	reset( Vars.HvH.bEnabled, "Vars.HvH.bEnabled" );
	reset( Vars.HvH.bLBYBreaker, "Vars.HvH.bLBYBreaker" );
	reset( Vars.HvH.Removals.bRecoil, "Vars.HvH.Removals.bRecoil" );

	reset( Vars.HvH.Aimbot.bEnabled, "Vars.HvH.Aimbot.bEnabled" );
	reset( Vars.HvH.Aimbot.bAutoFire, "Vars.HvH.Aimbot.bAutoFire" );
	reset( Vars.HvH.Aimbot.flMaxSpread, "Vars.HvH.Aimbot.flMaxSpread" );
	reset( Vars.HvH.Aimbot.flMinDamage, "Vars.HvH.Aimbot.flMinDamage" );

	output_file << std::setw( 4 ) << preset << std::endl;
	output_file.close();

	return true;
}