#include "..\CheatInclude.h"
#include "..\Math.h"

void CVisuals::Run()
{
	if ( !g_pEngine->IsInGame() )
	{
		flHurtTime = 0.f; // for hitmarker
		return;
	}
	int i = 1;
	C_BasePlayer* pPlayer = reinterpret_cast< C_BasePlayer* >(g_pClientEntList->GetClientEntity(i));
	m_pLocalPlayer = reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( g_pEngine->GetLocalPlayer() ) );
	if ( !m_pLocalPlayer )
		return;

	for ( int i = 1; i <= g_pGlobalVarsBase->maxClients; i++ )
	{
		C_BasePlayer* pPlayer = reinterpret_cast< C_BasePlayer* >(g_pClientEntList->GetClientEntity(i));
		if ( !pPlayer )
			continue;

		float flDist = pPlayer->GetAbsOrigin().Dist( m_pLocalPlayer->GetAbsOrigin() );
		Entities.push_back( std::pair<int, float>( i, flDist ) );
	}

	std::sort( Entities.begin(), Entities.end(), [] ( const std::pair<int, float> &left, const std::pair<int, float> &right ) { return left.second > right.second; } );


	for ( int i = 0; i < Entities.size(); i++ )
	{
		PlayerESP( i );
	}

	if ( Entities.size() > 0 )
		Entities.clear();

	F::GrenadePrediction->Paint();
	SpreadCircle();
	CustomCrosshair();
	Hitmarker();
	LBYIndicator();
	DrawAngles();
}

void CVisuals::SetHitmarkerValues( float hurttime, int health, int victim )
{
	flHurtTime = hurttime;
	iHealth = health;
	iVictim = victim;
}

void CVisuals::SpreadCircle()
{
	if ( !Vars.Visuals.bSpreadCircle )
		return;

	if ( !m_pLocalPlayer->IsAlive() )
		return;

	int width, height;
	g_pEngine->GetScreenSize( width, height );

	C_BaseCombatWeapon* pWeapon = m_pLocalPlayer->GetActiveWeapon();
	if ( !pWeapon )
		return;

	float cone = pWeapon->GetSpread() + pWeapon->GetInaccuracy();
	if ( cone > 0.0f )
	{
		int crossX = width / 2;
		int crossY = height / 2;
		if ( cone < 0.01f ) cone = 0.01f;
		float size = ( cone * height ) * 0.7f;
		static Color color( 255, 105, 180, 20 );
		static Color color2( 255, 105, 180, 8 );
		D::DrawFilledCircle( Vector2D( crossX, crossY ), color2, ( int ) size, 70 );
		D::DrawCircle( crossX, crossY, ( int ) size, 70, color );
	}
}

void CVisuals::SetupListener()
{
	pEventListener = new EventListener;

	static auto developer = g_pCvar->FindVar( "developer" );
	developer->SetValue( 1 );
	static auto con_filter_text = g_pCvar->FindVar( "con_filter_text" );
	static auto con_filter_text_out = g_pCvar->FindVar( "con_filter_text_out" );
	static auto con_filter_enable = g_pCvar->FindVar( "con_filter_enable" );

	con_filter_text->SetValue( "hit " );
	con_filter_text_out->SetValue( "" );
	con_filter_enable->SetValue( 2 );
}

void CVisuals::RemoveListener()
{
	if ( pEventListener )
	{
		pEventListener->~EventListener();
		delete pEventListener;
	}
}

