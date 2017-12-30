#include "..\CheatInclude.h"
#include "..\Math.h"

Vector C_BasePlayer::GetEyePosition() { return this->GetAbsOrigin() + this->GetViewOffset(); }
bool C_BasePlayer::IsAlive() { return this->GetLifeState() == LifeState::ALIVE; }
bool C_BasePlayer::IsEnemy() { return this->GetTeam() != ( reinterpret_cast< C_BasePlayer* >( g_pClientEntList->GetClientEntity( g_pEngine->GetLocalPlayer() ) ) )->GetTeam(); }
player_info_t C_BasePlayer::GetPlayerInfo() { player_info_t pinfo; g_pEngine->GetPlayerInfo( this->index, &pinfo ); return pinfo; }
std::string C_BasePlayer::GetName() { return std::string( this->GetPlayerInfo().name ); }
C_BaseCombatWeapon* C_BasePlayer::GetActiveWeapon() { auto handle = this->GetActiveWeaponHandle(); if ( !handle ) { return 0; }; return reinterpret_cast< C_BaseCombatWeapon* > ( g_pClientEntList->GetClientEntityFromHandle( *handle ) ); }
int C_BaseCombatWeapon::GetWeaponID() { if ( !this ) return -1; return ( ( C_BaseAttributableItem* ) this )->GetItemDefinitionIndex(); }

int C_BaseCombatWeapon::GetWeaponType() 
{ 
	if ( !this ) return -1; return this->GetCSWpnData()->WeaponType; 
}

bool C_BaseCombatWeapon::IsGrenade()
{ 
	return this->GetWeaponID() >= WEAPON_FLASHBANG && this->GetWeaponID() <= WEAPON_INCGRENADE;
}

CCSWeaponData* C_BaseCombatWeapon::GetCSWpnData() { return U::GetVFunc<CCSWeaponData*( __thiscall* )( void* )>( this, 446 )( this ); }
void C_BaseEntity::SetModelIndex( int index ) { U::GetVFunc<void( __thiscall* )( C_BaseEntity*, int )>( this, 75 )( this, index ); }
model_t* C_BasePlayer::GetModel() { return *reinterpret_cast< model_t** > ( reinterpret_cast< uintptr_t >( this ) + 0x6C ); }
int IClientRenderable::DrawModel( DrawModelFlags_t flags, uint8_t alpha ) { return U::GetVFunc<int( __thiscall* ) ( void*, DrawModelFlags_t, uint8_t )>( this, 9 )( this, flags, alpha ); }
bool C_BaseCombatWeapon::IsEmpty() { return this->GetClip1() == 0; }
void C_BaseCombatWeapon::UpdateAccuracyPenalty() { U::GetVFunc<void( __thiscall* )( void* )>( this, 470 )( this ); }
float C_BaseCombatWeapon::GetSpread() { return U::GetVFunc<float( __thiscall* )( void* )>( this, 439 )( this ); }
float C_BaseCombatWeapon::GetInaccuracy() { return U::GetVFunc<float( __thiscall* )( void* )>( this, 469 )( this ); }
Vector & C_BaseEntity::GetAbsOrigin() { return U::GetVFunc<Vector&( __thiscall* )( void* )>( this, 10 )( this ); }
QAngle & C_BasePlayer::GetAbsAngles() { return U::GetVFunc<QAngle&( __thiscall* )( void* )>( this, 11 )( this ); }
void C_BasePlayer::UpdateAnimations() { U::GetVFunc<void( __thiscall* )( void* )>( this, 218 )( this ); }

#define m_vecMins 0x0320
#define m_vecMaxs 0x032C
Vector C_BaseEntity::GetMins()
{
	Vector temp;
	return ((DWORD)this + m_vecMins, temp);
	return temp;
}

Vector C_BaseEntity::GetMaxs()
{
	Vector temp;
	return ((DWORD)this + m_vecMaxs, temp);
	return temp;
}

void C_BasePlayer::SetAbsOrigin( Vector vec )
{
	using SetAbsOriginFn = void( __thiscall* )( void*, const Vector &origin );
	static SetAbsOriginFn SetAbsOrigin;

	if ( !SetAbsOrigin )
		SetAbsOrigin = reinterpret_cast< SetAbsOriginFn >( U::FindSig( charenc( "client.dll" ), charenc( "55 8B EC 83 E4 F8 51 53 56 57 8B F1 E8 ?? ??" ) ) );

	SetAbsOrigin( this, vec );
}

void C_BasePlayer::SetAbsAngles( QAngle vec )
{
	using SetAbsAnglesFn = void( __thiscall* )( void*, const QAngle &angles );
	static SetAbsAnglesFn SetAbsAngles;

	if ( !SetAbsAngles )
		SetAbsAngles = reinterpret_cast< SetAbsAnglesFn >( ( uintptr_t ) U::FindSig( charenc( "client.dll" ), charenc( "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1 E8" ) ) );

	SetAbsAngles( this, vec );
}

