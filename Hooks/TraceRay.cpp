#include "..\CheatInclude.h"

TraceRayFn oTraceRay;

void __fastcall  g_hkTraceRay( void *thisptr, void*, const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace )
{
	if ( !G::InAutowall )
		return oTraceRay( thisptr, ray, fMask, pTraceFilter, pTrace );

	CTraceFilterSkipTwoEntities filter;
	filter.pPassEntity1 = reinterpret_cast< void* >( g_pClientEntList->GetClientEntity( g_pEngine->GetLocalPlayer() ) );
	filter.pPassEntity1 = reinterpret_cast< void* >( reinterpret_cast< C_BasePlayer* >( filter.pPassEntity1 )->GetActiveWeapon() );

	oTraceRay( thisptr, ray, fMask, &filter, pTrace );

	pTrace->surface.flags |= SURF_SKY;
}