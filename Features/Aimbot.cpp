#include "..\CheatInclude.h"
#include "..\Math.h"

void CAimbot::Run( CUserCmd * cmd, bool & sendpacket )
{
	G::CanShoot = false;

	Entities.clear();

	if ( !Vars.HvH.Aimbot.bEnabled )
		return;

	m_pLocalPlayer = reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( g_pEngine->GetLocalPlayer() ) );
	if ( !m_pLocalPlayer || !m_pLocalPlayer->IsAlive() )
		return;

	m_bSendPacket = &sendpacket;

	m_pCmd = cmd;

	SelectTarget();

	ChooseAngles();
}

bool CAimbot::CanShoot()
{
	C_BaseCombatWeapon* pWeapon = m_pLocalPlayer->GetActiveWeapon();

	if ( !pWeapon )
		return false;

	auto weapontype = pWeapon->GetWeaponType();
	if ( weapontype == WEAPONTYPE_KNIFE || weapontype == WEAPONTYPE_C4 || pWeapon->IsGrenade() )
		return false;

	if ( pWeapon->IsEmpty() )
		return false;

	if ( pWeapon->InReload() )
		return false;

	if ( pWeapon->GetNextPrimaryAttack() > g_pGlobalVarsBase->curtime )
		return false;

	if ( m_pLocalPlayer->GetNextAttack() > g_pGlobalVarsBase->curtime )
		return false;

	if ( pWeapon->GetWeaponID() == WEAPON_REVOLVER )
	{
		if (!Vars.HvH.Aimbot.bAutoR8)
			return false;

		if ( Vars.HvH.Aimbot.bAutoFire )
			m_pCmd->buttons |= IN_ATTACK;

		float flPostponeFireReady = pWeapon->GetPostponeFireReadyTime();
		if ( !flPostponeFireReady || flPostponeFireReady - 0.5f >= g_pGlobalVarsBase->curtime )
			return false;

		if ( Vars.HvH.Aimbot.bAutoFire )
			m_pCmd->buttons &= ~IN_ATTACK;
	}

	pWeapon->UpdateAccuracyPenalty();
	float cone = pWeapon->GetSpread() + pWeapon->GetInaccuracy();
	cone *= 1000;
	if ( cone > Vars.HvH.Aimbot.flMaxSpread && Vars.HvH.Aimbot.bAutoFire )
		return false;

	return true;
}

void CAimbot::SelectTarget()
{
	if ( !CanShoot() )
		return;

	for ( int index = 1; index <= g_pGlobalVarsBase->maxClients; index++ )
	{
		auto pPlayer = reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( index ) );

		if ( !IsViable( pPlayer ) )
			continue;

		AimbotData_t data = AimbotData_t( pPlayer );

		Entities.push_back( data );
	}
}