void CVisuals::PlayerESP( int index )
{
	if ( !Vars.Visuals.PlayerEsp.bEnabled )
		return;

	if ( Entities.empty() )
		return;

	auto pPlayer = reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( Entities[ index ].first ) );

	if ( !pPlayer || pPlayer == m_pLocalPlayer || ( !pPlayer->IsEnemy() && Vars.Visuals.bTeamCheck ) )
		return;

	int iClassID = pPlayer->GetClientClass()->m_ClassID;
	if ( iClassID != 83 && iClassID != 35 )
		return;

	if ( Vars.Visuals.PlayerEsp.bFade )
	{
		float step = 255.f * ( Vars.Visuals.PlayerEsp.flFadeSpeed / 10 ) * g_pGlobalVarsBase->frametime;
		pPlayer->IsDormant() ? PlayerList[ index ].flFadeAlpha -= step : PlayerList[ index ].flFadeAlpha += step;
		PlayerList[ index ].AlphaClamp();

		if ( PlayerList[ index ].flFadeAlpha == 0.f )
			return;
	}

	if ( !pPlayer->IsAlive() )
	{
		PlayerList[ index ].flFadeAlpha = 255.f;
		return;
	}

	if ( !Vars.Visuals.PlayerEsp.bFade )
	{
		PlayerList[ index ].flFadeAlpha = 255.f;
		if ( !pPlayer->IsDormant() )
			return;
	}

	Vector max = pPlayer->GetCollideable()->OBBMaxs();
	Vector pos, pos3D;
	Vector top, top3D;

	pos3D = pPlayer->GetAbsOrigin();
	top3D = pos3D + Vector( 0, 0, max.z );

	if ( !D::WorldToScreen( pos3D, pos ) || !D::WorldToScreen( top3D, top ) )
		return;

	float height = ( pos.y - top.y );
	float width = height / 4.f;

	if ( iClassID == 83 )
	{
		D::DrawString( Font::ESP, top.x, top.y + 3, Color::White(), FONT_CENTER, charenc( "hostage" ) );
		return;
	}

	Color flTeamBoxes = Color( Vars.Visuals.PlayerEsp.Colors.flTeamBoxes, PlayerList[ index ].flFadeAlpha );
	Color flEnemyBoxes = Color( Vars.Visuals.PlayerEsp.Colors.flEnemyBoxes, PlayerList[ index ].flFadeAlpha );
	Color flTeamBones = Color( Vars.Visuals.PlayerEsp.Colors.flTeamBones, PlayerList[ index ].flFadeAlpha );
	Color flEnemyBones = Color( Vars.Visuals.PlayerEsp.Colors.flEnemyBones, PlayerList[ index ].flFadeAlpha );
	Color flTeamFilling = Color( Vars.Visuals.PlayerEsp.Colors.flTeamBoxes, PlayerList[ index ].flFadeAlpha / 50 );
	Color flEnemyFilling = Color( Vars.Visuals.PlayerEsp.Colors.flEnemyBoxes, PlayerList[ index ].flFadeAlpha / 50 );

	if (Vars.Visuals.PlayerEsp.bSkeleton)
	{
		Skeleton(pPlayer, !pPlayer->IsEnemy() && Vars.Visuals.bTeamCheck ? flTeamBones : flEnemyBones);
	}

	if ( Vars.Visuals.PlayerEsp.bSkeletonBacktrack )
	{
		std::deque< LagRecord > &track = F::Backtrack->m_PlayerTrack[ pPlayer->GetIClientEntity()->EntIndex() - 1 ];
		for ( int i = 0; i < track.size(); i++ )
		{
			F::Backtrack->BackupPlayer( pPlayer );
			F::Backtrack->StartLagCompensation( pPlayer, NULL, i );
			F::Backtrack->RestorePlayer( pPlayer );
		}
	}
	D::CornerBox( top.x, top.y, width, height, false, !pPlayer->IsEnemy() && Vars.Visuals.bTeamCheck ? flTeamBoxes : flEnemyBoxes );

	D::FilledCornerBox( top.x, top.y, width, height, false, !pPlayer->IsEnemy() && Vars.Visuals.bTeamCheck ? flTeamFilling : flEnemyFilling );

	Info( top.x, top.y, width, height, pPlayer, !pPlayer->IsEnemy() && Vars.Visuals.bTeamCheck ? flTeamBoxes : flEnemyBoxes );

	HealthBar( top.x, top.y, width, height, pPlayer->GetHealth(), PlayerList[ index ].flFadeAlpha );
}

