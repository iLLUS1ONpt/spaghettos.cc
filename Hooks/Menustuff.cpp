#include "..\CheatInclude.h"
#include "..\Menu\Menustuff.h"
#include <functional>
#include <intrin.h>

#define dum() {ImGui::Dummy( ImVec2( 10, 1 ) ); ImGui::SameLine();}

void GUI_Init( IDirect3DDevice9* pDevice );

namespace ImGui
{
	// ImGui ListBox lambda binder
	static bool ListBox( const char* label, int* current_item, std::function<const char*( int )> lambda, int items_count, int height_in_items )
	{
		return ImGui::ListBox( label, current_item, [] ( void* data, int idx, const char** out_text )
		{
			*out_text = ( *reinterpret_cast< std::function<const char*( int )>* >( data ) )( idx );
			return true;
		}, &lambda, items_count, height_in_items );
	}
}

ImVec4 accent = ImVec4( 1.f, 0.26f, 0.f, 1.f );

void AccentText( const char* text )
{
	ImGui::PushStyleColor( ImGuiCol_Text, accent );
	ImGui::Text( text );
	ImGui::PopStyleColor();
}

IDirect3DDevice9* pDevice;
void DrawGUI()
{
	if ( G::unload )
		return;

	Hook->g_phkD3D9->ReHook();

	ImGui::GetIO().MouseDrawCursor = Vars.Menu.bOpened;
	ImGui_ImplDX9_NewFrame();


	if ( Vars.Menu.bOpened )
	{
		int pX, pY;
		g_pInputSystem->GetCursorPosition( &pX, &pY );
		ImGuiIO& io = ImGui::GetIO();
		io.MousePos.x = ( float ) ( pX );
		io.MousePos.y = ( float ) ( pY );
		ImGui::PushItemWidth( 150 );

		static char szCurrtooltip[ 128 ] = "";

		ImGui::Begin( charenc( "rawsawce for counter-strike: global offensive" ), &Vars.Menu.bOpened, ImVec2( 803, 458 ), 1.0f, ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar);
		{
			ImGui::PushFont(you);
			if ( ImGui::Button( charenc( "G" ), ImVec2(180, 50) ) ) //RAGEBOT
				Vars.Menu.iTab = 0;
			ImGui::PopFont();
			ImGui::PushFont(fuck);
			ImGui::SameLine();
			if ( ImGui::Button( charenc( "D" ), ImVec2(180, 50) ) ) //VISUALS
				Vars.Menu.iTab = 1;
			ImGui::SameLine();
			if ( ImGui::Button( charenc( "H" ), ImVec2(180, 50) ) ) //MISC
				Vars.Menu.iTab = 2;
			ImGui::SameLine();
			if ( ImGui::Button( charenc( "B" ), ImVec2(180, 50) ) ) //SKINCHANGER
				Vars.Menu.iTab = 3;
			ImGui::SameLine();
			if ( ImGui::Button( charenc( "C" ), ImVec2(180, 50) ) ) //CONFIG / COLORS
				Vars.Menu.iTab = 4;
			ImGui::PopFont();

			ImGui::Separator();

			ImGui::Dummy( ImVec2( 389, 21 ) );

			if (Vars.Menu.iTab == 0) // RAGEBOT
			{
				ImGui::BeginChildFrame(3, ImVec2(389, 200), ImGuiWindowFlags_ShowBorders);
				{
					AccentText("aimbot");
					ImGui::Checkbox("enabled##3", &Vars.HvH.Aimbot.bEnabled);
					ImGui::SameLine();
					ImGui::Checkbox("auto shoot", &Vars.HvH.Aimbot.bAutoFire);
					ImGui::SameLine();
					ImGui::Checkbox("auto revolver", &Vars.HvH.Aimbot.bAutoR8);
					ImGui::Combo("hitbox##hitboxes", &Vars.HvH.Aimbot.bHitbox, bHitboxes, ARRAYSIZE(bHitboxes));
					ImGui::Combo("hitscan##hitscanmodes", &Vars.HvH.Aimbot.bHitscan, bHitscan, ARRAYSIZE(bHitscan));
					AccentText("removals");
					ImGui::Checkbox("recoil", &Vars.HvH.Removals.bRecoil);
					AccentText("autowall");
					ImGui::SliderFloat("minimum damage", &Vars.HvH.Aimbot.flMinDamage, 0.f, 120.f, "%.2f");
				}
				ImGui::EndChildFrame();
				ImGui::SameLine();
				ImGui::BeginChildFrame(4, ImVec2(389, 200), ImGuiWindowFlags_ShowBorders);
				{
					AccentText("accuracy");
					ImGui::SliderFloat("max inaccuracy", &Vars.HvH.Aimbot.flMaxSpread, 0.f, 100.f, "%.1f");
					ImGui::Spacing();
					AccentText("anti-aim resolver");
					ImGui::Checkbox("resolver", &Vars.HvH.Aimbot.bResolver);
					ImGui::Text("bruteforce after x shots:");
					ImGui::SliderFloat("##shots", &Vars.HvH.Aimbot.bBFAfter, 0, 20, "%1.f");
				}
				ImGui::EndChildFrame();
				ImGui::BeginChildFrame(5, ImVec2(389, 200), ImGuiWindowFlags_ShowBorders);
				{
					AccentText("hack vs hack");
					ImGui::Checkbox("enabled", &Vars.HvH.bEnabled);
					ImGui::Checkbox("edge anti-aim", &Vars.HvH.bEdge);
					ImGui::Checkbox("lowerbody breaker", &Vars.HvH.bLBYBreaker);
					ImGui::Combo("pitch##pitches", &Vars.HvH.bPitch, bPitches, ARRAYSIZE(bPitches));
					ImGui::Combo("real yaw##realyaws", &Vars.HvH.bRealYaw, bRealYaws, ARRAYSIZE(bRealYaws));
					ImGui::Combo("fake yaw##fakeyaws", &Vars.HvH.bFakeYaw, bFakeYaws, ARRAYSIZE(bFakeYaws));
				}
				ImGui::EndChildFrame();
			} // RAGEBOT

			if (Vars.Menu.iTab == 1) // VISUALS
			{
				ImGui::BeginChildFrame(3, ImVec2(389, 405), ImGuiWindowFlags_ShowBorders);
				{
					AccentText("misc");
					ImGui::Checkbox("radar", &Vars.Other.bRadar);
					ImGui::Checkbox("thirdperson", &Vars.Other.bThirdperson);
					ImGui::SliderFloat("distance##Thirdperson Distance", &Vars.Other.flThirdpersonDist, 0.f, 500.f, "%.0f");
					ImGui::Combo("modes##tpmodes", &Vars.Other.bThirdpersonMode, bThirdpersonMode, ARRAYSIZE(bThirdpersonMode));
					ImGui::Checkbox("angle lines", &Vars.Visuals.bAngleLines);
					ImGui::Checkbox("hitmarkers", &Vars.Visuals.bHitmarker);
					ImGui::Checkbox("hitsound", &Vars.Other.bHitsound);
					ImGui::Checkbox("lag comp. hitboxes", &Vars.Visuals.bHitmarkerHitbox);
					ImGui::Checkbox("sniper crosshair", &Vars.Visuals.bSniperCrosshair);
					ImGui::Checkbox("bullet tracers", &Vars.Visuals.bBulletImpacts);
					ImGui::Checkbox("field of view", &Vars.Visuals.bFovChanger);
					ImGui::SliderFloat("fov", &Vars.Visuals.flFov, 0.f, 180.f, "%.0f");
					ImGui::Checkbox("viewmodel", &Vars.Visuals.bViewmodelFovChanger);
					ImGui::SliderFloat("v.model##flViewmodelFov", &Vars.Visuals.flViewmodelFov, 0.f, 180.f, "%.0f");
					ImGui::Checkbox("spread indicator", &Vars.Visuals.bSpreadCircle);
					ImGui::Checkbox("grenade prediction", &Vars.Visuals.bGrenadePrediction);

					AccentText("material manipulation");
					ImGui::Checkbox("enabled##MaterialConfig", &Vars.Visuals.MaterialConfig.bEnabled);
					ImGui::Checkbox("time: night", &Vars.Visuals.Chams.bDarkMode);
					ImGui::Checkbox("transparent walls", &Vars.Visuals.bAsusWalls);
					ImGui::SliderFloat("opacity##asusblend", &Vars.Visuals.bAsusBlend, 0.f, 100.f, "%.0f");
					ImGui::Checkbox("grey world", &Vars.Visuals.MaterialConfig.bDrawGrey);
					ImGui::Checkbox("fullbright", &Vars.Visuals.MaterialConfig.bFullbright);
					ImGui::Checkbox("low quality", &Vars.Visuals.MaterialConfig.bShowLowResImage);
					ImGui::Checkbox("mlg mode", &Vars.Visuals.MaterialConfig.bShowMipLevels);

				}
				ImGui::EndChildFrame();
				ImGui::SameLine();
				ImGui::BeginChildFrame(4, ImVec2(389, 405), ImGuiWindowFlags_ShowBorders);
				{
					AccentText("extra sensory perception");
					ImGui::Checkbox("enabled##Player ESP", &Vars.Visuals.PlayerEsp.bEnabled);
					ImGui::Checkbox("corner box", &Vars.Visuals.PlayerEsp.bBox);
					dum(); ImGui::Checkbox("filled", &Vars.Visuals.PlayerEsp.bFilled);
					ImGui::Checkbox("skeleton", &Vars.Visuals.PlayerEsp.bSkeleton);
					ImGui::Checkbox("backtracked skeleton", &Vars.Visuals.PlayerEsp.bSkeletonBacktrack);
					ImGui::Checkbox("fade", &Vars.Visuals.PlayerEsp.bFade);
					ImGui::SliderFloat("fade speed", &Vars.Visuals.PlayerEsp.flFadeSpeed, 1.f, 100.f, "%.0f");
					ImGui::Checkbox("dlight", &Vars.Visuals.PlayerEsp.bDlight);

					AccentText("chams");
					ImGui::Checkbox("enabled##Chams", &Vars.Visuals.Chams.bEnabled);
					ImGui::Checkbox("players", &Vars.Visuals.Chams.bPlayers);
					ImGui::Checkbox("hands", &Vars.Visuals.Chams.bHands);
					dum(); ImGui::Combo("material##hand", &Vars.Visuals.Chams.iHandMaterial, CustomMaterials, ARRAYSIZE(CustomMaterials));
					ImGui::Checkbox("weapons", &Vars.Visuals.Chams.bWeapon);
					dum(); ImGui::Combo("material##weapon", &Vars.Visuals.Chams.iWeaponMaterial, CustomMaterials, ARRAYSIZE(CustomMaterials));
					ImGui::Checkbox("xqz", &Vars.Visuals.Chams.bXQZ);
					ImGui::Checkbox("wireframe", &Vars.Visuals.Chams.bOutlined);

				}
				ImGui::EndChildFrame();
			} // VISUALS

			if (Vars.Menu.iTab == 2) // MISCELLANEOUS
			{
				ImGui::BeginChildFrame(4, ImVec2(389, 405), ImGuiWindowFlags_ShowBorders);
				{
					AccentText("miscellaneous");
					ImGui::Checkbox("auto jump", &Vars.Other.bHop);
					ImGui::Checkbox("auto strafe", &Vars.Other.bAutoStrafe);
					ImGui::Checkbox("circle strafe", &Vars.Other.bCircleStrafe);
					AccentText("fakelag");
					ImGui::Combo("mode##fakelagmodes", &Vars.Other.bFakeLagMode, bFakeLagModes, ARRAYSIZE(bFakeLagModes));
					ImGui::SliderInt("max. jitter", &Vars.Other.bFakeLagJitter, 0, 13, "%1.f");
					ImGui::SliderInt("max. factor", &Vars.Other.bFakeLagFactor, 0, 13, "%1.f");
				}
				ImGui::EndChildFrame();
			} // MISCELLANEOUS

			if (Vars.Menu.iTab == 3) // SKINCHANGER
			{
				ImGui::Checkbox("enabled##skinchanger", &Vars.SkinChanger.bEnabled);

				std::vector<EconomyItem_t>& entries = F::SkinChanger->GetItems();

				// If the user deleted the only config let's add one
				if (entries.size() == 0)
					entries.push_back(EconomyItem_t());

				static int selected_id = 0;

				ImGui::Columns(2, nullptr, false);

				// Config selection
				{
					ImGui::PushItemWidth(-1);

					char element_name[64];

					ImGui::ListBox("##config", &selected_id, [&element_name, &entries](int idx)
					{
						sprintf_s(element_name, "%s (%s)", entries.at(idx).name, k_weapon_names.at(entries.at(idx).definition_vector_index).name);
						return element_name;
					}, entries.size(), 15);

					ImVec2 button_size = ImVec2(ImGui::GetColumnWidth() / 2 - 12.8f, 25);

					if (ImGui::Button("add item", button_size))
					{
						entries.push_back(EconomyItem_t());
						selected_id = entries.size() - 1;
					}
					ImGui::SameLine();

					if (ImGui::Button("remove item", button_size))
						entries.erase(entries.begin() + selected_id);

					ImGui::PopItemWidth();
				}

				ImGui::NextColumn();

				selected_id = selected_id < int(entries.size()) ? selected_id : entries.size() - 1;

				EconomyItem_t& selected_entry = entries[selected_id];
				{
					// Name
					ImGui::InputText("name", selected_entry.name, 32);
					ImGui::Dummy(ImVec2(1, 4));

					// Item to change skins for
					ImGui::Combo("item", &selected_entry.definition_vector_index, [](void* data, int idx, const char** out_text)
					{
						*out_text = k_weapon_names[idx].name;
						return true;
					}, nullptr, k_weapon_names.size(), 5);
					ImGui::Dummy(ImVec2(1, 3));

					// Enabled
					ImGui::Checkbox("enabled", &selected_entry.enabled);
					ImGui::Dummy(ImVec2(1, 3));

					// Pattern Seed
					ImGui::InputInt("seed", &selected_entry.seed);
					ImGui::Dummy(ImVec2(1, 4));

					// Custom StatTrak number
					ImGui::InputInt("stattrak", &selected_entry.stat_trak);
					ImGui::Dummy(ImVec2(1, 4));

					// Wear Float
					ImGui::SliderFloat("wear", &selected_entry.wear, FLT_MIN, 1.f, "%.10f", 5);
					ImGui::Dummy(ImVec2(1, 4));

					// Paint kit
					if (selected_entry.definition_index != GLOVE_T_SIDE)
					{
						ImGui::Combo("paint kit", &selected_entry.paint_kit_vector_index, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_skins[idx].name.c_str();
							return true;
						}, nullptr, k_skins.size(), 10);
					}
					else
					{
						ImGui::Combo("paint kit", &selected_entry.paint_kit_vector_index, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_gloves[idx].name.c_str();
							return true;
						}, nullptr, k_gloves.size(), 10);
					}
					ImGui::Dummy(ImVec2(1, 4));

					// Quality
					ImGui::Combo("quality", &selected_entry.entity_quality_vector_index, [](void* data, int idx, const char** out_text)
					{
						*out_text = k_quality_names[idx].name;
						return true;
					}, nullptr, k_quality_names.size(), 5);
					ImGui::Dummy(ImVec2(1, 4));

					// Yes we do it twice to decide knifes
					selected_entry.UpdateValues();

					// Item defindex override
					if (selected_entry.definition_index == WEAPON_KNIFE)
					{
						ImGui::Combo("knife", &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_knife_names.at(idx).name;
							return true;
						}, nullptr, k_knife_names.size(), 5);
					}
					else if (selected_entry.definition_index == GLOVE_T_SIDE)
					{
						ImGui::Combo("glove", &selected_entry.definition_override_vector_index, [](void* data, int idx, const char** out_text)
						{
							*out_text = k_glove_names.at(idx).name;
							return true;
						}, nullptr, k_glove_names.size(), 5);
					}
					else
					{
						// We don't want to override weapons other than knives or gloves
						static auto unused_value = 0;
						selected_entry.definition_override_vector_index = 0;
						ImGui::Combo("unavailable", &unused_value, "only available for knives or gloves\0");
					}
					ImGui::Dummy(ImVec2(1, 4));

					selected_entry.UpdateValues();

					// Custom Name tag
					ImGui::InputText("name tag", selected_entry.custom_name, 32);
					ImGui::Dummy(ImVec2(1, 4));
				}

				ImGui::NextColumn();

				ImGui::Columns(1, nullptr, false);

				ImGui::Separator();

				ImGui::Dummy(ImVec2(1, 10));

				ImGui::Columns(3, nullptr, false);

				ImGui::PushItemWidth(-1);

				// Lower buttons for modifying items and saving
				{
					ImVec2 button_size = ImVec2(ImGui::GetColumnWidth() - 17.f, 25);

					if (ImGui::Button("force update", button_size))
						F::SkinChanger->ForceItemUpdates();
					ImGui::NextColumn();

					if (ImGui::Button("save", button_size))
						F::SkinChanger->Save();
					ImGui::NextColumn();

					if (ImGui::Button("load", button_size))
						F::SkinChanger->Load();
					ImGui::NextColumn();
				}
				ImGui::Dummy(ImVec2(1, 5));

				ImGui::PopItemWidth();
				ImGui::Columns(1);
			} // SKINCHANGER

			if ( Vars.Menu.iTab == 4 ) // CONFIG
			{
				ImGui::BeginChildFrame( 1, ImVec2( 389, 405 ), ImGuiWindowFlags_ShowBorders );
				{
					AccentText( "config" );
					if ( ImGui::Button(charenc( "load##3" ), ImVec2(100, 50)))
					{
						ImGui::SetTooltip( "load saved settings" );
						Config->LoadPreset();
					}
					ImGui::SameLine();
					if (ImGui::Button(charenc("save##3"), ImVec2(100, 50)))
					{
						ImGui::SetTooltip("save settings");
						Config->SavePreset();
					}
					ImGui::SameLine();
					if (ImGui::Button(charenc("reset##3"), ImVec2(100, 50)))
					{
						ImGui::SetTooltip("reset all settings");
						Config->Reset();
					}
				}
				ImGui::EndChildFrame();
				ImGui::SameLine();
				ImGui::BeginChildFrame( 2, ImVec2( 389, 405 ), ImGuiWindowFlags_ShowBorders );
				{
					ImGui::SetColorEditOptions( ImGuiColorEditFlags_NoInputs | ImGuiWindowFlags_ShowBorders | ImGuiColorEditFlags_PickerHueWheel );

					ImGui::Dummy( ImVec2( 1, 5 ) );
					ImGui::Columns( 2, "##123", false );
					AccentText( "chams" );
					dum(); AccentText( "team" );
					dum(); dum(); ImGui::Text( "visible " ); ImGui::SameLine(); dum(); dum(); ImGui::ColorEdit3( "##Visible", Vars.Visuals.Chams.Colors.flTeamVisible );
					dum(); dum(); ImGui::Text( "hidden" ); ImGui::SameLine(); dum(); dum(); ImGui::ColorEdit3( "##Hidden", Vars.Visuals.Chams.Colors.flTeamHidden );
					ImGui::Dummy( ImVec2( 1, 5 ) );

					dum(); AccentText( "enemy" );
					dum(); dum(); ImGui::Text( "visible " ); ImGui::SameLine(); dum(); dum(); ImGui::ColorEdit3( "##Visible2", Vars.Visuals.Chams.Colors.flEnemyVisible );
					dum(); dum(); ImGui::Text( "hidden" ); ImGui::SameLine(); dum(); dum(); ImGui::ColorEdit3( "##Hidden2", Vars.Visuals.Chams.Colors.flEnemyHidden );

					ImGui::Dummy( ImVec2( 1, 10 ) );
					AccentText( "misc" );
					dum(); ImGui::Text("hitmarker"); ImGui::SameLine(); ImGui::Dummy(ImVec2(28, 1)); ImGui::SameLine(); ImGui::ColorEdit3("##Hitmarker", Vars.Visuals.Colors.flHitmarker);
					dum(); ImGui::Text("bullet tracers"); ImGui::SameLine(); ImGui::Dummy(ImVec2(28, 1)); ImGui::SameLine(); ImGui::ColorEdit3("##BulletTracers", Vars.Visuals.Colors.flTracers);
					ImGui::SliderFloat("duration", &Vars.Visuals.Colors.flTracersDuration, 0.f, 10.f, "%.2f");
					ImGui::SliderFloat("width", &Vars.Visuals.Colors.flTracersWidth, 0.f, 10.f, "%.2f");
					ImGui::Checkbox("team check", &Vars.Visuals.bTeamCheck);

					ImGui::NextColumn();
					AccentText( "player esp" );
					dum(); AccentText( "team" );
					dum(); dum(); ImGui::Text( "boxes " ); ImGui::SameLine(); ImGui::Dummy( ImVec2( 22, 1 ) ); ImGui::SameLine(); ImGui::ColorEdit3( "##Boxes", Vars.Visuals.PlayerEsp.Colors.flTeamBoxes );
					dum(); dum(); ImGui::Text( "bones " ); ImGui::SameLine(); ImGui::Dummy( ImVec2( 21, 1 ) ); ImGui::SameLine(); ImGui::ColorEdit3( "##Bones", Vars.Visuals.PlayerEsp.Colors.flTeamBones );
					dum(); dum(); ImGui::Text( "hitboxes " ); ImGui::SameLine(); ImGui::Dummy( ImVec2( 4, 1 ) ); ImGui::SameLine(); ImGui::ColorEdit3( "##Hitboxes", Vars.Visuals.PlayerEsp.Colors.flTeamHitBoxes );
					ImGui::Dummy( ImVec2( 1, 5 ) );

					dum(); AccentText( "enemy" );
					dum(); dum(); ImGui::Text( "boxes " ); ImGui::SameLine(); ImGui::Dummy( ImVec2( 22, 1 ) ); ImGui::SameLine(); ImGui::ColorEdit3( "##Boxes2", Vars.Visuals.PlayerEsp.Colors.flEnemyBoxes );
					dum(); dum(); ImGui::Text( "bones " ); ImGui::SameLine(); ImGui::Dummy( ImVec2( 21, 1 ) ); ImGui::SameLine(); ImGui::ColorEdit3( "##Bones2", Vars.Visuals.PlayerEsp.Colors.flEnemyBones );
					dum(); dum(); ImGui::Text( "hitboxes " ); ImGui::SameLine(); ImGui::Dummy( ImVec2( 4, 1 ) ); ImGui::SameLine(); ImGui::ColorEdit3( "##Hitboxes2", Vars.Visuals.PlayerEsp.Colors.flEnemyHitBoxes );

					ImGui::Dummy( ImVec2( 1, 10 ) );
				}
				ImGui::EndChildFrame();
			} // CONFIG
		}
		ImGui::End();
	}
	ImGui::Render();
}

