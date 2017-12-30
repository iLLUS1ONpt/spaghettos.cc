#include "..\CheatInclude.h"

PaintTraverseFn oPaintTraverse;
void __fastcall g_hkPaintTraverse( void* pPanels, int edx, unsigned int panel, bool forceRepaint, bool allowForce )
{
	if ( G::unload )
		return oPaintTraverse( pPanels, panel, forceRepaint, allowForce );

	static unsigned int scopePanel;
	if ( !scopePanel )
	{
		const char* panelname = g_pVPanel->GetName( panel );

		if ( !strcmp( panelname, "HudZoom" ) )
			scopePanel = panel;
	}

	if ( panel == scopePanel && Vars.Visuals.bSniperCrosshair )
		return;

	oPaintTraverse( pPanels, panel, forceRepaint, allowForce );

	static unsigned int drawPanel;
	if ( !drawPanel )
	{
		const char* panelname = g_pVPanel->GetName( panel );

		if ( !strcmp( panelname, "MatSystemTopPanel" ) )
			drawPanel = panel;
	}

	if ( panel != drawPanel )
		return;

	F::Visuals->Run();

	F::Dlight->Run();
}