void CVisuals::CapsuleOverlay( int index, Color col, float duration )
{
	if ( index == g_pEngine->GetLocalPlayer() )
		return;

	auto pPlayer = reinterpret_cast < C_BasePlayer* > ( g_pClientEntList->GetClientEntity( index ) );
	if ( !pPlayer )
		return;

	studiohdr_t* pStudioModel = g_pVModelInfo->GetStudioModel( pPlayer->GetModel() );
	if ( !pStudioModel )
		return;

	static matrix3x4_t pBoneToWorldOut[ 128 ];
	if ( !pPlayer->SetupBones( pBoneToWorldOut, 128, 256, g_pGlobalVarsBase->curtime ) )
		return;

	mstudiohitboxset_t* pHitboxSet = pStudioModel->pHitboxSet( 0 );
	if ( !pHitboxSet )
		return;

	for ( int i = 0; i < pHitboxSet->numhitboxes; i++ )
	{
		mstudiobbox_t* pHitbox = pHitboxSet->pHitbox( i );
		if ( !pHitbox )
			continue;

		Vector vMin, vMax;
		Math->VectorTransform( pHitbox->bbmin, pBoneToWorldOut[ pHitbox->bone ], vMin ); //nullptr???
		Math->VectorTransform( pHitbox->bbmax, pBoneToWorldOut[ pHitbox->bone ], vMax );

		if ( pHitbox->radius > -1 )
		{
			g_pVDebugOverlay->AddCapsuleOverlay( vMin, vMax, pHitbox->radius, col.r(), col.g(), col.b(), 100, duration );
		}
	}
}

void CVisuals::Skeleton( C_BasePlayer *pPlayer, Color col )
{
	if (!Vars.Visuals.PlayerEsp.bSkeleton)
		return;

	studiohdr_t* pStudioModel = g_pVModelInfo->GetStudioModel( pPlayer->GetModel() );
	if ( !pStudioModel )
		return;

	static matrix3x4_t pBoneToWorldOut[ 128 ];
	if ( !pPlayer->SetupBones( pBoneToWorldOut, 128, 256, g_pGlobalVarsBase->curtime ) )
		return;

	mstudiohitboxset_t* pHitboxSet = pStudioModel->pHitboxSet( 0 );

	for ( int i = 0; i < pStudioModel->numbones; i++ )
	{
		mstudiobone_t* pBone = pStudioModel->pBone( i );
		if ( !pBone || !( pBone->flags & 256 ) || pBone->parent == -1 )
			continue;

		Vector vBonePos1;
		if ( !D::WorldToScreen( Vector( pBoneToWorldOut[ i ][ 0 ][ 3 ], pBoneToWorldOut[ i ][ 1 ][ 3 ], pBoneToWorldOut[ i ][ 2 ][ 3 ] ), vBonePos1 ) )
			continue;

		Vector vBonePos2;
		if ( !D::WorldToScreen( Vector( pBoneToWorldOut[ pBone->parent ][ 0 ][ 3 ], pBoneToWorldOut[ pBone->parent ][ 1 ][ 3 ], pBoneToWorldOut[ pBone->parent ][ 2 ][ 3 ] ), vBonePos2 ) )
			continue;

		D::DrawLine( ( int ) vBonePos1.x, ( int ) vBonePos1.y, ( int ) vBonePos2.x, ( int ) vBonePos2.y, col );
	}
}

void CVisuals::HealthBar( const int32_t x, const int32_t y, const int32_t w, const int32_t h, int health, float alpha )
{
	float flBoxes = std::ceil( health / 10.f );
	float flHeight = h / 10.f;

	int Red = 255 - ( health*2.55 );
	int Green = health * 2.55;

	D::DrawRect( x - w - w / 10, y, 2, flHeight * flBoxes + 1, Color( Red, Green, 0, alpha ) );
}

void CVisuals::Info( const int32_t x, const int32_t y, const int32_t w, const int32_t h, C_BasePlayer* pPlayer, Color col )
{
	player_info_t pinfo;
	g_pEngine->GetPlayerInfo( pPlayer->EntIndex(), &pinfo );

	D::DrawString(Font::ESP, x, y - h / 10, col, FONT_CENTER, "%s", pinfo.name); //players' name
	D::DrawString(Font::ESP, x, y + h + h / 10, col, FONT_CENTER, "%s", pPlayer->GetActiveWeapon()->GetWeaponName().c_str()); //players' weapons
	D::DrawString(Font::ESP, x, h + y - 10, col, FONT_CENTER, "%s", pinfo.guid); //players' steam id
	D::DrawString(Font::ESP, x, y - h / 5, col, FONT_CENTER, "%d $", std::int16_t(pPlayer->GetMoney())); //players' money

	int bottom = 0;

	if (pPlayer->IsScoped())
		D::DrawString( Font::TinyESP, x + w + w / 7, y + h / 10 + ( 10 * bottom++ ), Color::LightBlue(), FONT_LEFT, "scoped" ); //is scoped?

	if ( PlayerList[ pPlayer->GetIClientEntity()->EntIndex() ].bFakeAngles )
		D::DrawString( Font::TinyESP, x + w + w / 7, y + h / 10 + ( 10 * bottom++ ), Color::Pink(), FONT_LEFT, "fake" ); //is fake angle?

	if ( PlayerList[ pPlayer->GetIClientEntity()->EntIndex() ].bBacktracking )
		D::DrawString( Font::TinyESP, x + w + w / 7, y + h / 10 + ( 10 * bottom++ ), Color::Green(), FONT_LEFT, "backtracking" ); //are we backtracking the enemy?
}

