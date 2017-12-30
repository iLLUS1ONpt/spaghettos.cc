#include "..\CheatInclude.h"
#include "..\Math.h"

void CGrenadePred::Paint()
{
	float throwtime = 0;
	for ( int i = deques.size() - 1; i >= 0; i-- )
	{
		auto dq = deques.begin() + i;

		auto currdeque = dq->path;

		if ( !Shoulddraw( *dq, throwtime ) )
		{
			deques.erase( dq );
			continue;
		}

		throwtime = dq->flThrowTime;

		Vector prev = currdeque.at( 0 ).vecPoint;
		Vector res1, res2;
		for ( auto it = currdeque.begin(), end = currdeque.end(); it != end; it++ )
		{
			if ( D::WorldToScreen( it->vecPoint, res2 ) )
			{
				if ( D::WorldToScreen( prev, res1 ) )
					D::DrawLine( res1.x, res1.y, res2.x, res2.y, Color( 255, 255, 255, it->flAlpha ) );
				if ( it->bHitSurface )
					D::DrawFilled3DBox( it->vecPoint, 4, 4, Color( 87, 222, 255, it->flAlpha ), Color( 87, 222, 255, it->flAlpha / 2 ) );
			}
			prev = it->vecPoint;
		}

		if ( D::WorldToScreen( prev, res2 ) && !( currdeque.end() - 1 )->bHitSurface )
			D::DrawFilled3DBox( prev, 4, 4, Color( 255, 105, 180, ( currdeque.end() - 1 )->flAlpha ), Color( 255, 105, 180, ( currdeque.end() - 1 )->flAlpha / 2 ) );

		if ( dq->bThrown )
			dq->Fade();

		if ( !currdeque.size() )
			deques.erase( dq );
	}
}

void CGrenadePred::Simulate()
{
	m_pLocalPlayer = reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( g_pEngine->GetLocalPlayer() ) );
	if ( !m_pLocalPlayer )
		return;

	C_BaseCombatWeapon* pWeapon = m_pLocalPlayer->GetActiveWeapon();
	if ( !pWeapon || deques.size() >= maxdeques )
		return;

	if ( pWeapon->IsGrenade() )
	{
		if ( !pWeapon->GetPinPulled() )
		{
			float fThrowTime = pWeapon->GetThrowTime();

			if ( ( fThrowTime == 0 ) )
				return;
		}
	}
	else
	{
		return;
	}

	QAngle vangles;
	g_pEngine->GetViewAngles( vangles );

	Vector vecSrc, vecThrow;
	Setup( vecSrc, vecThrow, vangles );

	float interval = g_pGlobalVarsBase->interval_per_tick;

	int logstep = static_cast< int >( 0.05f / interval );
	int logtimer = 0;

	std::deque<PathPoint> path;

	int bouncess = 0;
	bool first = false;

	for ( unsigned int i = 0; i < 1500; ++i )// 1500 so we dont drop too many frames in case of a very long smoke or decoy fall
	{
		static int s = 0;
		static int bounces = 0;
		if ( !logtimer )
		{
			path.push_back( PathPoint( vecSrc, ( s & 2 ) && first ? true : false ) );
			first = true;
		}

		s = Step( vecSrc, vecThrow, i, interval );
		if ( ( s & 1 ) )
			break;

		if ( ( s & 2 ) || logtimer >= logstep ) logtimer = 0;
		else ++logtimer;

		if ( ( s & 2 ) )
			bouncess++;

		if ( bouncess > 20 )
		{
			break;
		}
	}

	path.push_back( PathPoint( vecSrc ) );
	deques.push_back( GrenadePath( path, pWeapon->GetThrowTime(), duration, speed ) );
}

int CGrenadePred::Step( Vector & vecSrc, Vector & vecThrow, int tick, float interval )
{
	Vector move;
	AddGravityMove( move, vecThrow, interval, false );

	trace_t tr;
	PushEntity( vecSrc, move, tr );

	int result = 0;
	if ( CheckDetonate( vecThrow, tr, tick, interval ) || tr.fraction == 0.0f )
	{
		result |= 1;
	}

	if ( tr.fraction != 1.0f )
	{
		result |= 2;
		ResolveFlyCollisionCustom( tr, vecThrow, interval );
	}

	vecSrc = tr.endpos;

	return result;
}

bool CGrenadePred::CheckDetonate( const Vector & vecThrow, const trace_t & tr, int tick, float interval )
{
	switch ( m_pLocalPlayer->GetActiveWeapon()->GetWeaponID() )
	{
		case WEAPON_SMOKEGRENADE:
		case WEAPON_DECOY:
			if ( vecThrow.Length() < 0.1f )
				return true;

			return false;
		case WEAPON_MOLOTOV:
		case WEAPON_INCGRENADE:
			if ( tr.fraction != 1.0f && tr.plane.normal.z > 0.7f )
				return true;

		case WEAPON_FLASHBANG:
		case WEAPON_HEGRENADE:
			return static_cast< float >( tick )*interval > 1.5f && !( tick%static_cast< int >( 0.1f / interval ) );

		default:
			return false;
	}
}

void CGrenadePred::TraceHull( const Vector & src, const Vector & end, trace_t & tr )
{

	CTraceFilter filter;
	filter.pSkip = m_pLocalPlayer;

	Ray_t ray;
	ray.Init( src, end );
	ray.m_Extents = Vector( 2.0f, 2.0f, 2.0f );
	ray.m_IsRay = false;

	g_pEngineTrace->TraceRay( ray, MASK_SHOT_HULL, &filter, &tr );
}

