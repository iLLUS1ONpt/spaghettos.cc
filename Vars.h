#pragma once

struct Variables
{
	Variables()
	{

	}
	struct
	{
		bool	bThirdperson;
		int		bThirdpersonMode;
		float	flThirdpersonDist = 60.f;
		bool	bRadar;
		bool	bHop;
		bool	bAutoStrafe;
		bool	bCircleStrafe;
		bool	bHitsound;
		int		bFakeLagMode;
		int		bFakeLagJitter;
		int		bFakeLagFactor;
	} Other;

	struct
	{
		bool	bOpened = false;
		int		iTab = 0;
	} Menu;

	struct
	{
		bool	bEnabled = true;
	} SkinChanger;

	struct
	{
		bool	bSniperCrosshair;
		bool	bHitmarker;
		bool	bHitmarkerHitbox;
		bool	bBulletImpacts;
		bool	bFovChanger; //enabler
		float	flFov; //slider
		bool	bViewmodelFovChanger; //enabler
		float	flViewmodelFov; //slider
		bool	bSpreadCircle;
		bool	bGrenadePrediction;
		bool	bTeamCheck;
		bool	bAsusWalls;
		float	bAsusBlend = 1.f;
		bool	bAngleLines;
		struct
		{
			float   flHitmarker[ 3 ];
			float   flTracers[ 3 ];
			float	flTracersDuration;
			float	flTracersWidth;
		} Colors;

		struct
		{
			bool	bEnabled;
			bool	bSkeleton;
			bool	bSkeletonBacktrack;
			bool	bBox;
			bool	bFilled;
			bool	bFade;
			float	flFadeSpeed;
			bool	bDlight;
			struct
			{
				float   flEnemyBoxes[ 3 ];
				float   flEnemyBones[ 3 ];
				float   flEnemyHitBoxes[ 3 ];
				float   flTeamBoxes[ 3 ];
				float   flTeamBones[ 3 ];
				float   flTeamHitBoxes[ 3 ];
			} Colors;
		} PlayerEsp;

		struct
		{
			bool	bEnabled;
			bool	bPlayers;
			bool	bHands;
			bool	bWeapon;
			int		iHandMaterial;
			int		iWeaponMaterial;
			bool	bXQZ;
			bool	bOutlined;
			bool	bDarkMode;
			struct
			{
				float   flEnemyVisible[ 3 ];
				float   flEnemyHidden[3];
				float   flTeamVisible[ 3 ];
				float   flTeamHidden[ 3 ];
			} Colors;
			
		} Chams;

		struct
		{
			bool	bEnabled;
			bool	bDrawGrey;
			bool	bFullbright;
			bool	bShowLowResImage;
			bool	bShowMipLevels;

		} MaterialConfig;
	} Visuals;

	struct
	{
		bool	bEnabled;
		bool	bEdge;
		bool	bLBYBreaker;
		int		bPitch;
		int		bRealYaw;
		int		bFakeYaw;
		struct
		{
			bool	bRecoil;
		} Removals;
		struct
		{
			bool	bEnabled;
			bool	bAutoFire;
			bool	bResolver;
			float	bBFAfter;
			bool	bAutoR8;
			int		bHitscan;
			int		bHitbox;
			float	flMaxSpread;
			float	flMinDamage;
		} Aimbot;
	} HvH;
};

extern Variables Vars;