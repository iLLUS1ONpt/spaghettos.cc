#include "..\CheatInclude.h"

bool CGameTrace::DidHitNonWorldEntity() const
{
	return m_pEnt != NULL && m_pEnt == g_pClientEntList->GetClientEntity( 0 );
}

CGameTrace::CGameTrace( const CGameTrace & vOther )
{
	fractionleftsolid = vOther.fractionleftsolid;
	surface = vOther.surface;
	hitgroup = vOther.hitgroup;
	physicsbone = vOther.physicsbone;
	worldSurfaceIndex = vOther.worldSurfaceIndex;
	m_pEnt = vOther.m_pEnt;
	hitbox = vOther.hitbox;
	startpos = vOther.startpos;
	endpos = vOther.endpos;
	plane = vOther.plane;
	fraction = vOther.fraction;
	contents = vOther.contents;
	dispFlags = vOther.dispFlags;
	allsolid = vOther.allsolid;
	startsolid = vOther.startsolid;
}
