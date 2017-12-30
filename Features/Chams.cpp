#include "..\CheatInclude.h"
#include <intrin.h>

void CChams::Run()
{
	NightMode();
	AsusWalls();

	if ( !Vars.Visuals.Chams.bEnabled )
		return;

	m_pLocalPlayer = reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( g_pEngine->GetLocalPlayer() ) );
	if ( !m_pLocalPlayer || !g_pEngine->IsInGame() )
		return;

	for ( int i = 1; i <= g_pGlobalVarsBase->maxClients; i++ )
	{
		auto pPlayer = reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( i ) );

		if ( !pPlayer || pPlayer == m_pLocalPlayer )
			continue;

		float flDist = pPlayer->GetAbsOrigin().Dist( m_pLocalPlayer->GetAbsOrigin() );
		Entities.push_back( std::pair<C_BasePlayer*, float>( pPlayer, flDist ) );
	}

	std::sort( Entities.begin(), Entities.end(), [] ( const std::pair<C_BasePlayer*, float> &left, const std::pair<C_BasePlayer*, float> &right ) { return left.second > right.second; } );

	DrawPlayers();

	if ( Entities.size() > 0 )
		Entities.clear();

	DrawAngles();

	g_pVModelRender->ForcedMaterialOverride( NULL );
}

bool CChams::Run( const ModelRenderInfo_t & info )
{
	if ( !Vars.Visuals.Chams.bEnabled )
		return true;

	m_pLocalPlayer = reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( g_pEngine->GetLocalPlayer() ) );
	if ( !m_pLocalPlayer || !g_pEngine->IsInGame() || !info.pModel )
		return true;

	std::string modelName = g_pVModelInfo->GetModelName( info.pModel );

	if ( info.entity_index == g_pEngine->GetLocalPlayer() && g_pInput->m_fCameraInThirdPerson && m_pLocalPlayer->IsScoped() )
		g_pVRenderView->SetBlend( 0.8f );

	if ( Vars.Visuals.Chams.bPlayers && modelName.find( "models/player" ) != std::string::npos )
	{
		if ( G::InChams )
			return true;
		else if ( info.entity_index != g_pEngine->GetLocalPlayer() )
			return false;
	}

	else if ( Vars.Visuals.Chams.bHands && modelName.find( "arms" ) != std::string::npos )
	{
		auto Hands = g_pMaterialSystem->FindMaterial( modelName.c_str(), charenc( TEXTURE_GROUP_MODEL ) );

		switch ( Vars.Visuals.Chams.iHandMaterial )
		{
			case CMAT_GOLD:
				Hands = M::gold;
				break;
			case CMAT_PLATINUM:
				Hands = M::platinum;
				break;
			case CMAT_GLASS:
				Hands = M::glass;
				break;
			case CMAT_CRYSTAL:
				Hands = M::crystal;
				break;
			case CMAT_CHROME:
				Hands = M::chrome;
				break;
			case CMAT_GLOW:
				Hands = M::glow;
				break;
			case CMAT_GLOSS:
				Hands = M::gloss;
				break;
			default: break;
		}
		Hands->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW, Vars.Visuals.Chams.iHandMaterial == CMAT_NODRAW );
		Hands->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, Vars.Visuals.Chams.iHandMaterial == CMAT_WIREFRAME );
		g_pVModelRender->ForcedMaterialOverride( Hands );
	}

	else if ( Vars.Visuals.Chams.bWeapon && modelName.find( "models/weapons/v" ) != std::string::npos && modelName.find( "arms" ) == std::string::npos )
	{
		auto Weapon = g_pMaterialSystem->FindMaterial( modelName.c_str(), charenc( TEXTURE_GROUP_MODEL ) );

		switch ( Vars.Visuals.Chams.iWeaponMaterial )
		{
			case CMAT_GOLD:
				Weapon = M::gold;
				break;
			case CMAT_PLATINUM:
				Weapon = M::platinum;
				break;
			case CMAT_GLASS:
				Weapon = M::glass;
				break;
			case CMAT_CRYSTAL:
				Weapon = M::crystal;
				break;
			case CMAT_CHROME:
				Weapon = M::chrome;
				break;
			case CMAT_GLOW:
				Weapon = M::glow;
				break;
			case CMAT_GLOSS:
				Weapon = M::gloss;
				break;
			default: break;
		}
		Weapon->SetMaterialVarFlag( MATERIAL_VAR_NO_DRAW, Vars.Visuals.Chams.iWeaponMaterial == CMAT_NODRAW );
		Weapon->SetMaterialVarFlag( MATERIAL_VAR_WIREFRAME, Vars.Visuals.Chams.iWeaponMaterial == CMAT_WIREFRAME );
		g_pVModelRender->ForcedMaterialOverride( Weapon );
	}
	return true;
}

