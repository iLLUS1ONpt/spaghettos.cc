#include "..\CheatInclude.h"
#include "..\Math.h"

void CAntiAim::Run( CUserCmd * cmd, bool & sendpacket )
{
	m_pCmd = cmd;

	m_bSendPacket = &sendpacket;

	Entities.clear();

	F::Animations->UpdateServerAnimations( m_pCmd, m_bSendPacket );

	if ( !Vars.HvH.bEnabled || G::InAimbot )
		return;

	if (Vars.Other.bFakeLagMode == 0)
	{
		*m_bSendPacket = !(cmd->command_number % 3) ? true : false;
	}

	m_pLocalPlayer = reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( g_pEngine->GetLocalPlayer() ) );
	if ( !m_pLocalPlayer || m_pLocalPlayer->GetImmunity() || ( *G::GameRules )->m_bFreezePeriod() )
		return;

	if ( m_pLocalPlayer->GetMoveType() == MOVETYPE_LADDER || m_pLocalPlayer->GetMoveType() == MOVETYPE_NOCLIP )
		return;

	if ( !m_pLocalPlayer->IsAlive() )
		sendpacket = true;

	auto activeweapon = m_pLocalPlayer->GetActiveWeapon();
	int id = m_pLocalPlayer->GetActiveWeapon()->GetWeaponID();

	if ( ( cmd->buttons & IN_ATTACK && ( !Vars.HvH.Aimbot.bEnabled || activeweapon->GetWeaponType() == WEAPONTYPE_KNIFE ) )
		 || ( cmd->buttons & IN_ATTACK2 && ( activeweapon->GetWeaponType() == WEAPONTYPE_KNIFE || ( id == WEAPON_REVOLVER && !Vars.HvH.Aimbot.bEnabled ) ) )
		 || cmd->buttons & IN_USE )
		return;

	if ( id == WEAPON_REVOLVER && !Vars.HvH.Aimbot.bAutoFire )
	{
		float flPostponeFireReady = activeweapon->GetPostponeFireReadyTime();
		if ( flPostponeFireReady && flPostponeFireReady - 0.5f < g_pGlobalVarsBase->curtime )
			return;
	}

	if ( activeweapon->IsBeingThrown() )
		return;

	G::InAntiaim = true;

	SelectTarget();

	ChooseAngles();

	ChooseFake();

	AntiAims();
}

void CAntiAim::SelectTarget()
{
	for ( int index = 1; index <= g_pGlobalVarsBase->maxClients; index++ )
	{
		auto pPlayer = reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( index ) );

		if ( !IsViable( pPlayer ) )
			continue;

		float flDist = pPlayer->GetAbsOrigin().Dist( m_pLocalPlayer->GetAbsOrigin() );
		bool bInAir = !( pPlayer->GetFlags() & FL_ONGROUND );

		AntiaimData_t data( index, flDist, bInAir );

		Entities.push_back( data );
	}
}