void CGrenadePred::AddGravityMove( Vector & move, Vector & vel, float frametime, bool onground )
{
	Vector basevel( 0.0f, 0.0f, 0.0f );
	move.x = ( vel.x + basevel.x ) * frametime;
	move.y = ( vel.y + basevel.y ) * frametime;

	if ( onground )
		move.z = ( vel.z + basevel.z ) * frametime;
	else
	{
		float gravity = 800.0f * 0.4f;
		float newZ = vel.z - ( gravity * frametime );
		move.z = ( ( vel.z + newZ ) / 2.0f + basevel.z ) * frametime;
		vel.z = newZ;
	}
}

void CGrenadePred::PushEntity( Vector & src, const Vector & move, trace_t & tr )
{
	Vector vecAbsEnd = src;
	vecAbsEnd += move;

	TraceHull( src, vecAbsEnd, tr );
}

void CGrenadePred::ResolveFlyCollisionCustom( trace_t & tr, Vector & vecVelocity, float interval )
{
	float flSurfaceElasticity = 1.0f;
	if ( tr.m_pEnt && tr.m_pEnt->GetClientClass()->m_ClassID == 35 )
		flSurfaceElasticity = 0.3f;

	float flGrenadeElasticity = 0.45f;
	float flTotalElasticity = flGrenadeElasticity * flSurfaceElasticity;
	if ( flTotalElasticity > 0.9f ) flTotalElasticity = 0.9f;
	if ( flTotalElasticity < 0.0f ) flTotalElasticity = 0.0f;

	Vector vecAbsVelocity;
	PhysicsClipVelocity( vecVelocity, tr.plane.normal, vecAbsVelocity, 2.0f );
	vecAbsVelocity *= flTotalElasticity;

	float flSpeedSqr = vecAbsVelocity.LengthSqr();
	static const float flMinSpeedSqr = 20.0f * 20.0f;

	if ( tr.plane.normal.z > 0.7f )
	{
		if ( flSpeedSqr > 96000.f )
		{
			auto l = vecAbsVelocity.Normalized().Dot( tr.plane.normal );
			if ( l > 0.5f )
				vecAbsVelocity *= 1.f - l + 0.5f;
		}
		if ( flSpeedSqr < flMinSpeedSqr )
			vecAbsVelocity.Zero();
		else
		{
			vecVelocity = vecAbsVelocity;
			vecAbsVelocity *= ( 1.0f - tr.fraction ) * interval;
			PushEntity( tr.endpos, vecAbsVelocity, tr );
		}
	}
	else
		vecVelocity = vecAbsVelocity;
}

int CGrenadePred::PhysicsClipVelocity( const Vector & in, const Vector & normal, Vector & out, float overbounce )
{
	static const float STOP_EPSILON = 0.1f;

	float    backoff;
	float    change;
	float    angle;
	int        i, blocked;

	blocked = 0;

	angle = normal[ 2 ];

	if ( angle > 0 )
		blocked |= 1;
	if ( !angle )
		blocked |= 2;

	backoff = in.Dot( normal ) * overbounce;

	for ( i = 0; i < 3; i++ )
	{
		change = normal[ i ] * backoff;
		out[ i ] = in[ i ] - change;
		if ( out[ i ] > -STOP_EPSILON && out[ i ] < STOP_EPSILON )
			out[ i ] = 0;
	}

	return blocked;
}


void CGrenadePred::Setup( Vector& vecSrc, Vector& vecThrow, const QAngle& angEyeAngles )
{
	QAngle angThrow = angEyeAngles;
	float pitch = angThrow.x;

	float a = pitch - ( 90.0f - fabs( pitch ) ) * 10.0f / 90.0f;
	angThrow.x = a;

	float flVel = 750.0f * 0.9f;

	float flThrowStrength = m_pLocalPlayer->GetActiveWeapon()->GetThrowStrength();
	float b = flThrowStrength;

	b = b * 0.7f;
	b = b + 0.3f;
	flVel *= b;

	Vector vForward, vRight, vUp;
	Math->AngleVectors( angThrow, &vForward, &vRight, &vUp );

	vecSrc = m_pLocalPlayer->GetEyePosition();
	float off = ( flThrowStrength * 12.0f ) - 12.0f;
	vecSrc.z += off;

	trace_t tr;
	Vector vecDest = vecSrc;
	vecDest += vForward * 22.0f;
	TraceHull( vecSrc, vecDest, tr );

	Vector vecBack = vForward;
	vecBack *= 6.0f;
	vecSrc = tr.endpos;
	vecSrc -= vecBack;

	vecThrow = m_pLocalPlayer->GetVelocity();
	vecThrow *= 1.25f;
	vecThrow += vForward * flVel;
}


bool CGrenadePred::Shoulddraw( GrenadePath& gp, float prevthrowtime )
{
	for ( auto it = gp.path.begin(), end = gp.path.end(); it != end; ++it )
	{
		if ( gp.flThrowTime && g_pGlobalVarsBase->curtime + duration > gp.flThrowTime && gp.flThrowTime != prevthrowtime )
			return true;

		if ( gp.bThrown )
		{
			gp.bThrown = false;
			return true;
		}
	}

	return false;
}