Vector C_BasePlayer::GetHitboxPos( int iHitbox )
{
	if ( !this )
		return Vector( 0, 0, 0 );
	if ( IsDormant() )
		return Vector( 0, 0, 0 );
	if ( iHitbox < 0 || iHitbox >= 128 )
		return Vector( 0, 0, 0 );

	matrix3x4a_t matrix[ 128 ];
	//Vector vMin, vMax;

	const auto pModel = GetModel();
	if ( !pModel )
		return Vector( 0, 0, 0 );

	auto pStudioHdr = g_pVModelInfo->GetStudioModel( pModel );
	if ( !pStudioHdr )
		return Vector( 0, 0, 0 );

	if ( !SetupBones( matrix, 128, 0x100, g_pGlobalVarsBase->curtime ) )
		return Vector( 0, 0, 0 );

	auto pSet = pStudioHdr->pHitboxSet( GetHitboxSet() );
	if ( !pSet )
		return Vector( 0, 0, 0 );


	auto pBox = pSet->pHitbox( iHitbox );
	if ( !pBox )
		return Vector( 0, 0, 0 );

	Vector center = ( ( pBox->bbmin + pBox->bbmax ) * .5f );
	Vector tmp;

	Math->VectorTransform( center, matrix[ pBox->bone ], tmp );
	return tmp;
}

bool C_BaseCombatWeapon::IsBeingThrown()
{
	if ( this->IsGrenade() )
	{
		if ( !this->GetPinPulled() )
		{
			float fThrowTime = this->GetThrowTime();

			if ( ( fThrowTime > 0 ) && ( fThrowTime < g_pGlobalVarsBase->curtime ) )
				return true;
		}
	}
	return false;
}

bool C_BasePlayer::SetupBones( matrix3x4_t *pBoneToWorldOut, int nMaxBones, int boneMask, float currentTime )
{
	*reinterpret_cast< int* > ( reinterpret_cast< uintptr_t >( this ) + 0xA30 ) = g_pGlobalVarsBase->framecount;
	*reinterpret_cast< int* > ( reinterpret_cast< uintptr_t >( this ) + 0xA28 ) = 0;

	__asm
	{
		mov edi, this
		lea ecx, dword ptr ds : [edi + 0x4]
		mov edx, dword ptr ds : [ecx]
		push currentTime
		push boneMask
		push nMaxBones
		push pBoneToWorldOut
		call dword ptr ds : [edx + 0x34]
	}
}

CUtlVector<C_AnimationLayer>& C_BasePlayer::GetAnimOverlays()
{
	return ( *reinterpret_cast< CUtlVector<C_AnimationLayer>* >( reinterpret_cast< std::uintptr_t >( this ) + 0x2970 ) );
}

C_AnimationLayer * C_BasePlayer::GetAnimOverlay( int i )
{
	Assert( i >= 0 && i < MAX_OVERLAYS );
	return &GetAnimOverlays()[ i ];
}

int C_BasePlayer::GetNumAnimOverlays()
{
	return GetAnimOverlays().Count();
}

int C_BasePlayer::GetSequenceActivity( int sequence )
{
	auto hdr = g_pVModelInfo->GetStudioModel( this->GetModel() );
	if ( !hdr )
		return -1;

	static auto GetSequenceActivity = reinterpret_cast< int( __fastcall* )( void*, studiohdr_t*, int ) >( ( uintptr_t ) U::FindSig( charenc( "client.dll" ), charenc( "55 8B EC 83 7D 08 FF 56 8B F1 74 3D" ) ) );

	return GetSequenceActivity( this, hdr, sequence );
}

bool C_BasePlayer::GetPointsFromHitbox( C_BasePlayer* pBaseEntity, const int& iHitbox, std::vector<Vector>& points )
{
	if ( !this )
		return false;

	matrix3x4_t cCachedBones[ 128 ];
	if ( !this->SetupBones( cCachedBones, 128, 0x100, 0.f ) )
		return false;

	const auto* pModel = this->GetModel();
	if ( !pModel )
	{
		return false;
	}

	auto* pStudioHdr = g_pVModelInfo->GetStudioModel( pModel );
	if ( !pStudioHdr )
		return false;

	auto* pHitbox = pStudioHdr->pHitbox( iHitbox, 0 );
	if ( !pHitbox )
		return false;

	auto& mat = cCachedBones[ pHitbox->bone ];

	auto c = Math->CalcAngle( pBaseEntity->GetEyePosition(), this->GetEyePosition() );

	auto cs = cosf( DEG2RAD( c.y ) );
	auto sn = sinf( DEG2RAD( c.y ) );

	auto mod = pHitbox->radius != -1.f ? pHitbox->radius : 0.f;

	Vector mins;
	Math->VectorTransform( pHitbox->bbmin - Vector( mod, mod, mod ), mat, mins );
	Vector maxs;
	Math->VectorTransform( pHitbox->bbmax + Vector( mod, mod, mod ), mat, maxs );
	auto center = ( mins + maxs ) * 0.5f;

	auto fscale = 0.8f;
	float rs = mod * fscale;

	switch ( iHitbox )
	{
		case HITBOX_HEAD:
			{
				points.push_back( center );
				if ( !U::IsVisible( pBaseEntity->GetEyePosition(), center, this, pBaseEntity ) )
				{
					points.push_back( Vector( center.x, center.y, center.z + rs ) );
					points.push_back( Vector( center.x + cs + rs * sn, center.y + sn - rs * cs, center.z ) );
					points.push_back( Vector( center.x + cs - rs * sn, center.y + sn + rs * cs, center.z ) );
				}
				break;
			}
		case HITBOX_PELVIS:
			{
				points.push_back( center );
				if ( !U::IsVisible( pBaseEntity->GetEyePosition(), center, this, pBaseEntity ) )
				{
					points.push_back( Vector( center.x, center.y, center.z + rs ) );
					points.push_back( Vector( center.x + cs + rs * sn, center.y + sn - rs * cs, center.z ) );
					points.push_back( Vector( center.x + cs - rs * sn, center.y + sn + rs * cs, center.z ) );
				}
				break;
			}
		default:
			{
				points.push_back( center );
				break;
			}
		}
	return true;
}