void CAntiAim::ChooseAngles()
{
	if (!Vars.HvH.bEdge)
		return;

	if ( Entities.empty() )
	{
		m_pCmd->viewangles.y += 180;
		return;
	}

	auto eyepos = m_pLocalPlayer->GetEyePosition();
	auto headpos = m_pLocalPlayer->GetHitboxPos( 0 );
	auto origin = m_pLocalPlayer->GetAbsOrigin();

	auto checkWallThickness = [ & ] ( C_BasePlayer* pPlayer, Vector newhead ) -> float
	{
		Vector endpos1, endpos2;
		Vector eyepos = pPlayer->GetEyePosition();

		Ray_t ray;
		ray.Init( newhead, eyepos );

		CTraceFilterSkipTwoEntities filter( pPlayer, m_pLocalPlayer );

		trace_t trace1, trace2;
		g_pEngineTrace->TraceRay( ray, MASK_SHOT_BRUSHONLY, &filter, &trace1 );

		if ( trace1.DidHit() )
			endpos1 = trace1.endpos;
		else
			return 0.f;

		ray.Init( eyepos, newhead );
		g_pEngineTrace->TraceRay( ray, MASK_SHOT_BRUSHONLY, &filter, &trace2 );

		if ( trace2.DidHit() )
			endpos2 = trace2.endpos;

		float add = newhead.Dist( eyepos ) - m_pLocalPlayer->GetEyePosition().Dist( eyepos ) + 3.f;
		return endpos1.Dist( endpos2 ) + add / 3;
	};

	float bestrotation = 0.f;
	float highestthickness = 0.f;
	Vector besthead;

	float step = M_PI * 2.0 / 15;
	float radius = Vector( headpos - origin ).Length2D();
	for ( float rotation = 0; rotation < ( M_PI * 2.0 ); rotation += step )
	{
		Vector newhead( radius * cos( rotation ) + eyepos.x, radius * sin( rotation ) + eyepos.y, eyepos.z );

		float totalthickness = 0.f;

		for ( auto current : Entities )
			totalthickness += checkWallThickness( reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( current.iPlayer ) ), newhead );

		if ( totalthickness > highestthickness )
		{
			highestthickness = totalthickness;
			bestrotation = rotation;
			besthead = newhead;
		}
	}
	if ( highestthickness != 0 )
	{
		m_pCmd->viewangles.y = RAD2DEG( bestrotation );
		if ( g_pInput->m_fCameraInThirdPerson )
			g_pVDebugOverlay->AddBoxOverlay( headpos, Vector( -3.5f, -3.5f, -3.5f ), Vector( 3.5f, 3.5f, 3.5f ), Vector( 0.f, 0.f, 0.f ), 255, 105, 180, 10, g_pGlobalVarsBase->interval_per_tick * 2 );
	}
	else
	{
		m_pCmd->viewangles.y += 180;
	}
}

void CAntiAim::ChooseFake()
{
	if (!Vars.HvH.bLBYBreaker)
		return;

	static bool fake = false;
	static int choke = 0;

	if ( m_pLocalPlayer->GetVelocity().Length2D() > 20.f )
		choke = RandomInt( 5, 10 );
	else
		choke = 1;

	if ( choke < g_pClientState->m_nChokedCommands() )
		fake = false;
	else
		fake = true;

	if ( fake )
		*m_bSendPacket = false;

	Fakewalk();

	FakeLby();

	if ( *m_bSendPacket )
		m_pCmd->viewangles.y += RandomFlt( 130.f, 260.f );
}

