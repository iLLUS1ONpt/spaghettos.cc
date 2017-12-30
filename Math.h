#pragma once
class CMath
{
public:
	inline void SinCos( float radians, float *sine, float *cosine );
	void VectorTransform( Vector& in1, matrix3x4a_t& in2, Vector &out );
	void VectorTransform( Vector & in1, matrix3x4_t & in2, Vector & out );
	void VectorAngles( const Vector& forward, QAngle &angles );
	void AngleVectors( const QAngle& angles, Vector *forward );
	void AngleVectors( const QAngle &angles, Vector *forward, Vector *right, Vector *up );
	float GetDelta( float hspeed, float maxspeed, float airaccelerate );
	float VectorDistance( Vector v1, Vector v2 );
	float GetFov( const QAngle& viewAngle, const QAngle& aimAngle );
	float GetFovB( Vector vLocalOrigin, Vector vPosition, Vector vForward );
	Vector ExtrapolateTick( Vector p0, Vector v0 );
	QAngle CalcAngle( Vector v1, Vector v2 );
	Vector AnglesToPixels( QAngle SourceAngles, QAngle FinalAngles, float sensitivity );
	FORCEINLINE float DotProduct( const Vector& a, const Vector& b )
	{
		return ( a.x * b.x + a.y * b.y + a.z * b.z );
	}
	FORCEINLINE void VectorMAInline( const float* start, float scale, const float* direction, float* dest )
	{
		dest[ 0 ] = start[ 0 ] + direction[ 0 ] * scale;
		dest[ 1 ] = start[ 1 ] + direction[ 1 ] * scale;
		dest[ 2 ] = start[ 2 ] + direction[ 2 ] * scale;
	}

	FORCEINLINE void VectorMAInline( const Vector& start, float scale, const Vector& direction, Vector& dest )
	{
		dest.x = start.x + direction.x*scale;
		dest.y = start.y + direction.y*scale;
		dest.z = start.z + direction.z*scale;
	}

	FORCEINLINE void VectorMA( const Vector& start, float scale, const Vector& direction, Vector& dest )
	{
		VectorMAInline( start, scale, direction, dest );
	}

	FORCEINLINE void VectorMA( const float * start, float scale, const float *direction, float *dest )
	{
		VectorMAInline( start, scale, direction, dest );
	}

	
};

double inline __declspec ( naked ) __fastcall FASTSQRT( double n )
{
	_asm fld qword ptr[ esp + 4 ]
		_asm fsqrt
	_asm ret 8
}

extern CMath* Math;