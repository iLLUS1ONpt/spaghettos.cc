#pragma once

#define CHAR_TEX_CONCRETE	'C'			// texture types
#define CHAR_TEX_METAL		'M'
#define CHAR_TEX_DIRT		'D'
#define CHAR_TEX_VENT		'V'
#define CHAR_TEX_GRATE		'G'
#define CHAR_TEX_TILE		'T'
#define CHAR_TEX_SLOSH		'S'
#define CHAR_TEX_WOOD		'W'
#define CHAR_TEX_COMPUTER	'P'
#define CHAR_TEX_GLASS		'Y'
#define CHAR_TEX_FLESH		'F'
#define CHAR_TEX_SNOW		'N'
#define	HITGROUP_GENERIC	0
#define	HITGROUP_HEAD		1
#define	HITGROUP_CHEST		2
#define	HITGROUP_STOMACH	3
#define HITGROUP_LEFTARM	4	
#define HITGROUP_RIGHTARM	5
#define HITGROUP_LEFTLEG	6
#define HITGROUP_RIGHTLEG	7
#define HITGROUP_GEAR		10	

struct FireBulletData
{
	Vector						src;
	trace_t						enter_trace;
	Vector						direction;
	CTraceFilter				filter;
	float						trace_length;
	float						trace_length_remaining;
	float						current_damage;
	int							penetrate_count;
};

class CBulletPenetration
{
public:
	bool HandleBulletPenetration( CCSWeaponData* wpn_data, FireBulletData& BulletData );
	bool PenetrateWall( C_BasePlayer* pPlayer, C_BaseCombatWeapon* pWeapon, const Vector& vecPoint, float& flDamage );
	bool SimulateFireBullet( C_BasePlayer* pPlayer, C_BaseCombatWeapon* pWeapon, FireBulletData& BulletData );
	void UTIL_TraceLine( const Vector& vecStart, const Vector& vecEnd, unsigned int nMask, C_BaseEntity* pCSIgnore, trace_t* pTrace );
	void UTIL_ClipTraceToPlayers( const Vector & vecAbsStart, const Vector & vecAbsEnd, unsigned int mask, ITraceFilter * filter, trace_t * tr );
	void ScaleDamage( int iHitgroup, C_BasePlayer* pPlayer, float flWeaponArmorRatio, float& flDamage );
	bool IsPenetrable( C_BasePlayer * pLocal, C_BasePlayer * pPlayer, Vector point, float & damage );
};