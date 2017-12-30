#pragma once

extern WNDPROC oWndProc;

using FrameStageNotifyFn = void( __stdcall* )( ClientFrameStage_t );
extern FrameStageNotifyFn oFrameStageNotify;

using EndSceneFn = long( __stdcall* )( IDirect3DDevice9* );
extern EndSceneFn oEndScene;

using ResetFn = long( __stdcall* )( IDirect3DDevice9*, D3DPRESENT_PARAMETERS* );
extern ResetFn oReset;

using SvCheatsGetBoolFn = bool( __thiscall * )( void* );
extern SvCheatsGetBoolFn oSvCheatsGetBool;

using PaintTraverseFn = void( __thiscall* )( void*, unsigned int, bool, bool );
extern PaintTraverseFn oPaintTraverse;

using CreateMoveFn = bool( __stdcall* )( float, CUserCmd* );
extern CreateMoveFn oCreateMove;

using DrawModelExecuteFn = void( __thiscall* )( IVModelRender*, IMatRenderContext*, const DrawModelState_t&, const ModelRenderInfo_t&, matrix3x4_t* );
extern DrawModelExecuteFn oDrawModelExecute;

using OverrideConfigFn = bool( __thiscall* )( IMaterialSystem*, MaterialSystem_Config_t*, bool );
extern OverrideConfigFn oOverrideConfig;

using BeginFrameFn = void( __thiscall* )( void* );
extern BeginFrameFn oBeginFrame;

using SceneEndFn = void( __thiscall* )( void* );
extern SceneEndFn oSceneEnd;

using RenderViewFn = void( __thiscall* )( void*, CViewSetup&, CViewSetup&, unsigned int, int );
extern RenderViewFn oRenderView;

using TraceRayFn = void( __thiscall* )( void*, const Ray_t&, unsigned int, ITraceFilter*, trace_t* );
extern TraceRayFn oTraceRay;

using IsBoxVisibleFn = int( __stdcall* )( const Vector&, const Vector& );
extern IsBoxVisibleFn oIsBoxVisible;

extern LRESULT __stdcall g_hkWndProc( HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

extern void __stdcall g_hkFrameStageNotify( ClientFrameStage_t stage );
extern long __stdcall g_hkEndScene( IDirect3DDevice9* pDevice );
extern long __stdcall g_hkReset( IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters );
extern bool __fastcall g_hkSvCheatsGetBool( void* pConVar, void* edx );
extern void __fastcall g_hkPaintTraverse( void* pPanels, int edx, unsigned int panel, bool forceRepaint, bool allowForce );
extern bool __stdcall g_hkCreateMove( float flInputSampleTime, CUserCmd* cmd );
extern void __stdcall  g_hkDrawModelExecute( IMatRenderContext* context, const DrawModelState_t& state, const ModelRenderInfo_t& info, matrix3x4_t* pCustomBoneToWorld );
extern bool __fastcall g_hkOverrideConfig( IMaterialSystem* this0, int edx, MaterialSystem_Config_t* config, bool forceUpdate );
extern void __fastcall g_hkBeginFrame( void* thisptr );
extern void __fastcall g_hkSceneEnd( void* ecx, void* edx );
extern void __fastcall g_hkRenderView( void* thisptr, void*, CViewSetup& setup, CViewSetup& hudViewSetup, unsigned int nClearFlags, int whatToDraw );
extern void __fastcall g_hkTraceRay( void* thisptr, void*, const Ray_t& ray, unsigned int fMask, ITraceFilter* pTraceFilter, trace_t* pTrace );
extern int	__stdcall g_hkIsBoxVisible( const Vector& mins, const Vector& maxs );


//Netvar
extern void __cdecl g_hkSequence( const CRecvProxyData* proxy_data_const, void* entity, void* output );