void C_BasePlayer::SetAngle2( QAngle wantedang )
{
	typedef void( __thiscall* SetAngleFn )( void*, const QAngle & );
	static SetAngleFn SetAngle = ( SetAngleFn ) ( ( uintptr_t ) U::FindSig( "client.dll", "55 8B EC 83 E4 F8 83 EC 64 53 56 57 8B F1" ) );
	SetAngle( this, wantedang );
}

std::string C_BaseCombatWeapon::GetWeaponName()
{
	if ( !this )
		return "";

	int id = this->GetWeaponID();

	switch ( id )
	{
		case WEAPON_DEAGLE:
			return strenc( "Desert Eagle" );
		case WEAPON_ELITE:
			return strenc( "Dual Berettas" );
		case WEAPON_FIVESEVEN:
			return strenc( "Five-SeveN" );
		case WEAPON_GLOCK:
			return strenc( "Glock-18" );
		case WEAPON_AK47:
			return strenc( "AK-47" );
		case WEAPON_AUG:
			return strenc( "AUG" );
		case WEAPON_AWP:
			return strenc( "AWP" );
		case WEAPON_FAMAS:
			return strenc( "FAMAS" );
		case WEAPON_G3SG1:
			return strenc( "G3SG1" );
		case WEAPON_GALILAR:
			return strenc( "Galil" );
		case WEAPON_M249:
			return strenc( "M249" );
		case WEAPON_M4A1:
			return strenc( "M4A1" );
		case WEAPON_MAC10:
			return strenc( "MAC-10" );
		case WEAPON_P90:
			return strenc( "P90" );
		case WEAPON_UMP45:
			return strenc( "UMP-45" );
		case WEAPON_XM1014:
			return strenc( "XM1014" );
		case WEAPON_BIZON:
			return strenc( "PP-Bizon" );
		case WEAPON_MAG7:
			return strenc( "MAG-7" );
		case WEAPON_NEGEV:
			return strenc( "Negev" );
		case WEAPON_SAWEDOFF:
			return strenc( "Sawed-Off" );
		case WEAPON_TEC9:
			return strenc( "Tec-9" );
		case WEAPON_TASER:
			return strenc( "Taser" );
		case WEAPON_HKP2000:
			return strenc( "P2000" );
		case WEAPON_MP7:
			return strenc( "MP7" );
		case WEAPON_MP9:
			return strenc( "MP9" );
		case WEAPON_NOVA:
			return strenc( "Nova" );
		case WEAPON_P250:
			return strenc( "P250" );
		case WEAPON_SCAR20:
			return strenc( "SCAR-20" );
		case WEAPON_SG556:
			return strenc( "SG 553" );
		case WEAPON_SSG08:
			return strenc( "SSG 08" );
		case WEAPON_KNIFE:
			return strenc( "Knife" );
		case WEAPON_FLASHBANG:
			return strenc( "Flashbang" );
		case WEAPON_HEGRENADE:
			return strenc( "HE Grenade" );
		case WEAPON_SMOKEGRENADE:
			return strenc( "Smoke Grenade" );
		case WEAPON_MOLOTOV:
			return strenc( "Molotov" );
		case WEAPON_DECOY:
			return strenc( "Decoy" );
		case WEAPON_INCGRENADE:
			return strenc( "Incendiary Grenade" );
		case WEAPON_C4:
			return strenc( "C4" );
		case WEAPON_KNIFE_T:
			return strenc( "Knife" );
		case WEAPON_M4A1_SILENCER:
			return strenc( "M4A1-S" );
		case WEAPON_USP_SILENCER:
			return strenc( "USP-S" );
		case WEAPON_CZ75A:
			return strenc( "CZ75-Auto" );
		case WEAPON_REVOLVER:
			return strenc( "R8 Revolver" );
		default:
			return strenc( "Knife" );
	}
	return "";
}