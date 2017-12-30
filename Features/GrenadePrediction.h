#pragma once

struct PathPoint
{
	PathPoint( const Vector& point, const bool& bHitSurface = false )
	{
		this->vecPoint = point;
		this->bHitSurface = bHitSurface;
		this->flAlpha = 255.f;
	}
	Vector	vecPoint;
	bool	bHitSurface;
	float	flAlpha;
};

struct GrenadePath
{
	GrenadePath( const std::deque<PathPoint>& path, const float& flThrowTime, const float& flDuration, const float& flFadeSpeed )
	{
		this->path = path;
		this->flThrowTime = flThrowTime;
		this->flDuartion = flDuration;
		this->flFadeSpeed = flFadeSpeed;
		this->iInitialPathSize = path.size();
		this->bThrown = true;
		this->step = 0;
	}
	void Fade()
	{
		step += g_pGlobalVarsBase->frametime;
		if ( step <= flDuartion )
			return;

		for ( int i = path.size() - 1; i >= 0; i-- )
		{
			auto it = path.begin() + i;

			if ( !it->flAlpha )
			{
				path.erase( it );
				continue;
			}

			it->flAlpha -= ( iInitialPathSize * flFadeSpeed / 255 ) / ( i + 6 );
			if ( it->flAlpha < 0 ) it->flAlpha = 0.f;
		}
	}
	std::deque<PathPoint> path;
	float	flThrowTime;
	float	flDuartion;
	float	flFadeSpeed;
	size_t	iInitialPathSize;
	bool	bThrown;
	float	step;
};

class CGrenadePred
{
public:
	void	Paint();
	void	Simulate();
private:
	void	Setup( Vector& vecSrc, Vector& vecThrow, const QAngle& angEyeAngles );
	int		Step( Vector& vecSrc, Vector& vecThrow, int tick, float interval );
	bool	CheckDetonate( const Vector& vecThrow, const trace_t& tr, int tick, float interval );
	void	TraceHull( const Vector& src, const Vector& end, trace_t& tr );
	void	AddGravityMove( Vector& move, Vector& vel, float frametime, bool onground );
	void	PushEntity( Vector& src, const Vector& move, trace_t& tr );
	void	ResolveFlyCollisionCustom( trace_t& tr, Vector& vecVelocity, float interval );
	int		PhysicsClipVelocity( const Vector& in, const Vector& normal, Vector& out, float overbounce );
	bool	Shoulddraw( GrenadePath& gp, float prevthrowtime );

	C_BasePlayer* m_pLocalPlayer;
	std::deque<GrenadePath> deques;
	float	duration = 6.f;
	float	speed = 100.f;
	int		maxdeques = 5;
};