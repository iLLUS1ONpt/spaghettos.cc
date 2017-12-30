#include "..\MainInclude.h"
#include "..\Math.h"
#include "..\Globals.h"



void CHook::SetupHooks()
{
	g_phkVPanel = new VMTHook( reinterpret_cast< DWORD** > ( g_pVPanel ) );
	g_phkClient = new VMTHook( reinterpret_cast< DWORD** > ( g_pClient ) );
	g_phkClientMode = new VMTHook( reinterpret_cast< DWORD** > ( g_pClientMode ) );
	g_phkD3D9 = new VMTHook( reinterpret_cast< DWORD** > ( Offsets->d3d9Device ) );
	g_phkVModelRender = new VMTHook( reinterpret_cast< DWORD** > ( g_pVModelRender ) );
	g_phkMaterialSystem = new VMTHook( reinterpret_cast< DWORD** > ( g_pMaterialSystem ) );
	g_phkStudioRender = new VMTHook( reinterpret_cast< DWORD** > ( g_pStudioRender ) );
	g_phkVRenderView = new VMTHook( reinterpret_cast< DWORD** > ( g_pVRenderView ) );
	g_phkViewRender = new VMTHook( reinterpret_cast< DWORD** > ( g_pViewRender ) );
	g_phkEngineTrace = new VMTHook( reinterpret_cast< DWORD** > ( g_pEngineTrace ) );
	g_phkEngine = new VMTHook( reinterpret_cast< DWORD** > ( g_pEngine ) );

	ConVar* sv_cheats_cvar = g_pCvar->FindVar( "sv_cheats" );
	g_phkSvCheats = new VMTHook( reinterpret_cast< DWORD** > ( sv_cheats_cvar ) );

	RecvProp* sequence_prop = C_BaseViewModel::GetSequenceProp();
	g_phkSequence = new RecvPropHook( sequence_prop, g_hkSequence );

	
	oReset = ( ResetFn ) Hook->g_phkD3D9->HookFunction( reinterpret_cast< uintptr_t >( g_hkReset ), 16 );
	oEndScene = ( EndSceneFn ) Hook->g_phkD3D9->HookFunction( reinterpret_cast< uintptr_t >( g_hkEndScene ), 42 );
	oFrameStageNotify = ( FrameStageNotifyFn ) Hook->g_phkClient->HookFunction( reinterpret_cast< uintptr_t >( g_hkFrameStageNotify ), 36 );
	oSvCheatsGetBool = ( SvCheatsGetBoolFn ) Hook->g_phkSvCheats->HookFunction( reinterpret_cast< uintptr_t >( g_hkSvCheatsGetBool ), 13 );
	oPaintTraverse = ( PaintTraverseFn ) Hook->g_phkVPanel->HookFunction( reinterpret_cast< uintptr_t >( g_hkPaintTraverse ), 41 );
	oCreateMove = ( CreateMoveFn ) Hook->g_phkClientMode->HookFunction( reinterpret_cast< uintptr_t >( g_hkCreateMove ), 24 );
	oDrawModelExecute = ( DrawModelExecuteFn ) g_phkVModelRender->HookFunction( reinterpret_cast< uintptr_t >( g_hkDrawModelExecute ), 21 );
	oOverrideConfig = ( OverrideConfigFn ) g_phkMaterialSystem->HookFunction( reinterpret_cast< uintptr_t >( g_hkOverrideConfig ), 21 );
	oBeginFrame = ( BeginFrameFn ) g_phkStudioRender->HookFunction( reinterpret_cast< uintptr_t >( g_hkBeginFrame ), 9 );
	oSceneEnd = ( SceneEndFn ) g_phkVRenderView->HookFunction( reinterpret_cast< uintptr_t >( g_hkSceneEnd ), 9 );
	oRenderView = ( RenderViewFn ) g_phkViewRender->HookFunction( reinterpret_cast< uintptr_t >( g_hkRenderView ), 6 );
	oTraceRay = ( TraceRayFn ) Hook->g_phkEngineTrace->HookFunction( reinterpret_cast< uintptr_t >( g_hkTraceRay ), 5 );
	oIsBoxVisible = ( IsBoxVisibleFn ) Hook->g_phkEngine->HookFunction( reinterpret_cast< uintptr_t >( g_hkIsBoxVisible ), 32 );
}

void CHook::ResetHooks()
{
	FreeConsole();

	ImGui_ImplDX9_Shutdown();
	SetWindowLongPtr( G::Window, GWL_WNDPROC, ( LONG_PTR ) oWndProc );

	F::Visuals->RemoveListener();

	g_phkVPanel->UnHook();
	g_phkClient->UnHook();
	g_phkClientMode->UnHook();
	g_phkD3D9->UnHook();
	g_phkVModelRender->UnHook();
	g_phkMaterialSystem->UnHook();
	g_phkStudioRender->UnHook();
	g_phkVRenderView->UnHook();
	g_phkViewRender->UnHook();
	g_phkEngineTrace->UnHook();

	g_phkSequence->~RecvPropHook();
	delete g_phkSequence;

	g_phkSvCheats->UnHook();
	delete g_phkSvCheats;

	delete g_phkVPanel;
	delete g_phkClient;
	delete g_phkClientMode;
	delete g_phkD3D9;
	delete g_phkVModelRender;
	delete g_phkMaterialSystem;
	delete g_phkStudioRender;
	delete g_phkVRenderView;
	delete g_phkViewRender;
	delete g_phkEngineTrace;

	delete F::Misc;
	delete F::SkinChanger;
	delete F::Thirdperson;
	delete F::Visuals;
	delete F::Chams;
	delete F::MaterialConfig;
	delete F::Dlight;
	delete F::Prediction;
	delete F::Aimbot;
	delete F::AntiAim;
	delete F::BulletPenetration;
	delete F::GrenadePrediction;
	delete F::Animations;
	delete F::HvH;
}

CHook* Hook = new CHook;