void CChams::DrawPlayers()
{
	if ( !Vars.Visuals.Chams.bPlayers || !g_pEngine->IsInGame() )
		return;

	for ( int i = 0; i < Entities.size(); i++ )
	{
		auto pPlayer = Entities[ i ].first;

		if ( !g_pInput->m_fCameraInThirdPerson && pPlayer == m_pLocalPlayer )
			continue;

		if ( !pPlayer || !pPlayer->IsAlive() || pPlayer->IsDormant() )
			continue;

		Color render_color_hidden = !pPlayer->IsEnemy() && Vars.Visuals.bTeamCheck ? Color( Vars.Visuals.Chams.Colors.flTeamHidden ) : Color( Vars.Visuals.Chams.Colors.flEnemyHidden );
		Color render_color_visible = !pPlayer->IsEnemy() && Vars.Visuals.bTeamCheck ? Color( Vars.Visuals.Chams.Colors.flTeamVisible ) : Color( Vars.Visuals.Chams.Colors.flEnemyVisible );

		if ( pPlayer != m_pLocalPlayer && m_pLocalPlayer->IsAlive() )
		{
			std::deque< LagRecord > &track = F::Backtrack->m_PlayerTrack[ pPlayer->GetIClientEntity()->EntIndex() - 1 ];
			if ( track.size() < 1 )
				continue;

			Vector absorg = pPlayer->GetOrigin();
			F::Backtrack->BackupPlayer( pPlayer );
			for ( int i = track.size(); i > 0; i-- )
			{
				if ( F::Backtrack->StartLagCompensation( pPlayer, NULL, i - 1 ) )
				{
					if ( fabsf( track.at( i - 1 ).m_vecOrigin.Length() - absorg.Length() ) < 3.f )
						break;

					ForceMaterial( M::hidden, Color( 50, 50, 50, 50 ) );
					G::InChams = true;
					pPlayer->DrawModel( STUDIO_RENDER, 50 );
					G::InChams = false;
					break;
				}
			}
			F::Backtrack->RestorePlayer( pPlayer );
		}

		if ( Vars.Visuals.Chams.bOutlined )
		{
			ForceMaterial( Vars.Visuals.Chams.bXQZ ? M::wireframed_hidden : M::wireframed, Color::White() );
			G::InChams = true;
			pPlayer->DrawModel( STUDIO_WIREFRAME, 255 );
			G::InChams = false;
		}

		if ( Vars.Visuals.Chams.bXQZ && pPlayer != m_pLocalPlayer )
		{
			ForceMaterial( M::hidden, render_color_hidden );
			G::InChams = true;
			pPlayer->DrawModel( STUDIO_RENDER, 255 );
			G::InChams = false;
		}

		M::visible->SetMaterialVarFlag( MATERIAL_VAR_IGNOREZ, false );
		ForceMaterial( M::visible, render_color_visible );
		G::InChams = true;
		pPlayer->DrawModel( STUDIO_RENDER, 255 );
		G::InChams = false;
	}
}

void CChams::DrawAngles()
{
	static auto Material = M::visible;

	if (Material && Vars.HvH.bEnabled)
	{
		QAngle OrigAng;
		Material->AlphaModulate(0.5f);
		m_pLocalPlayer->SetAngle2(QAngle(0, m_pLocalPlayer->GetEyeAngles().y, 0));
		g_pVRenderView->SetColorModulation(Color(Vars.Visuals.Chams.Colors.flTeamVisible).Base());
		g_pVModelRender->ForcedMaterialOverride(Material);
		m_pLocalPlayer->DrawModel(STUDIO_RENDER, 255);
		Material->AlphaModulate(1.f);
		m_pLocalPlayer->SetAngle2(OrigAng);
	}
}