void CAimbot::ChooseAngles()
{
	C_BasePlayer* pBest = NULL;
	Vector vecBestHitboxPos;
	QAngle angAimAngles;
	int besttickcount = m_pCmd->tick_count;
	bool backtrack = false;

	float flBestDamage = 0.f;

	std::vector<Hitboxes> Hitboxes;
	if (Vars.HvH.Aimbot.bHitscan == 0)
	{
		switch (Vars.HvH.Aimbot.bHitbox)
		{
		case 0:
			Hitboxes.push_back(HITBOX_HEAD);
			break;
		case 1:
			Hitboxes.push_back(HITBOX_NECK);
			break;
		case 2:
			Hitboxes.push_back(HITBOX_CHEST);
			break;
		case 3:
			Hitboxes.push_back(HITBOX_PELVIS);
			break;
			Hitboxes.push_back(HITBOX_BODY);
		case 4:
			Hitboxes.push_back(HITBOX_RIGHT_FOOT);
			break;
		case 5:
			Hitboxes.push_back(HITBOX_LEFT_FOOT);
			break;
		}
	}
	else if (Vars.HvH.Aimbot.bHitscan)
	{
		switch (Vars.HvH.Aimbot.bHitscan)
		{
		case 0: //Off
			break;
		case 1: //Low
			Hitboxes.push_back(HITBOX_PELVIS);
			Hitboxes.push_back(HITBOX_HEAD);
			Hitboxes.push_back(HITBOX_NECK);
			Hitboxes.push_back(HITBOX_CHEST);
			Hitboxes.push_back(HITBOX_BODY);
			Hitboxes.push_back(HITBOX_THORAX);
			break;
		case 2: //Medium
			Hitboxes.push_back(HITBOX_HEAD);
			Hitboxes.push_back(HITBOX_NECK);
			Hitboxes.push_back(HITBOX_LOWER_NECK);
			Hitboxes.push_back(HITBOX_UPPER_CHEST);
			Hitboxes.push_back(HITBOX_CHEST);
			Hitboxes.push_back(HITBOX_RIGHT_FOREARM);
			Hitboxes.push_back(HITBOX_RIGHT_UPPER_ARM);
			Hitboxes.push_back(HITBOX_LEFT_FOREARM);
			Hitboxes.push_back(HITBOX_LEFT_UPPER_ARM);
			Hitboxes.push_back(HITBOX_BODY);
			Hitboxes.push_back(HITBOX_PELVIS);
			Hitboxes.push_back(HITBOX_THORAX);
			break;
		case 3: //High
			Hitboxes.push_back(HITBOX_HEAD);
			Hitboxes.push_back(HITBOX_NECK);
			Hitboxes.push_back(HITBOX_LOWER_NECK);
			Hitboxes.push_back(HITBOX_PELVIS);
			Hitboxes.push_back(HITBOX_BODY);
			Hitboxes.push_back(HITBOX_THORAX);
			Hitboxes.push_back(HITBOX_CHEST);
			Hitboxes.push_back(HITBOX_UPPER_CHEST);
			Hitboxes.push_back(HITBOX_RIGHT_THIGH);
			Hitboxes.push_back(HITBOX_LEFT_THIGH);
			Hitboxes.push_back(HITBOX_RIGHT_CALF);
			Hitboxes.push_back(HITBOX_LEFT_CALF);
			Hitboxes.push_back(HITBOX_RIGHT_FOOT);
			Hitboxes.push_back(HITBOX_LEFT_FOOT);
			Hitboxes.push_back(HITBOX_RIGHT_HAND);
			Hitboxes.push_back(HITBOX_LEFT_HAND);
			Hitboxes.push_back(HITBOX_RIGHT_UPPER_ARM);
			Hitboxes.push_back(HITBOX_RIGHT_FOREARM);
			Hitboxes.push_back(HITBOX_LEFT_UPPER_ARM);
			Hitboxes.push_back(HITBOX_LEFT_FOREARM);
			break;
		}
	}

	for ( auto& player : Entities )
	{
		Vector playerBestPoint;
		float playerBestDamage = 0.f;

		C_BasePlayer* pPlayer = player.pPlayer;
		if ( !pPlayer )
			continue;

		std::vector<Vector> Points;
		for ( auto& hitbox : Hitboxes )
		{
			if ( !pPlayer->GetPointsFromHitbox( m_pLocalPlayer, hitbox, Points ) )
				continue;
		}

		for ( auto& point : Points )
		{
			float tempdamage;
			if ( F::BulletPenetration->IsPenetrable( m_pLocalPlayer, pPlayer, point, tempdamage ) )
			{
				if ( tempdamage > playerBestDamage )
				{
					playerBestPoint = point;
					playerBestDamage = tempdamage;
				}
			}
		}

		if ( playerBestDamage >= pPlayer->GetHealth() )
		{
			pBest = pPlayer;
			vecBestHitboxPos = playerBestPoint;
			flBestDamage = playerBestDamage;
			break;
		}

		if ( playerBestDamage > flBestDamage )
		{
			pBest = pPlayer;
			vecBestHitboxPos = playerBestPoint;
			flBestDamage = playerBestDamage;
		}

		F::Backtrack->BackupPlayer( pPlayer );

		besttickcount = F::Backtrack->GetBestTick( pPlayer );

		std::vector<Vector> BtPoints;
		for ( auto& hitbox : Hitboxes )
		{
			if ( !pPlayer->GetPointsFromHitbox( m_pLocalPlayer, hitbox, BtPoints ) )
				continue;
		}

		for ( auto& point : BtPoints )
		{
			float tempdamage;
			if ( F::BulletPenetration->IsPenetrable( m_pLocalPlayer, pPlayer, point, tempdamage ) )
			{
				if ( tempdamage > playerBestDamage )
				{
					playerBestPoint = point;
					playerBestDamage = tempdamage;
				}
			}
		}

		F::Backtrack->RestorePlayer( pPlayer );

		if ( playerBestDamage >= pPlayer->GetHealth() )
		{
			pBest = pPlayer;
			vecBestHitboxPos = playerBestPoint;
			flBestDamage = playerBestDamage;
			backtrack = true;
			break;
		}

		if ( playerBestDamage > flBestDamage )
		{
			pBest = pPlayer;
			vecBestHitboxPos = playerBestPoint;
			flBestDamage = playerBestDamage;
			backtrack = true;
		}
	}

	if ( !pBest )
		return;

	G::CanShoot = true;

	angAimAngles = Math->CalcAngle( m_pLocalPlayer->GetEyePosition(), vecBestHitboxPos );
	angAimAngles.Normalize();
	angAimAngles.Clamp();

	static int vars_delaybla = 2;
	static int delay = 0;

	if ( Vars.HvH.Aimbot.bAutoFire && !( m_pCmd->buttons & IN_ATTACK ) )
	{
		if ( delay < vars_delaybla )
		{
			delay++;
			return;
		}
		delay = 0;

		m_pCmd->buttons |= IN_ATTACK;
	}

	if ( m_pCmd->buttons & IN_ATTACK )
	{
		if ( backtrack )
		{
			m_pCmd->tick_count = besttickcount;
			G::Msg( "Hit backtracked " );
		}
		G::InAimbot = true;
		m_pCmd->viewangles = angAimAngles;
		*m_bSendPacket = false;
	}
}

bool CAimbot::IsViable( C_BasePlayer * pPlayer )
{
	if ( !pPlayer )
		return false;

	if ( pPlayer->IsDormant() || !pPlayer->IsAlive() || pPlayer->GetFlags() & FL_FROZEN )
		return false;

	if ( !pPlayer->IsEnemy() && Vars.Visuals.bTeamCheck )
		return false;

	if ( pPlayer->GetClientClass()->m_ClassID != 35 )
		return false;

	if ( pPlayer == m_pLocalPlayer )
		return false;

	if ( pPlayer->GetImmunity() )
		return false;

	return true;
}