void CAntiAim::AntiAims()
{
	static bool fake = false;
	static int choke = 0;

	if (choke < g_pClientState->m_nChokedCommands())
		fake = false;
	else
		fake = true; //fake

	if (fake) //real
		*m_bSendPacket = false;

	//Pitches
	switch (Vars.HvH.bPitch)
	{
	case 0:
	{
		return;
	}
	break;

	case 1: //down
	{
		m_pCmd->viewangles.x = 89.f;
	}
	break;

	case 2: //b1g savage up
	{
		m_pCmd->viewangles.x = -89.f;
	}
	break;

	case 3: //b1g emotion
	{
		m_pCmd->viewangles.x = 87.32f;
	}
	break;

	case 4: //fake down mootiny
	{
		m_pCmd->viewangles.x = 1169;
	}
	break;

	case 5: //fake up mootiny? xd
	{
		m_pCmd->viewangles.x = 1000.f;
	}
	break;
	case 6: //jitter down
	{
		m_pCmd->viewangles.x = RandomFlt(69.f, 89.f);
	}
	}

	//Real Yaws
	if (fake)
	{
		switch (Vars.HvH.bRealYaw)
		{
		case 0:
		{
			return;
		}
		break;

		case 1: //Masterlooser
		{
			int value = rand() % 361;
			float clamped = m_pLocalPlayer->GetLowerBodyYawTarget() + value;
			if (fabsf(m_pLocalPlayer->GetLowerBodyYawTarget() - clamped) < 45)
				value += 50;
			else if (fabsf(m_pLocalPlayer->GetLowerBodyYawTarget() - clamped) > 310)
				value -= 50;

			m_pCmd->viewangles.y = m_pLocalPlayer->GetLowerBodyYawTarget() + value - 180;
		}
		break;

		case 2: //Sideways Jitter
		{
			m_pCmd->viewangles.y = RandomFlt(90, -90);
		}
		break;

		case 3: //LBY Switch
		{
			static bool pFlip;
			static float oldLBY = 0.0f;
			float LBY = m_pLocalPlayer->GetLowerBodyYawTarget();
			if (LBY != oldLBY) // did lowerbody update?
			{
				pFlip = !pFlip;
				oldLBY = LBY;
			}
			m_pCmd->viewangles.y = pFlip ? m_pCmd->viewangles.y - 90.f : m_pCmd->viewangles.y + 90.f;
		}
		break;

		case 4: //b1g pycache aa
		{
			static bool flip2;
			if (!fake)
			{
				flip2 = !flip2;
				if (flip2)
				{
					m_pCmd->viewangles.y += RandomFlt(40.0f, 135.0f);
				}
				else
				{
					m_pCmd->viewangles.y -= RandomFlt(40.0f, 135.0f);
					m_pCmd->viewangles.y -= 90.f;
				}
			}
			else
			{
				m_pCmd->viewangles.y += 180.0f;

				if (m_pLocalPlayer->GetVelocity().Length2D() < 0.1) //standind still
					m_pCmd->viewangles.y += RandomFlt(0.0f, 60.0f) - 30;
			}
		}
		break;
		}
	}

	//Fake Yaws
	if (!fake)
	{
		switch (Vars.HvH.bFakeYaw)
		{
		case 0:
		{
			return;
		}
		break;

		case 1: //LBY Breaker
		{
			static bool bJitter; bJitter = !bJitter;
			static bool quick_switch;
			static bool lby_updated_tick;
			static int state;

			if (lby_updated_tick)
			{
				if (state < 2)
				{
					state++;
					quick_switch = true;
				}
				if (state >= 2)
				{
					state = 0;
					quick_switch = false;
				}
			}
			else
				quick_switch = false;

			m_pCmd->viewangles.y = quick_switch ? -180 : (180 + bJitter ? 30 : -30);
		}
		break;

		case 2: //Fast Spin
		{
			static QAngle Angle;
			Angle.y += 50;
			m_pCmd->viewangles.y = Angle.y;
		}
		break;

		case 3: //Slow Spin
		{
			static QAngle Angle;
			Angle.y += 5;
			m_pCmd->viewangles.y = Angle.y;
		}
		break;

		case 4: //Manual LBY
		{
			static bool dir = false;
			static bool shouldflip;

			if (GetAsyncKeyState(VK_LEFT)) dir = false; else if (GetAsyncKeyState(VK_RIGHT)) dir = true;

			if (dir && m_pLocalPlayer->GetVelocity().Length2D() < 1)
			{
				if (shouldflip)
				{
					m_pCmd->viewangles.y = -110;
				}
				else
				{
					m_pCmd->viewangles.y = +135;
				}
			}
			else if (!dir && m_pLocalPlayer->GetVelocity().Length2D() < 1)
			{
				if (shouldflip)
				{
					m_pCmd->viewangles.y = +110;
				}
				else
				{
					m_pCmd->viewangles.y = -135;
				}
			}
			else if (m_pLocalPlayer->GetVelocity().Length2D() > 0)
			{
				m_pCmd->viewangles.y = +180;
			}
		}
		break;

		case 5: //pycache Jitter
		{
			static bool flip2;
			if (!fake)
			{
				flip2 = !flip2;
				if (flip2)
				{
					m_pCmd->viewangles.y += RandomFlt(40.0f, 135.0f);
				}
				else
				{
					m_pCmd->viewangles.y -= RandomFlt(40.0f, 135.0f);
					m_pCmd->viewangles.y -= 90.f;
				}
			}
			else
			{
				m_pCmd->viewangles.y += 180.0f;

				if (m_pLocalPlayer->GetVelocity().Length2D() < 0.1) //standind still
					m_pCmd->viewangles.y += RandomFlt(0.0f, 60.0f) - 30;
			}
		}
		break;
		}
	}
}

bool CAntiAim::IsViable( C_BasePlayer * pPlayer )
{
	if ( !pPlayer )
		return false;

	if ( pPlayer->IsDormant() || !pPlayer->IsAlive() || pPlayer->GetFlags() & FL_FROZEN )
		return false;

	auto pWeapon = pPlayer->GetActiveWeapon();
	if ( !pWeapon )
		return false;

	int iWeaponType = pWeapon->GetWeaponType();

	if ( iWeaponType == WEAPONTYPE_C4 || iWeaponType == WEAPONTYPE_GRENADE || iWeaponType == WEAPONTYPE_KNIFE )
		return false;

	if ( !pPlayer->IsEnemy() && Vars.Visuals.bTeamCheck )
		return false;

	return true;
}

