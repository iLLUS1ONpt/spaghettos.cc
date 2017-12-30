#include "..\MainInclude.h"

uintptr_t U::FindSig( std::string moduleName, std::string pattern )
{
	const char* daPattern = pattern.c_str();
	uintptr_t firstMatch = 0;
	uintptr_t moduleBase = ( uintptr_t ) GetModuleHandleA( moduleName.c_str() );
	MODULEINFO miModInfo; GetModuleInformation( GetCurrentProcess(), ( HMODULE ) moduleBase, &miModInfo, sizeof( MODULEINFO ) );
	uintptr_t moduleEnd = moduleBase + miModInfo.SizeOfImage;
	for ( uintptr_t pCur = moduleBase; pCur < moduleEnd; pCur++ )
	{
		if ( !*daPattern )
			return firstMatch;

		if ( *( PBYTE ) daPattern == '\?' || *( BYTE* ) pCur == getByte( daPattern ) )
		{
			if ( !firstMatch )
				firstMatch = pCur;

			if ( !daPattern[ 2 ] )
				return firstMatch;

			if ( *( PWORD ) daPattern == '\?\?' || *( PBYTE ) daPattern != '\?' )
				daPattern += 3;

			else
				daPattern += 2;
		}
		else
		{
			daPattern = pattern.c_str();
			firstMatch = 0;
		}
	}
	return 0;
}

void SetupInterfaces()
{
	g_pClient = U::GetInterface<IBaseClientDll>( strenc( "Client.dll" ), strenc( "VClient018" ) );
	g_pClientEntList = U::GetInterface<IClientEntityList>( strenc( "Client.dll" ), strenc( "VClientEntityList003" ) );
	g_pCvar = U::GetInterface<ICVar>( strenc( "vstdlib.dll" ), strenc( "VEngineCvar007" ) );
	g_pClientMode = **reinterpret_cast< IClientModeShared*** > ( ( *reinterpret_cast< uintptr_t** > ( g_pClient ) )[ 10 ] + 0x5 );
	g_pEngine = U::GetInterface<IEngineClient>( strenc( "engine.dll" ), strenc( "VEngineClient014" ) );
	g_pEngineTrace = U::GetInterface<IEngineTrace>( strenc( "engine.dll" ), strenc( "EngineTraceClient004" ) );
	g_pClientState = **reinterpret_cast< CBaseClientState*** > ( ( *reinterpret_cast< uintptr_t** > ( g_pEngine ) )[ 12 ] + 0x10 );
	g_pInput = *reinterpret_cast< IInput** > ( ( *reinterpret_cast< uintptr_t** >( g_pClient ) )[ 15 ] + 0x1 );
	g_pInputSystem = U::GetInterface<IInputSystem>( strenc( "inputsystem.dll" ), strenc( "InputSystemVersion001" ) );
	g_pGameEventManager = U::GetInterface<IGameEventManager2>( strenc( "engine.dll" ), strenc( "GAMEEVENTSMANAGER002" ) );
	g_pVModelInfo = U::GetInterface<IVModelInfo>( strenc( "engine.dll" ), strenc( "VModelInfoClient004" ) );
	g_pLocalize = U::GetInterface<ILocalize>( strenc( "localize.dll" ), strenc( "Localize_001" ) );
	g_pGlobalVarsBase = **reinterpret_cast< IGlobalVarsBase*** > ( ( *reinterpret_cast< uintptr_t** > ( g_pClient ) )[ 0 ] + 0x1B );
	g_pSurface = U::GetInterface<ISurface>( strenc( "vguimatsurface.dll" ), strenc( "VGUI_Surface031" ) );
	g_pVPanel = U::GetInterface<IVPanel>( strenc( "vgui2.dll" ), strenc( "VGUI_Panel009" ) );
	g_pVModelRender = U::GetInterface<IVModelRender>( strenc( "engine.dll" ), strenc( "VEngineModel016" ) );
	g_pVRenderView = U::GetInterface<IVRenderView>( strenc( "engine.dll" ), strenc( "VEngineRenderView014" ) );
	g_pMaterialSystem = U::GetInterface<IMaterialSystem>( strenc( "materialsystem.dll" ), strenc( "VMaterialSystem080" ) );
	g_pVEffects = U::GetInterface<IVEffects>( strenc( "engine.dll" ), strenc( "VEngineEffects001" ) );
	g_pVDebugOverlay = U::GetInterface<IVDebugOverlay>( strenc( "engine.dll" ), strenc( "VDebugOverlay004" ) );
	g_pStudioRender = U::GetInterface<IStudioRender>( strenc( "studiorender.dll" ), strenc( "VStudioRender026" ) );
	g_pGameMovement = U::GetInterface<IGameMovement>( strenc( "client.dll" ), strenc( "GameMovement001" ) );
	g_pPrediction = U::GetInterface<IPrediction>( strenc( "client.dll" ), strenc( "VClientPrediction001" ) );
	g_pMoveHelper = **reinterpret_cast< IMoveHelper*** > ( U::FindSig( strenc( "client.dll" ), strenc( "8B 0D ? ? ? ? 8B 46 08 68" ) ) + 0x2 );
	g_pPhysicsSurfaceProps = U::GetInterface<IPhysicsSurfaceProps>( strenc( "vphysics.dll" ), strenc( "VPhysicsSurfaceProps001" ) );
	g_pViewRender = **reinterpret_cast< IViewRender*** > ( U::FindSig( strenc( "client.dll" ), strenc( "FF 50 14 E8 ?? ?? ?? ?? 5D" ) ) - 0x7 );
	g_pViewRenderBeams = *reinterpret_cast< IViewRenderBeams** > ( U::FindSig( strenc( "client.dll" ), strenc( "A1 ? ? ? ? 56 8B F1 B9 ? ? ? ? FF 50 08" ) ) + 0x1 );
	g_pMemAlloc = *reinterpret_cast< IMemAlloc** >( GetProcAddress( GetModuleHandleA( charenc( "tier0.dll" ) ), charenc( "g_pMemAlloc" ) ) );
	G::GameRules = *reinterpret_cast<C_CSGameRulesProxy***>( U::FindSig( charenc( "client.dll" ), charenc( "A1 ? ? ? ? 85 C0 0F 84 ? ? ? ? 80 B8 ? ? ? ? ? 0F 84 ? ? ? ? 0F 10 05" ) ) + 0x1 );
}

