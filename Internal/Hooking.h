#pragma once

class CHook
{
public:

	VMTHook* g_phkVPanel;
	VMTHook* g_phkClient;
	VMTHook* g_phkClientMode;
	VMTHook* g_phkD3D9;
	VMTHook* g_phkVModelRender;
	VMTHook* g_phkMaterialSystem;
	VMTHook* g_phkStudioRender;
	VMTHook* g_phkVRenderView;
	VMTHook* g_phkViewRender;
	VMTHook* g_phkEngineTrace;
	VMTHook* g_phkEngine;

	VMTHook* g_phkSvCheats;

	RecvPropHook* g_phkSequence;
	void SetupHooks();
	void ResetHooks();
};

extern CHook* Hook;