EndSceneFn oEndScene;
long __stdcall g_hkEndScene( IDirect3DDevice9* pDevice )
{
	if ( !G::d3dinit )
		GUI_Init( pDevice );

	static void* dwReturnAddress = _ReturnAddress();

	if ( dwReturnAddress == _ReturnAddress() )
	{
		pDevice->SetRenderState( D3DRS_COLORWRITEENABLE, 0xFFFFFFFF );
		DrawGUI();
	}

	ImGuiStyle &style = ImGui::GetStyle();

	if (Vars.Menu.bOpened)
	{
		if (style.Alpha > 1.f)
			style.Alpha = 1.f;
		else if (style.Alpha != 1.f)
			style.Alpha += 0.01f;
	}
	else
	{
		if (style.Alpha < 0.f)
			style.Alpha = 0.f;
		else if (style.Alpha != 0.f)
			style.Alpha -= 0.01f;
	}

	return oEndScene( pDevice );
}

ResetFn oReset;
long __stdcall g_hkReset( IDirect3DDevice9* pDevice, D3DPRESENT_PARAMETERS* pPresentationParameters )
{
	if ( !G::d3dinit )
		return oReset( pDevice, pPresentationParameters );

	ImGui_ImplDX9_InvalidateDeviceObjects();

	auto hr = oReset( pDevice, pPresentationParameters );

	ImGui_ImplDX9_CreateDeviceObjects();

	return hr;
}

