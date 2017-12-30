#include "..\CheatInclude.h"
#include "..\Math.h"

bool CBulletPenetration::HandleBulletPenetration( CCSWeaponData* wpn_data, FireBulletData& BulletData )
{
	using HandleBulletPenetrationFn = bool( __thiscall* )( C_BasePlayer*, float&, int&, int*, trace_t*, Vector*, float, float, float, int, int, float, int*, Vector*, float, float, float* );
	static HandleBulletPenetrationFn BulletHandler;

	if ( !BulletHandler )
		BulletHandler = ( HandleBulletPenetrationFn ) ( U::FindSig( strenc( "client.dll" ), strenc( "53 8B DC 83 EC 08 83 E4 F8 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 83 EC 78 8B 53 14" ) ) );

	int usestaticvalues = 0;
	auto enter_surface_data = g_pPhysicsSurfaceProps->GetSurfaceData( BulletData.enter_trace.surface.surfaceProps );
	int material = enter_surface_data->game.material;

	G::InAutowall = true;
	bool result = BulletHandler( reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( g_pEngine->GetLocalPlayer() ) ), wpn_data->flPenetration, material, &usestaticvalues, &BulletData.enter_trace, &BulletData.direction, 0.f, enter_surface_data->game.penetrationmodifier, enter_surface_data->game.damagemodifier, 0, 0x1002, wpn_data->flPenetration, &BulletData.penetrate_count, &BulletData.src, 0.f, 0.f, &BulletData.current_damage );
	G::InAutowall = false;
	return result;
}

bool CBulletPenetration::PenetrateWall( C_BasePlayer * pPlayer, C_BaseCombatWeapon * pWeapon, const Vector & vecPoint, float & flDamage )
{
	if ( !pPlayer || !pWeapon )
		return false;

	FireBulletData BulletData;
	BulletData.src = pPlayer->GetEyePosition();
	BulletData.filter.pSkip = pPlayer;

	QAngle qAngles = Math->CalcAngle( BulletData.src, vecPoint );

	Math->AngleVectors( qAngles, &BulletData.direction );
	BulletData.direction.Normalize();

	if ( !SimulateFireBullet( pPlayer, pWeapon, BulletData ) )
		return false;

	flDamage = BulletData.current_damage;

	return true;
}

bool CBulletPenetration::SimulateFireBullet( C_BasePlayer* pPlayer, C_BaseCombatWeapon* pWeapon, FireBulletData& BulletData )
{
	if ( !pPlayer || !pWeapon )
		return false;

	BulletData.penetrate_count = 4;
	BulletData.trace_length = 0.0f;
	CCSWeaponData* weaponData = pPlayer->GetActiveWeapon()->GetCSWpnData();

	if ( !weaponData )
		return false;

	BulletData.current_damage = ( float ) weaponData->iDamage;

	while ( ( BulletData.penetrate_count > 0 ) && ( BulletData.current_damage >= 1.0f ) )
	{
		BulletData.trace_length_remaining = weaponData->flRange - BulletData.trace_length;

		Vector end = BulletData.src + BulletData.direction * BulletData.trace_length_remaining;

		UTIL_TraceLine( BulletData.src, end, MASK_SHOT | CONTENTS_GRATE, pPlayer, &BulletData.enter_trace );
		UTIL_ClipTraceToPlayers( BulletData.src, end + BulletData.direction * 40.f, MASK_SHOT | CONTENTS_GRATE, &BulletData.filter, &BulletData.enter_trace );


		if ( BulletData.enter_trace.fraction == 1.0f )
			break;

		if ( ( BulletData.enter_trace.hitgroup <= 7 ) && ( BulletData.enter_trace.hitgroup > 0 ) )
		{
			BulletData.trace_length += BulletData.enter_trace.fraction * BulletData.trace_length_remaining;
			BulletData.current_damage *= pow( weaponData->flRangeModifier, BulletData.trace_length * 0.002 );

			auto pEntity = reinterpret_cast< C_BasePlayer* >( BulletData.enter_trace.m_pEnt );
			if ( !pEntity->IsEnemy() && Vars.Visuals.bTeamCheck )
				return false;

			ScaleDamage( BulletData.enter_trace.hitgroup, pEntity, weaponData->flArmorRatio, BulletData.current_damage );
			return true;
		}

		if ( HandleBulletPenetration( weaponData, BulletData ) )
			break;
	}
	return false;
}

