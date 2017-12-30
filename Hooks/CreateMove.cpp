#include "..\CheatInclude.h"

CreateMoveFn oCreateMove;
bool __stdcall g_hkCreateMove( float flInputSampleTime, CUserCmd* cmd )
{
	bool bSendPacket = true;

	G::InAntiaim = false;
	G::InAimbot = false;

	if ( G::unload )
		return oCreateMove( flInputSampleTime, cmd );

	if ( F::Misc->FinishHalftimeFix( cmd ) )
		return true;

	if ( !cmd->command_number )
		return true;

	static int tickbase = 0;

	F::Misc->SetupHalftimeFix(cmd);
	F::Misc->FixTickbase( tickbase, cmd );
	F::Misc->Radar();
	F::Misc->Bhop( cmd );
	F::Misc->AutoStrafe( cmd );
	F::Misc->CircleStrafer( cmd );
	F::Misc->Fakelag();

	F::Prediction->StartPrediction( tickbase, cmd );
	F::Prediction->EndPrediction();

	F::Aimbot->Run( cmd, bSendPacket );
	F::HvH->Run( cmd );
	F::AntiAim->Run( cmd, bSendPacket );
	F::AntiAim->MovementFix();

	cmd->viewangles.Clamp();

	if (Vars.Other.bThirdpersonMode == 0 && !bSendPacket && !G::InAimbot || !G::InAntiaim)
		F::Thirdperson->m_angle = cmd->viewangles;
	if (Vars.Other.bThirdpersonMode == 1 && bSendPacket && !G::InAimbot || !G::InAntiaim)
		F::Thirdperson->m_angle = cmd->viewangles;

	uintptr_t* framePointer;
	__asm mov framePointer, ebp;
	*reinterpret_cast< bool* > ( *framePointer - 0x1C ) = bSendPacket;

	return false;
}