void GUI_Init( IDirect3DDevice9* pDevice )
{
	ImGui_ImplDX9_Init( G::Window, pDevice );

	ImVec4 bg = ImVec4( 1.f, 1.f, 1.f, 0.008f );
	ImVec4 hovered = ImVec4( 1.f, 1.f, 1.f, 0.04f );
	ImVec4 active = ImVec4( 1.f, 1.f, 1.f, 0.07f );
	ImGuiIO& io = ImGui::GetIO();

	io.Fonts->Clear();
	io.Fonts->AddFontFromFileTTF( "C:\\Windows\\Fonts\\tahoma.ttf", 15.0f );
	fuck = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Aurora.ttf", 42.0f);
	you = io.Fonts->AddFontFromFileTTF("C:\\Windows\\Fonts\\Font.ttf", 42.0f);
	io.Fonts->Build();

	ImGuiStyle& style = ImGui::GetStyle();

	style.Colors[ ImGuiCol_Text ] = ImVec4( 0.78f, 0.78f, 0.78f, 1.00f );
	style.Colors[ ImGuiCol_TextDisabled ] = ImVec4( 0.59f, 0.59f, 0.59f, 1.00f );
	style.Colors[ ImGuiCol_WindowBg ] = ImVec4(0.15f, 0.15f, 0.15f, 0.70f); //main quarter
	//style.Colors[ ImGuiCol_WindowBg ] = ImVec4( 0.02f, 0.02f, 0.02f, 1.0f );
	style.Colors[ ImGuiCol_ChildWindowBg ] = ImVec4( 0.03f, 0.03f, 0.03f, 1.0f );
	style.Colors[ ImGuiCol_Border ] = ImVec4( 0.10f, 0.10f, 0.10f, 1.0f );
	style.Colors[ ImGuiCol_FrameBg ] = ImVec4(0.15f, 0.15f, 0.15f, 0.70f); //child bg color
	style.Colors[ ImGuiCol_FrameBgHovered ] = ImVec4( 0.02f, 0.02f, 0.02f, 1.0f );
	style.Colors[ ImGuiCol_FrameBgActive ] = ImVec4( 0.02f, 0.02f, 0.02f, 1.0f );
	style.Colors[ ImGuiCol_TitleBg ] = bg;
	style.Colors[ ImGuiCol_TitleBgCollapsed ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.0f );
	style.Colors[ ImGuiCol_TitleBgActive ] = ImVec4( 0.00f, 0.00f, 0.00f, 0.0f );
	style.Colors[ ImGuiCol_MenuBarBg ] = bg;
	style.Colors[ ImGuiCol_ScrollbarBg ] = bg;
	style.Colors[ ImGuiCol_ScrollbarGrab ] = accent;
	style.Colors[ ImGuiCol_ScrollbarGrabHovered ] = hovered;
	style.Colors[ ImGuiCol_ScrollbarGrabActive ] = active;
	style.Colors[ ImGuiCol_Separator ] = accent;
	style.Colors[ ImGuiCol_SliderGrab ] = accent;
	style.Colors[ ImGuiCol_SliderGrabActive ] = ImVec4( 1.f, 0.36f, 12.f, 1.f );
	style.Colors[ ImGuiCol_Button ] = ImVec4( 1.f, 1.f, 1.f, 0.01f );
	style.Colors[ ImGuiCol_ButtonHovered ] = hovered;
	style.Colors[ ImGuiCol_ButtonActive ] = accent;
	style.Colors[ ImGuiCol_Header ] = ImVec4( 1.f, 1.f, 1.f, 0.02f );
	style.Colors[ ImGuiCol_HeaderHovered ] = ImVec4( 6.f, 1.f, 1.f, 0.5f );
	style.Colors[ ImGuiCol_HeaderActive ] = ImVec4( 6.f, 1.f, 1.f, 0.5f );
	style.Colors[ ImGuiCol_CloseButton ] = ImVec4( 0.02f, 0.02f, 0.02f, 1.0f );
	style.Colors[ ImGuiCol_CloseButtonActive ] = ImVec4( 0.02f, 0.02f, 0.02f, 1.0f );
	style.Colors[ ImGuiCol_CloseButtonHovered ] = ImVec4( 0.02f, 0.02f, 0.02f, 1.0f );
	style.Colors[ ImGuiCol_ModalWindowDarkening ] = ImVec4( 0.02f, 0.02f, 0.02f, 1.0f );
	style.Colors[ ImGuiCol_ComboBg ] = ImVec4( 0.03f, 0.03f, 0.03f, 1.f );
	style.Colors[ ImGuiCol_CheckMark ] = ImVec4(1.00f, 0.30f, 0.10f, 1.00f); //main colored
	style.Colors[ ImGuiCol_PopupBg ] = ImVec4( 0.03f, 0.03f, 0.03f, 1.f );

	style.WindowRounding = 0.f;
	style.WindowPadding = ImVec2( 8, 8 );
	style.FramePadding = ImVec2( 5, 2 );
	style.ItemInnerSpacing = ImVec2( 4, 4 );
	style.DisplayWindowPadding = ImVec2( 805, 472 );
	style.DisplaySafeAreaPadding = ImVec2( 4, 4 );
	style.FrameRounding = 0.f;
	style.ScrollbarSize = 2.f;
	style.ScrollbarRounding = 0.f;
	style.GrabRounding = 0.f;
	style.GrabMinSize = 8.f;
	style.AntiAliasedShapes = true;
	style.ChildWindowRounding = 0.f;

	G::d3dinit = true;
}