void CVisuals::CustomCrosshair()
{
	if ( !m_pLocalPlayer->IsAlive() )
		return;

	int szWidth, szHeight;
	g_pEngine->GetScreenSize( szWidth, szHeight );

	Color ch = G::CanShoot ? Color::Pink() : Color::White();

	D::DrawRect( szWidth / 2 - 1, szHeight / 2 - 1, 2, 2, ch );

	if ( !Vars.Visuals.bSniperCrosshair )
		return;

	if ( m_pLocalPlayer->GetActiveWeapon()->GetWeaponType() == WEAPONTYPE_SNIPER_RIFLE && !m_pLocalPlayer->IsScoped() )
	{
		D::DrawRect( szWidth / 2 - 5, szHeight / 2 - 1, 10, 2, ch );
		D::DrawRect( szWidth / 2 - 1, szHeight / 2 - 5, 2, 10, ch );
	}

	if ( !m_pLocalPlayer->IsScoped() )
		return;

	D::DrawLine( 0, szHeight / 2, szWidth, szHeight / 2, Color::Black() );
	D::DrawLine( szWidth / 2, 0, szWidth / 2, szHeight, Color::Black() );

	D::DrawRect( szWidth / 2 - 1, szHeight / 2 - 1, 3, 3, ch );
}

void CVisuals::Hitmarker()
{
	if ( !Vars.Visuals.bHitmarker )
		return;

	static int lineSize = 5;

	static float alpha = 0;
	float step = 255.f / 0.3f * g_pGlobalVarsBase->frametime;

	if ( flHurtTime + 0.4f >= g_pGlobalVarsBase->curtime )
		alpha = 255.f;
	else
		alpha -= step;

	if ( alpha > 0 )
	{
		int screenSizeX, screenCenterX;
		int screenSizeY, screenCenterY;
		g_pEngine->GetScreenSize( screenSizeX, screenSizeY );

		screenCenterX = screenSizeX / 2;
		screenCenterY = screenSizeY / 2;

		Color col = Color( Vars.Visuals.Colors.flHitmarker, alpha );

		D::DrawLine( screenCenterX - lineSize * 2, screenCenterY - lineSize * 2, screenCenterX - ( lineSize ), screenCenterY - ( lineSize ), col );
		D::DrawLine( screenCenterX - lineSize * 2, screenCenterY + lineSize * 2, screenCenterX - ( lineSize ), screenCenterY + ( lineSize ), col );
		D::DrawLine( screenCenterX + lineSize * 2, screenCenterY + lineSize * 2, screenCenterX + ( lineSize ), screenCenterY + ( lineSize ), col );
		D::DrawLine( screenCenterX + lineSize * 2, screenCenterY - lineSize * 2, screenCenterX + ( lineSize ), screenCenterY - ( lineSize ), col );

		D::DrawString( Font::ESP, screenCenterX + 12, screenCenterY + 12, Color( 255, 0, 0, alpha ), FONT_CENTER, "- %i", iHealth );
	}
}

char* HitgroupToName( int hitgroup )
{
	switch ( hitgroup )
	{
		case HITGROUP_HEAD:
			return "head";
		case HITGROUP_LEFTLEG:
		case HITGROUP_RIGHTLEG:
			return "leg";
		case HITGROUP_STOMACH:
			return "stomach";
		default:
			return "body";
	}
}

