#include "..\CheatInclude.h"
#include "..\Math.h"
#include <algorithm>

void CMisc::Radar()
{
	if ( !Vars.Other.bRadar )
		return;

	for ( int i = 1; i <= g_pGlobalVarsBase->maxClients; i++ )
	{
		if ( i == g_pEngine->GetLocalPlayer() )
			continue;

		auto pPlayer = reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( i ) );
		if ( !pPlayer )
			continue;

		pPlayer->GetSpotted() = true;
	}
}

bool CMisc::FinishHalftimeFix( CUserCmd * cmd )
{
	m_pLocalPlayer = reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( g_pEngine->GetLocalPlayer() ) );

	if ( m_pLocalPlayer && m_pLocalPlayer->GetFlags() & FL_FROZEN )
	{
		// Don't stomp the first time we get frozen
		if ( m_bWasFrozen )
		{
			// Stomp the new viewangles with old ones
			cmd->viewangles = m_oldangles;
			return true;
		}
		else
		{
			m_bWasFrozen = true;
		}
	}
	else
	{
		m_bWasFrozen = false;
	}

	return false;
}

void CMisc::SetupHalftimeFix( CUserCmd * cmd )
{
	m_oldangles = cmd->viewangles;
}

void CMisc::FixTickbase( int &tickbase, CUserCmd* cmd )
{
	if ( !m_pOldCmd || !m_pLocalPlayer )
	{
		m_pOldCmd = cmd;
		return;
	}

	if ( m_pOldCmd->hasbeenpredicted )
		tickbase = m_pLocalPlayer->GetTickBase();
	else
		++tickbase;

	m_pOldCmd = cmd;

	g_pGlobalVarsBase->curtime = tickbase * g_pGlobalVarsBase->interval_per_tick;
}

void CMisc::Bhop(CUserCmd * cmd)
{
	if (!Vars.Other.bHop)
		return;

	static bool bLastJumped = false;
	static bool bShouldFake = false;

	if (!bLastJumped && bShouldFake)
	{
		bShouldFake = false;
		cmd->buttons |= IN_JUMP;
	}
	else if (cmd->buttons & IN_JUMP)
	{
		if (m_pLocalPlayer->GetFlags() & FL_ONGROUND)
		{
			bLastJumped = true;
			bShouldFake = true;
		}
		else
		{
			cmd->buttons &= ~IN_JUMP;
			bLastJumped = false;
		}
	}
	else
	{
		bLastJumped = false;
		bShouldFake = false;
	}
}

void CMisc::AutoStrafe(CUserCmd *cmd)
{
	if (!Vars.Other.bAutoStrafe)
		return;

	if (m_pLocalPlayer->GetMoveType() & (MOVETYPE_NOCLIP | MOVETYPE_LADDER))
		return;

	if (cmd->buttons & (IN_FORWARD | IN_MOVERIGHT | IN_MOVELEFT | IN_BACK))
		return;

	if (cmd->buttons & IN_JUMP || !(m_pLocalPlayer->GetFlags() & FL_ONGROUND))
	{
		if (cmd->mousedx > 1 || cmd->mousedx < -1)
		{
			cmd->sidemove = cmd->mousedx < 0.f ? -450.f : 450.f;
		}
		else
		{
			cmd->forwardmove = (10000.f) / (m_pLocalPlayer->GetVelocity().Length2D() + 1);
			cmd->sidemove = (cmd->command_number % 2) == 0 ? -450.f : 450.f;
		}
	}
}

void CMisc::CircleStrafer(CUserCmd *cmd)
{
	if (!Vars.Other.bCircleStrafe || !GetAsyncKeyState(0x43))
		return;

	static float StrafeAngle;

	Vector Velocity = m_pLocalPlayer->GetVelocity();
	Velocity.z = 0;

	float Speed = Velocity.Length();
	if (Speed < 45) Speed = 45;
	if (Speed > 750) Speed = 750;

	Ray_t ray;
	trace_t tr;
	CTraceFilterWorldOnly filter;
	ray.Init(m_pLocalPlayer->GetOrigin() + Vector(0, 0, 10), m_pLocalPlayer->GetOrigin() + Vector(0, 0, 10) + Velocity / 2.0f);
	g_pEngineTrace->TraceRay(ray, MASK_SHOT, &filter, &tr);

	float FinalPath = tr.fraction;
	float DeltaAngle = fmax((275.0f / Speed) * (2.0f / FinalPath) * (128.0f / (1.7f / g_pGlobalVarsBase->interval_per_tick)) * 4.f, 2.0f);
	StrafeAngle += DeltaAngle;

	if (fabs(StrafeAngle) >= 360.0f)
	{
		StrafeAngle = 0.0f;
	}
	else
	{
		cmd->forwardmove = cos((StrafeAngle + 90) * (M_PI / 180.0f)) * 450.f;
		cmd->sidemove = sin((StrafeAngle + 90) * (M_PI / 180.0f)) * 450.f;
	}
}

void CMisc::SetName( const char* newname )
{
	auto name = g_pCvar->FindVar( "name" );
	if ( !name )
		return;

	*reinterpret_cast< int* > ( reinterpret_cast< uintptr_t >( &name->fnChangeCallbacks + 0x15 ) ) = 0;
	name->SetValue( newname );
}

bool ShouldFake()
{
	bool will_update = false;
	static float lby_time = 0;

	INetChannelInfo* nci = g_pEngine->GetNetChannelInfo();

	float latency = nci->GetAvgLatency(FLOW_OUTGOING);

	float server_time = g_pGlobalVarsBase->curtime + latency;

	if (server_time >= (lby_time))
	{
		lby_time = server_time + 1.1f;
		will_update = true;
	}
	return will_update;
}
void CMisc::Fakelag()
{
	bool in_lbyjitter = ShouldFake(); // choke a tick if about to flick

	int variance = Vars.Other.bFakeLagJitter;
	int max_choke = Vars.Other.bFakeLagFactor;

	static int choked_ticks = 0;
	static int to_choke = 7;

		if (Vars.Other.bFakeLagMode)
		{
			switch (Vars.Other.bFakeLagMode)
			{
			case 0:
			{
				return;
			} break;

			case 1:
			{
				to_choke = max_choke;
			} break;
	
			case 2:
			{
				static int old_health = m_pLocalPlayer->GetHealth();
				if (old_health != m_pLocalPlayer->GetHealth())
				{
					old_health = m_pLocalPlayer->GetHealth();
					to_choke = RandomInt(0, max_choke);
				}
			} break;

			case 3:
			{
				to_choke = max(1, min((int)(fabs(m_pLocalPlayer->GetVelocity().Length() / 80.f)), max_choke));
			} break;

			case 4:
			{
				to_choke = RandomInt(0, max_choke);
			}
		}

		bool bRandom = RandomInt(0, 1) == 0;
		static int variance = RandomInt(0, variance);

		to_choke += bRandom ? variance : -(variance);
		to_choke = to_choke, 0, max_choke;

		if (choked_ticks <= to_choke && !in_lbyjitter)
		{
			bSendPacket = false;
			choked_ticks++;
		}
		else
		{
			bSendPacket = true;
			choked_ticks = 0;
		}
	}
	else
	{
		choked_ticks = 0;
	}
}