#include "MainInclude.h"

HMODULE				G::Dll;
HWND				G::Window;
bool				G::PressedKeys[ 256 ] = {};
bool				G::d3dinit			= false;
bool				G::unload			= false;
bool				G::InAimbot			= false;
bool				G::InAntiaim		= false;
bool				G::InAutowall		= false;
bool				G::CanShoot			= false;
bool				G::InChams			= false;
C_CSGameRulesProxy** G::GameRules		= nullptr;
C_BasePlayer*		G::LocalPlayer = nullptr;
CUserCmd*			G::pCmd = nullptr;
using msg_t = void( __cdecl* )( const char*, ... );
msg_t				G::Msg = reinterpret_cast< msg_t >( GetProcAddress( GetModuleHandleA( "tier0.dll" ), "Msg" ) );

CSkinChanger*		F::SkinChanger		= new CSkinChanger();
CVisuals*			F::Visuals			= new CVisuals();
CThirdperson*		F::Thirdperson		= new CThirdperson();
CMisc*				F::Misc				= new CMisc();
CChams*				F::Chams			= new CChams();
CMaterialConfig*	F::MaterialConfig	= new CMaterialConfig();
CDlight*			F::Dlight			= new CDlight();
CPrediction*		F::Prediction		= new CPrediction();
CAnimations*		F::Animations		= new CAnimations();
CHvH*				F::HvH				= new CHvH();
CGrenadePred*		F::GrenadePrediction = new CGrenadePred();
CBulletPenetration*	F::BulletPenetration = new CBulletPenetration();
CAntiAim*			F::AntiAim			= new CAntiAim();
CAimbot*			F::Aimbot			= new CAimbot();
CResolver*			F::Resolver			= new CResolver();
CBacktrack*			F::Backtrack		= new CBacktrack();

IMaterial*			M::visible;
IMaterial*			M::hidden;
IMaterial*			M::wireframed;
IMaterial*			M::wireframed_hidden;
IMaterial*			M::gold;
IMaterial*			M::platinum;
IMaterial*			M::glass;
IMaterial*			M::crystal;
IMaterial*			M::chrome;
IMaterial*			M::glow;
IMaterial*			M::gloss;

const std::map<size_t, Item_t> k_weapon_info =
{
	{ WEAPON_KNIFE, { "models/weapons/v_knife_default_ct.mdl", "knife_default_ct" } },
	{ WEAPON_KNIFE_T, { "models/weapons/v_knife_default_t.mdl", "knife_t" } },
	{ WEAPON_KNIFE_BAYONET, { "models/weapons/v_knife_bayonet.mdl", "bayonet" } },
	{ WEAPON_KNIFE_FLIP, { "models/weapons/v_knife_flip.mdl", "knife_flip" } },
	{ WEAPON_KNIFE_GUT, { "models/weapons/v_knife_gut.mdl", "knife_gut" } },
	{ WEAPON_KNIFE_KARAMBIT, { "models/weapons/v_knife_karam.mdl", "knife_karambit" } },
	{ WEAPON_KNIFE_M9_BAYONET, { "models/weapons/v_knife_m9_bay.mdl", "knife_m9_bayonet" } },
	{ WEAPON_KNIFE_TACTICAL, { "models/weapons/v_knife_tactical.mdl", "knife_tactical" } },
	{ WEAPON_KNIFE_FALCHION, { "models/weapons/v_knife_falchion_advanced.mdl", "knife_falchion" } },
	{ WEAPON_KNIFE_SURVIVAL_BOWIE, { "models/weapons/v_knife_survival_bowie.mdl", "knife_survival_bowie" } },
	{ WEAPON_KNIFE_BUTTERFLY, { "models/weapons/v_knife_butterfly.mdl", "knife_butterfly" } },
	{ WEAPON_KNIFE_PUSH, { "models/weapons/v_knife_push.mdl", "knife_push" } },
	{ GLOVE_STUDDED_BLOODHOUND, { "models/weapons/v_models/arms/glove_bloodhound/v_glove_bloodhound.mdl" } },
	{ GLOVE_T_SIDE, { "models/weapons/v_models/arms/glove_fingerless/v_glove_fingerless.mdl" } },
	{ GLOVE_CT_SIDE, { "models/weapons/v_models/arms/glove_hardknuckle/v_glove_hardknuckle.mdl" } },
	{ GLOVE_SPORTY, { "models/weapons/v_models/arms/glove_sporty/v_glove_sporty.mdl" } },
	{ GLOVE_SLICK, { "models/weapons/v_models/arms/glove_slick/v_glove_slick.mdl" } },
	{ GLOVE_LEATHER_WRAP, { "models/weapons/v_models/arms/glove_handwrap_leathery/v_glove_handwrap_leathery.mdl" } },
	{ GLOVE_MOTORCYCLE, { "models/weapons/v_models/arms/glove_motorcycle/v_glove_motorcycle.mdl" } },
	{ GLOVE_SPECIALIST, { "models/weapons/v_models/arms/glove_specialist/v_glove_specialist.mdl" } }
};

