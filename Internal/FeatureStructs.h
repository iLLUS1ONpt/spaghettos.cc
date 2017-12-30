#pragma once

struct AimbotData_t
{
	AimbotData_t(C_BasePlayer* player)
	{
		this->pPlayer = player;
	}
	C_BasePlayer*	pPlayer;
};

struct AntiaimData_t
{
	AntiaimData_t(int player, const float& dist, const bool& inair)
	{
		this->iPlayer = player;
		this->flDist = dist;
		this->bInAir = inair;
	}
	int				iPlayer;
	float			flDist;
	bool			bInAir;
};

struct VisualsInfo_t
{
	float	flFadeAlpha;
	bool	bBacktracking;
	bool	bFakeAngles;

	VisualsInfo_t()
	{
		flFadeAlpha	= 255.f;
		bBacktracking = false;
		bFakeAngles = false;
	}

	void AlphaClamp()
	{
		if ( flFadeAlpha > 255.f )	flFadeAlpha	= 255.f;
		if ( flFadeAlpha < 0.f )	flFadeAlpha	= 0.f;
	}
};

struct BulletImpact_t
{
	float			flImpactTime;
	Vector			vecImpactPos;
	Color			color;
	C_BasePlayer*	pPlayer;

	__forceinline BulletImpact_t()
	{
		vecImpactPos	= { 0.0f, 0.0f, 0.0f };
		flImpactTime	= 0.0f;
		color			= Color::White();
		pPlayer			= nullptr;
	}

	__forceinline BulletImpact_t( C_BasePlayer* player, Vector pos, float time, Color col = Color::White() )
	{
		pPlayer			= player;
		flImpactTime	= time;
		vecImpactPos	= pos;
		color			= col;
	}
};