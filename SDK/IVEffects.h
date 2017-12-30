#pragma once

struct ColorRGBExp32
{
	byte r, g, b;
	signed char exponent;
};

struct dlight_t
{
	int		flags;
	Vector	origin;
	float	radius;
	ColorRGBExp32	color;
	float	die;
	float	decay;
	float	minlight;
	int		key;
	int		style;
	Vector	m_Direction;
	float	m_InnerAngle;
	float	m_OuterAngle;
	float GetRadius() const
	{
		return radius;
	}
	float GetRadiusSquared() const
	{
		return radius * radius;
	}
	float IsRadiusGreaterThanZero() const
	{
		return radius > 0.0f;
	}
};

class IVEffects
{
public:
	dlight_t*		CL_AllocDlight( int key );
	dlight_t*		CL_AllocElight( int key );
	dlight_t*		GetElightByKey( int key );
};