void CChams::NightMode()
{
	if (!g_pEngine->IsInGame() || !g_pEngine->IsConnected() || !g_pEngine->GetLocalPlayer())
		return;

	static bool bPerformed = true;
	static bool bLastSetting;

	if (!bPerformed)
	{
		for (auto i = g_pMaterialSystem->FirstMaterial(); i != g_pMaterialSystem->InvalidMaterial(); i = g_pMaterialSystem->NextMaterial(i))
		{
			auto pMat = g_pMaterialSystem->GetMaterial(i);

			if (!pMat || pMat->IsErrorMaterial())
				continue;

			if (strstr(pMat->GetTextureGroupName(), "World") || strstr(pMat->GetTextureGroupName(), "StaticProp"))
			{
				static auto r_DrawSpecificStaticProp = g_pCvar->FindVar("r_DrawSpecificStaticProp");

				std::string modelName = pMat->GetName();

				if (bLastSetting)
				{
					r_DrawSpecificStaticProp->SetValue(0);
					pMat->ColorModulate(0.25f, 0.25f, 0.25f);
				}
				else
				{
					r_DrawSpecificStaticProp->SetValue(1);
					pMat->ColorModulate(1.f, 1.f, 1.f);
					pMat->AlphaModulate(1.f);
				}
			}
		}
		bPerformed = true;
	}
	if (bLastSetting != Vars.Visuals.Chams.bDarkMode && !G::unload)
	{
		bLastSetting = Vars.Visuals.Chams.bDarkMode;
		bPerformed = false;
	}
}

void CChams::AsusWalls()
{
	static auto r_drawspecific = g_pCvar->FindVar("r_DrawSpecificStaticProp");
	static auto r_DrawSpecificStaticProp = g_pCvar->FindVar("r_DrawSpecificStaticProp");
	r_DrawSpecificStaticProp->SetValue(0);

	static bool performed = true;
	static bool last_setting;

	if (!g_pEngine->IsInGame() || !g_pEngine->IsConnected() || !g_pEngine->GetLocalPlayer())
		return;

	if (!performed)
	{
		float blend = Vars.Visuals.bAsusBlend / 100.0f;

		for (auto i = g_pMaterialSystem->FirstMaterial(); i != g_pMaterialSystem->InvalidMaterial(); i = g_pMaterialSystem->NextMaterial(i))
		{
			IMaterial* pMaterial = g_pMaterialSystem->GetMaterial(i);

			if (!pMaterial)
				return;

			if (strstr(pMaterial->GetTextureGroupName(), "World textures") || (strstr(pMaterial->GetTextureGroupName(), "StaticProp textures")))
			{
				if (last_setting)
				{
					pMaterial->AlphaModulate(blend);
				}
				else
				{
					pMaterial->AlphaModulate(1.0f);
				}
			}
		}
		performed = true;
	}
	if (last_setting != Vars.Visuals.bAsusWalls)
	{
		last_setting = Vars.Visuals.bAsusWalls;
		performed = false;
	}
}

CChams::CChams()
{
	std::ofstream( "csgo\\materials\\cstm_visible.vmt" ) << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
  "$wireframe"    "0"
}
)#";
	std::ofstream( "csgo\\materials\\cstm_hidden.vmt" ) << R"#("VertexLitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
  "$wireframe"    "0"
}
)#";
	std::ofstream( "csgo\\materials\\cstm_visible_wireframed.vmt" ) << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "0"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
  "$wireframe"    "1"
}
)#";
	std::ofstream( "csgo\\materials\\cstm_hidden_wireframed.vmt" ) << R"#("UnlitGeneric"
{
  "$basetexture" "vgui/white_additive"
  "$ignorez"      "1"
  "$envmap"       ""
  "$nofog"        "1"
  "$model"        "1"
  "$nocull"       "0"
  "$selfillum"    "1"
  "$halflambert"  "1"
  "$znearer"      "0"
  "$flat"         "1"
  "$wireframe"    "1"
}
)#";
}

CChams::~CChams()
{
	std::remove( "csgo\\materials\\cstm_visible.vmt" );
	std::remove( "csgo\\materials\\cstm_hidden.vmt" );
	std::remove( "csgo\\materials\\cstm_visible_wireframed.vmt" );
	std::remove( "csgo\\materials\\cstm_hidden_wireframed.vmt" );
}

void CChams::ForceMaterial( IMaterial * material, Color color )
{
	if ( material != NULL )
	{
		g_pVRenderView->SetColorModulation( color.Base() );
		g_pVModelRender->ForcedMaterialOverride( material );
	}
}