void CAntiAim::Fakewalk()
{
	if ( !m_pLocalPlayer->IsAlive() )
		return;

	static auto sv_stopspeed = g_pCvar->FindVar( "sv_stopspeed" );
	static auto sv_friction = g_pCvar->FindVar( "sv_friction" );

	static int tochoke = 15;
	int neededsteps = 0;

	Vector initialspeed = m_pLocalPlayer->GetVelocity();

	auto calcstep = [ & ] () -> void
	{
		float	speed, newspeed, control;
		float	friction;
		float	drop = 0;

		friction = sv_friction->GetFloat();
		speed = initialspeed.Length2D();
		control = ( speed < sv_stopspeed->GetFloat() ) ? sv_stopspeed->GetFloat() : speed;
		drop += control * friction *g_pGlobalVarsBase->frametime; 

		newspeed = speed - drop;

		if ( newspeed < 0 )
			newspeed = 0;

		if ( newspeed != speed )
		{
			newspeed /= speed;

			initialspeed.x *= newspeed;
			initialspeed.y *= newspeed;
			initialspeed.z *= newspeed;
		}
	};

	if (!GetAsyncKeyState(VK_LSHIFT))
	{
		tochoke = 15;
		return;
	}

	if ( tochoke <= 0 || initialspeed.Length2D() < 0.1f /*|| g_pClientState->m_nChokedCommands() >= 15*/ )
	{
		*m_bSendPacket = true;
		tochoke = 15;
		return;
	}

	while ( initialspeed.Length2D() > 0.1f )
	{
		calcstep();
		neededsteps++;
		*m_bSendPacket = false;
	}

	if ( neededsteps >= tochoke )
	{
		//float max = 450 / max( m_pCmd->sidemove, m_pCmd->forwardmove );
		if ( m_pCmd->sidemove != 0.f )
			m_pCmd->sidemove *= 0;

		if ( m_pCmd->forwardmove != 0.f )
			m_pCmd->forwardmove *= 0;
	}
	tochoke--;
}

bool CAntiAim::FakeLby()
{
	if ( !g_pEngine->IsInGame() || !m_pLocalPlayer || !m_pLocalPlayer->IsAlive() )
		return false;

	if ( !Vars.HvH.bLBYBreaker || !Vars.HvH.bEnabled )
		return false;

	if ( bUpdateLby && g_pGlobalVarsBase->curtime > F::Animations->m_flFirstBodyUpdate &&  F::Animations->m_serverAnimState->m_flSpeed < 0.1f )
	{
		float turn = RandomFlt( 165.f, 180.f ); //stop using rand
		m_pCmd->viewangles.y += RandomInt( 0, 1 ) ? -turn : turn;
		bUpdateLby = false;
		return true;
	}
	return false;
}

void CAntiAim::MovementFix()
{
	QAngle vOldAngles;
	g_pEngine->GetViewAngles( vOldAngles );
	float fOldForward = m_pCmd->forwardmove;
	float fOldSidemove = m_pCmd->sidemove;
	float deltaView = m_pCmd->viewangles.y - vOldAngles.y;
	float f1;
	float f2;

	if ( vOldAngles.y < 0.f )
		f1 = 360.0f + vOldAngles.y;
	else
		f1 = vOldAngles.y;

	if ( m_pCmd->viewangles.y < 0.0f )
		f2 = 360.0f + m_pCmd->viewangles.y;
	else
		f2 = m_pCmd->viewangles.y;

	if ( f2 < f1 )
		deltaView = abs( f2 - f1 );
	else
		deltaView = 360.0f - abs( f1 - f2 );
	deltaView = 360.0f - deltaView;

	m_pCmd->viewangles.Clamp();

	m_pCmd->forwardmove = cos( DEG2RAD( deltaView ) ) * fOldForward + cos( DEG2RAD( deltaView + 90.f ) ) * fOldSidemove;
	m_pCmd->sidemove = sin( DEG2RAD( deltaView ) ) * fOldForward + sin( DEG2RAD( deltaView + 90.f ) ) * fOldSidemove;
	if ( m_pCmd->viewangles.x >= 180 && m_pCmd->viewangles.x <= 270 ) m_pCmd->forwardmove = -m_pCmd->forwardmove;
}