void CBulletPenetration::ScaleDamage( int iHitgroup, C_BasePlayer* pPlayer, float flWeaponArmorRatio, float& flDamage )
{
	int armor = pPlayer->GetArmor();
	float ratio;

	switch ( iHitgroup )
	{
		case HITGROUP_HEAD:
			flDamage *= 4.f;
			break;
		case HITGROUP_STOMACH:
			flDamage *= 1.25f;
			break;
		case HITGROUP_LEFTLEG:
		case HITGROUP_RIGHTLEG:
			flDamage *= 0.75f;
			break;
	}

	if ( armor > 0 )
	{
		switch ( iHitgroup )
		{
			case HITGROUP_HEAD:
				if ( pPlayer->HasHelmet() )
				{
					ratio = ( flWeaponArmorRatio * 0.5 ) * flDamage;
					if ( ( ( flDamage - ratio ) * 0.5 ) > armor )
						ratio = flDamage - ( armor * 2.0 );
					flDamage = ratio;
				}
				break;
			case HITGROUP_GENERIC:
			case HITGROUP_CHEST:
			case HITGROUP_STOMACH:
			case HITGROUP_LEFTARM:
			case HITGROUP_RIGHTARM:
				ratio = ( flWeaponArmorRatio * 0.5 ) * flDamage;
				if ( ( ( flDamage - ratio ) * 0.5 ) > armor )
					ratio = flDamage - ( armor * 2.0 );
				flDamage = ratio;
				break;
		}
	}
}

bool CBulletPenetration::IsPenetrable( C_BasePlayer * pLocal, C_BasePlayer * pPlayer, Vector point, float& damage )
{
	if ( !pPlayer || !pLocal )
		return false;

	auto pWeapon = pLocal->GetActiveWeapon();
	if ( !pWeapon )
		return false;

	FireBulletData BulletData;
	BulletData.src = pLocal->GetEyePosition();
	BulletData.filter.pSkip = pLocal;

	QAngle qAngles = Math->CalcAngle( BulletData.src, point );

	Math->AngleVectors( qAngles, &BulletData.direction );
	BulletData.direction.Normalize();

	if ( !SimulateFireBullet( pLocal, pWeapon, BulletData ) )
		return false;

	if ( BulletData.current_damage < Vars.HvH.Aimbot.flMinDamage && BulletData.current_damage < pPlayer->GetHealth() )
		return false;

	damage = BulletData.current_damage;

	return true;
}

void CBulletPenetration::UTIL_TraceLine( const Vector& vecStart, const Vector& vecEnd, unsigned int nMask, C_BaseEntity* pCSIgnore, trace_t* pTrace )
{
	Ray_t ray;
	ray.Init( vecStart, vecEnd );

	CTraceFilter filter;
	filter.pSkip = pCSIgnore;
	g_pEngineTrace->TraceRay( ray, nMask, &filter, pTrace );
}

void CBulletPenetration::UTIL_ClipTraceToPlayers( const Vector& vecAbsStart, const Vector& vecAbsEnd, unsigned int mask, ITraceFilter* filter, trace_t* tr )
{
	static uintptr_t dwAddress;

	if ( !dwAddress )
		dwAddress = U::FindSig( strenc( "client.dll" ), strenc( "53 8B DC 83 EC 08 83 E4 F0 83 C4 04 55 8B 6B 04 89 6C 24 04 8B EC 81 EC ? ? ? ? 8B 43 10" ) );

	_asm
	{
		MOV		EAX, filter
		LEA		ECX, tr
		PUSH	ECX
		PUSH	EAX
		PUSH	mask
		LEA		EDX, vecAbsEnd
		LEA		ECX, vecAbsStart
		CALL	dwAddress
		ADD		ESP, 0xC
	}
}