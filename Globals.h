#pragma once

#include "CheatInclude.h"
#include "Features\Features.h"
#include <unordered_map>

namespace G
{
	extern HMODULE		Dll;
	extern HWND			Window;
	extern bool			PressedKeys[ 256 ];
	extern bool			d3dinit;
	extern bool			unload;
	extern bool			InAimbot;
	extern bool			InAntiaim;
	extern bool			InAutowall;
	extern bool			CanShoot;
	extern bool			InChams;
	static int			resolvemode = 1;
	static int			Shots;
	extern C_CSGameRulesProxy** GameRules;
	extern C_BasePlayer* LocalPlayer;
	extern CUserCmd* pCmd;
	using msg_t = void( __cdecl* )( const char*, ... );
	extern msg_t		Msg;
}

namespace F
{
	extern CSkinChanger*	SkinChanger;
	extern CVisuals*		Visuals;
	extern CThirdperson*	Thirdperson;
	extern CMisc*			Misc;
	extern CChams*			Chams;
	extern CMaterialConfig*	MaterialConfig;
	extern CDlight*			Dlight;
	extern CPrediction*		Prediction;
	extern CAnimations*		Animations;
	extern CHvH*			HvH;
	extern CGrenadePred*	GrenadePrediction;
	extern CBulletPenetration*	BulletPenetration;
	extern CAntiAim*		AntiAim;
	extern CAimbot*			Aimbot;
	extern CResolver*		Resolver;
	extern CBacktrack*		Backtrack;
}

namespace M
{
	extern IMaterial*		visible;
	extern IMaterial*		hidden;
	extern IMaterial*		wireframed;
	extern IMaterial*		wireframed_hidden;
	extern IMaterial*		gold;
	extern IMaterial*		platinum;
	extern IMaterial*		glass;
	extern IMaterial*		crystal;
	extern IMaterial*		chrome;
	extern IMaterial*		glow;
	extern IMaterial*		gloss;
}