void CVisuals::EventListener::FireGameEvent( IGameEvent * pEvent )
{
	if ( !strcmp( pEvent->GetName(), "player_hurt" ) )
	{
		int iAttacker = g_pEngine->GetPlayerForUserID( pEvent->GetInt( "attacker" ) );
		int iVictim = g_pEngine->GetPlayerForUserID( pEvent->GetInt( "userid" ) );
		if ( iAttacker == g_pEngine->GetLocalPlayer() && iVictim != g_pEngine->GetLocalPlayer() )
		{
			if ( Vars.Other.bHitsound )
				g_pSurface->PlaySound( "buttons\\arena_switch_press_02.wav" );

			if ( Vars.Visuals.bHitmarkerHitbox )
				F::Visuals->CapsuleOverlay( iVictim, Vars.Visuals.PlayerEsp.Colors.flEnemyHitBoxes, 0.8f );

			F::Visuals->SetHitmarkerValues( g_pGlobalVarsBase->curtime, pEvent->GetInt( "dmg_health" ), iVictim );

			auto pVictim = reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( iVictim ) );

			player_info_t pinfo;
			g_pEngine->GetPlayerInfo( iVictim, &pinfo );
			g_pCvar->ConsoleColorPrintf( Color( 200, 255, 0, 255 ), "[RawSawce] " );
			G::Msg( "Hit %s in the %s for %d damage (%d health remaining) \n", pinfo.name, HitgroupToName( pEvent->GetInt( "hitgroup" ) ), pEvent->GetInt( "dmg_health" ), pEvent->GetInt( "health" ) );
		}
	}
	else if ( !strcmp( pEvent->GetName(), "bullet_impact" ) )
	{
		int iUser = g_pEngine->GetPlayerForUserID( pEvent->GetInt( "userid" ) );

		auto pPlayer = reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( iUser ) );

		if ( !pPlayer )
			return;

		if (iUser != g_pEngine->GetLocalPlayer())
			return;

		if ( pPlayer->IsDormant() )
			return;

		float x, y, z;
		x = pEvent->GetFloat( "x" );
		y = pEvent->GetFloat( "y" );
		z = pEvent->GetFloat( "z" );

		BulletImpact_t impact( pPlayer, Vector( x, y, z ), g_pGlobalVarsBase->curtime, iUser == g_pEngine->GetLocalPlayer() ? Color::Green() : Color::Red() );

		F::Visuals->Impacts.push_back( impact );
	}
	else if ( !strcmp( pEvent->GetName(), "item_purchase" ) )
	{
		int iUser = g_pEngine->GetPlayerForUserID( pEvent->GetInt( "userid" ) );

		auto pLocalPlayer = reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( g_pEngine->GetLocalPlayer() ) );
		if ( pLocalPlayer || pLocalPlayer->GetTeam() == pEvent->GetInt( "team" ) )
			return;

		player_info_t pinfo;
		g_pEngine->GetPlayerInfo( iUser, &pinfo );
		g_pCvar->ConsoleColorPrintf( Color( 200, 255, 0, 255 ), "[RawSawce] " );
		G::Msg( "%s bought %s\n", pinfo.name, pEvent->GetString( "weapon" ) );
	}
};

void CVisuals::LBYIndicator()
{
	if (!Vars.HvH.bEnabled)
		return;

	int screenSizeX;
	int screenSizeY;
	g_pEngine->GetScreenSize( screenSizeX, screenSizeY );

	static float prevdelta = 0;
	static float oldcurrtime = 0;
	float delta = std::abs( F::Thirdperson->m_angle.Clamp().y - m_pLocalPlayer->GetLowerBodyYawTarget() );

	D::DrawString( Font::LBY, 15, screenSizeY - 60, delta > 35 || prevdelta > 35 ? Color::Green() : Color::Red(), FONT_LEFT, "lby" );

	if ( g_pGlobalVarsBase->curtime > oldcurrtime + g_pGlobalVarsBase->interval_per_tick )
	{
		prevdelta = delta;
		oldcurrtime = g_pGlobalVarsBase->curtime;
	}
	if ( oldcurrtime > g_pGlobalVarsBase->curtime )
	{
		oldcurrtime = 0;
	}
}