void SetupMaterials()
{
	M::visible = g_pMaterialSystem->FindMaterial( "cstm_visible", TEXTURE_GROUP_MODEL );
	M::hidden = g_pMaterialSystem->FindMaterial( "cstm_hidden", TEXTURE_GROUP_MODEL );
	M::wireframed = g_pMaterialSystem->FindMaterial( "cstm_visible_wireframed", TEXTURE_GROUP_MODEL );
	M::wireframed_hidden = g_pMaterialSystem->FindMaterial( " cstm_hidden_wireframed", TEXTURE_GROUP_MODEL );
	M::gold = g_pMaterialSystem->FindMaterial( "models/inventory_items/trophy_majors/gold", TEXTURE_GROUP_OTHER );
	M::platinum = g_pMaterialSystem->FindMaterial( "models/player/ct_fbi/ct_fbi_glass", TEXTURE_GROUP_OTHER );
	M::glass = g_pMaterialSystem->FindMaterial( "models/inventory_items/cologne_prediction/cologne_prediction_glass", TEXTURE_GROUP_OTHER );
	M::crystal = g_pMaterialSystem->FindMaterial( "models/inventory_items/trophy_majors/crystal_clear", TEXTURE_GROUP_OTHER );
	M::chrome = g_pMaterialSystem->FindMaterial( "models/gibs/glass/glass", TEXTURE_GROUP_OTHER );
	M::glow = g_pMaterialSystem->FindMaterial( "vgui/achievements/glow", TEXTURE_GROUP_OTHER );
	M::gloss = g_pMaterialSystem->FindMaterial( "models/inventory_items/trophy_majors/gloss", TEXTURE_GROUP_OTHER );
}

void U::Startup()
{
	while ( !( G::Window = FindWindowA( charenc( "Valve001" ), NULL ) ) )
		Sleep( 200 );

	oWndProc = ( WNDPROC ) SetWindowLongPtr( G::Window, GWL_WNDPROC, ( LONG_PTR ) g_hkWndProc );

	while ( !GetModuleHandleA( "serverbrowser.dll" ) )
		Sleep( 100 );

	SetupInterfaces();
	SetupMaterials();
	D::SetupFonts();
	Offsets->GrabOffsets();
	Hook->SetupHooks();
	InitializeKits();
	F::Visuals->SetupListener();
	F::SkinChanger->Load();
	Config->LoadPreset();
}

bool U::IsVisible( const Vector & start, const Vector & end, C_BasePlayer * pPlayer, C_BasePlayer * pLocalPlayer )
{
	Ray_t ray;
	trace_t tr;
	ray.Init( start, end );

	CTraceFilter filter;
	filter.pSkip = pLocalPlayer;

	g_pEngineTrace->TraceRay( ray, MASK_SHOT, &filter, &tr );

	if ( tr.m_pEnt == pPlayer || tr.fraction > 0.97f )
	{
		return true;
	}
	return false;
}