const std::vector<WeaponName_t> k_knife_names =
{
	{ 0, "Default" },
	{ WEAPON_KNIFE_BAYONET, "Bayonet" },
	{ WEAPON_KNIFE_FLIP, "Flip Knife" },
	{ WEAPON_KNIFE_GUT, "Gut Knife" },
	{ WEAPON_KNIFE_KARAMBIT, "Karambit" },
	{ WEAPON_KNIFE_M9_BAYONET, "M9 Bayonet" },
	{ WEAPON_KNIFE_TACTICAL, "Huntsman Knife" },
	{ WEAPON_KNIFE_FALCHION, "Falchion Knife" },
	{ WEAPON_KNIFE_SURVIVAL_BOWIE, "Bowie Knife" },
	{ WEAPON_KNIFE_BUTTERFLY, "Butterfly Knife" },
	{ WEAPON_KNIFE_PUSH, "Shadow Daggers" }
};

const std::vector<WeaponName_t> k_glove_names =
{
	{ 0, "Default" },
	{ GLOVE_STUDDED_BLOODHOUND, "Bloodhound" },
	{ GLOVE_T_SIDE, "Default (Terrorists)" },
	{ GLOVE_CT_SIDE, "Default (Counter-Terrorists)" },
	{ GLOVE_SPORTY, "Sporty" },
	{ GLOVE_SLICK, "Slick" },
	{ GLOVE_LEATHER_WRAP, "Handwrap" },
	{ GLOVE_MOTORCYCLE, "Motorcycle" },
	{ GLOVE_SPECIALIST, "Specialist" }
};

const std::vector<WeaponName_t> k_weapon_names =
{
	{ WEAPON_KNIFE, "Knife" },
	{ GLOVE_T_SIDE, "Glove" },
	{ 7, "AK-47" },
	{ 8, "AUG" },
	{ 9, "AWP" },
	{ 63, "CZ-75" },
	{ 1, "Desert Eagle" },
	{ 2, "Dual Berettas" },
	{ 10, "FAMAS" },
	{ 3, "Five-SeveN" },
	{ 11, "G3SG1" },
	{ 13, "Galil AR" },
	{ 4, "Glock-18" },
	{ 14, "M249" },
	{ 60, "M4A1-S" },
	{ 16, "M4A4" },
	{ 17, "MAC-10" },
	{ 27, "MAG-7" },
	{ 33, "MP7" },
	{ 34, "MP9" },
	{ 28, "Negev" },
	{ 35, "Nova" },
	{ 32, "P2000" },
	{ 36, "P250" },
	{ 19, "P90" },
	{ 26, "PP-Bizon" },
	{ 64, "R8 Revolver" },
	{ 29, "Sawed-Off" },
	{ 38, "SCAR-20" },
	{ 40, "SSG 08" },
	{ 39, "SG-553" },
	{ 30, "Tec-9" },
	{ 24, "UMP-45" },
	{ 61, "USP-S" },
	{ 25, "XM1014" },
};

const std::vector<QualityName_t> k_quality_names =
{
	{ 0, "Default" },
	{ 1, "Genuine" },
	{ 2, "Vintage" },
	{ 3, "Unusual" },
	{ 5, "Community" },
	{ 6, "Developer" },
	{ 7, "Self-Made" },
	{ 8, "Customized" },
	{ 9, "Strange" },
	{ 10, "Completed" },
	{ 12, "Tournament" }
};