void CVisuals::BulletTracers()
{
	if ( !Vars.Visuals.bBulletImpacts )
		return;

	//if the player is not ingame, clear the shots
	if ( !g_pEngine->IsInGame() || !m_pLocalPlayer )
	{
		Impacts.clear();
		return;
	}

	if ( Impacts.size() > 30 )
		Impacts.pop_back();

	for ( int i = 0; i < Impacts.size(); i++ )
	{
		auto current = Impacts.at( i );

		if ( !current.pPlayer )
			continue;

		if ( current.pPlayer->IsDormant() )
			continue;

		if ( current.pPlayer == m_pLocalPlayer )
			current.color = Color( Vars.Visuals.Colors.flTracers ); //color of local player's tracers

		BeamInfo_t beamInfo;
		beamInfo.m_nType = TE_BEAMPOINTS;
		beamInfo.m_pszModelName = "sprites/physbeam.vmt";
		beamInfo.m_nModelIndex = -1;
		beamInfo.m_flHaloScale = 0.0f;
		beamInfo.m_flLife = Vars.Visuals.Colors.flTracersDuration; //duration of tracers
		beamInfo.m_flWidth = Vars.Visuals.Colors.flTracersWidth; //start width
		beamInfo.m_flEndWidth = Vars.Visuals.Colors.flTracersWidth; //end width
		beamInfo.m_flFadeLength = 0.0f;
		beamInfo.m_flAmplitude = 2.0f;
		beamInfo.m_flBrightness = 255.f;
		beamInfo.m_flSpeed = 0.2f;
		beamInfo.m_nStartFrame = 0;
		beamInfo.m_flFrameRate = 0.f;
		beamInfo.m_flRed = current.color.r();
		beamInfo.m_flGreen = current.color.g();
		beamInfo.m_flBlue = current.color.b();
		beamInfo.m_nSegments = 2;
		beamInfo.m_bRenderable = true;
		beamInfo.m_nFlags = FBEAM_ONLYNOISEONCE | FBEAM_NOTILE | FBEAM_HALOBEAM;

		beamInfo.m_vecStart = current.pPlayer->GetEyePosition();
		beamInfo.m_vecEnd = current.vecImpactPos;

		auto beam = g_pViewRenderBeams->CreateBeamPoints( beamInfo );
		if ( beam )
			g_pViewRenderBeams->DrawBeam( beam );

		g_pVDebugOverlay->AddBoxOverlay( current.vecImpactPos, Vector( -2, -2, -2 ), Vector( 2, 2, 2 ), Vector( 0, 0, 0 ), current.color.r(), current.color.g(), current.color.b(), 125, 0.8f );

		Impacts.erase( Impacts.begin() + i );
	}
}

void CVisuals::DrawAngles()
{
	if (!Vars.Visuals.bAngleLines)
		return;

	Vector src3D, dst3D, forward, src, dst;
	trace_t tr;
	Ray_t ray;
	CTraceFilter filter;

	filter.pSkip = m_pLocalPlayer;

	///////////////////

	Math->AngleVectors(QAngle(0, m_pLocalPlayer->GetLowerBodyYawTarget(), 0), &forward); //gets local player's LBY
	src3D = m_pLocalPlayer->GetOrigin();
	dst3D = src3D + (forward * 50.f); //replace this with the length you want the line to have

	ray.Init(src3D, dst3D);

	g_pEngineTrace->TraceRay(ray, 0, &filter, &tr);

	if (!D::WorldToScreen(src3D, src) || !D::WorldToScreen(tr.endpos, dst))
		return;

	D::DrawLine(src.x, src.y, dst.x, dst.y, Color(255, 165, 0, 255)); //draws a line
	D::DrawString(Font::MediumESP, dst.x, dst.y, Color(255, 165.f, 0, 255), FONT_CENTER, "lowerbody"); //draws text at the end of the line

	///////////////////

	Math->AngleVectors(QAngle(0, m_pLocalPlayer->GetEyeAngles().y, 0), &forward); //gets the fake angle
	src3D = m_pLocalPlayer->GetOrigin();
	dst3D = src3D + (forward * 50.f); //replace this with the length you want the line to have

	ray.Init(src3D, dst3D);

	g_pEngineTrace->TraceRay(ray, 0, &filter, &tr);

	if (!D::WorldToScreen(src3D, src) || !D::WorldToScreen(tr.endpos, dst))
		return;

	D::DrawLine(src.x, src.y, dst.x, dst.y, Color(0, 255, 0, 255));
	D::DrawString(Font::MediumESP, dst.x, dst.y, Color(0, 255, 0, 255), FONT_CENTER, "fake"); //draws text at the end of the line

	///////////////////
}