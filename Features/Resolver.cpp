#include "..\CheatInclude.h"
#include "..\Math.h"

C_BasePlayer* pEnt;

void CResolver::Run(ClientFrameStage_t stage)
{
	if (Vars.HvH.Aimbot.bResolver)
	{
		for (auto i = 0; i < g_pClientEntList->GetHighestEntityIndex(); i++)
		{
			if (!pEnt) continue;

			if (pEnt->IsDormant()) continue;

			player_info_t pTemp;

			if (!g_pEngine->GetPlayerInfo(i, &pTemp))
				continue;

			if (!pEnt->IsEnemy() == pEnt->GetTeam()) continue;

			auto FYaw = pEnt->GetLowerBodyYawTarget();
			auto pitch = pEnt->GetEyeAngles().x;

			pEnt->GetEyeAngles().y = FYaw;
			float PlayerIsMoving = abs(pEnt->GetVelocity().Length2D());
			bool bLowerBodyUpdated = false;
			bool IsBreakingLBY = false;

			bool isFakeHeading = false;

			float oldLBY = pEnt->GetLowerBodyYawTarget();

			if (oldLBY != pEnt->GetLowerBodyYawTarget())
			{
				bLowerBodyUpdated = true;
			}
			else
			{
				bLowerBodyUpdated = false;
			}

			if (pEnt->GetEyeAngles().y - pEnt->GetLowerBodyYawTarget() > 35)
			{
				isFakeHeading = true;
			}
			else
			{
				isFakeHeading = false;
			}

			float bodyeyedelta = fabs(pEnt->GetEyeAngles().y - pEnt->GetLowerBodyYawTarget());
			if (PlayerIsMoving || bLowerBodyUpdated)
			{
				G::resolvemode = 3;

				pEnt->GetEyeAngles().y = FYaw;
				oldLBY = pEnt->GetEyeAngles().y;

				IsBreakingLBY = false;
			}
			else
			{
				G::resolvemode = 1;

				if (bodyeyedelta == 0.f && pEnt->GetVelocity().Length2D() >= 0 && pEnt->GetVelocity().Length2D() < 36)
				{
					pEnt->GetEyeAngles().y = oldLBY;
					IsBreakingLBY = true;
				}
				else
				{
					IsBreakingLBY = false;
				}
			}

			if (IsBreakingLBY)
			{
				G::resolvemode = 2;

				pEnt->GetEyeAngles().y = oldLBY;

				switch (G::Shots % 3)
				{
				case 1: pEnt->GetEyeAngles().y = 180; break;
				case 2: pEnt->GetEyeAngles().y = pEnt->GetLowerBodyYawTarget() + rand() % 180;
				case 3: pEnt->GetEyeAngles().y = pEnt->GetLowerBodyYawTarget() + pEnt->GetEyeAngles().y + rand() % 35;
				}
			}
			else if (!IsBreakingLBY && !isFakeHeading)
			{
				G::resolvemode = 3;

				pEnt->GetEyeAngles().y = pEnt->GetLowerBodyYawTarget();

				switch (G::Shots % 4)
				{
				case 1: pEnt->GetEyeAngles().y = 45 + rand() % 180;
				case 2: pEnt->GetEyeAngles().y = oldLBY + rand() % 90;
				case 3: pEnt->GetEyeAngles().y = 180 + rand() % 90;
				case 4: pEnt->GetEyeAngles().y = oldLBY + pEnt->GetEyeAngles().y + rand() % 45;
				}
			}
			else if (isFakeHeading)
			{
				G::resolvemode = 2;

				pEnt->GetEyeAngles().y = pEnt->GetLowerBodyYawTarget() - pEnt->GetEyeAngles().y;

				switch (G::Shots % 2)
				{
				case 1: pEnt->GetEyeAngles().y = pEnt->GetLowerBodyYawTarget() + rand() % 90; break;
				case 2: pEnt->GetEyeAngles().y = pEnt->GetEyeAngles().y + rand() % 360; break;
				}
			}
			else if (G::Shots >= Vars.HvH.Aimbot.bBFAfter && Vars.HvH.Aimbot.bBFAfter != 0)
			{
				G::resolvemode = 2;

				pEnt->GetEyeAngles().y = 180;

				switch (G::Shots % 4)
				{
				case 1: pEnt->GetEyeAngles().y = pEnt->GetLowerBodyYawTarget(); break;
				case 2: pEnt->GetEyeAngles().y = pEnt->GetLowerBodyYawTarget() + rand() % 90; break;
				case 3: pEnt->GetEyeAngles().y = pEnt->GetLowerBodyYawTarget() + rand() % 180; break;
				case 4: pEnt->GetEyeAngles().y = oldLBY + rand() % 45; break;
				}
			}
			else
			{
				G::resolvemode = 1;

				pEnt->GetEyeAngles().y = pEnt->GetLowerBodyYawTarget() + rand() % 180;

				switch (G::Shots % 13)
				{
				case 1: pEnt->GetEyeAngles().y = 180; break;
				case 2: pEnt->GetEyeAngles().y = pEnt->GetLowerBodyYawTarget() + rand() % 180;
				case 3: pEnt->GetEyeAngles().y = pEnt->GetLowerBodyYawTarget() + pEnt->GetEyeAngles().y + rand() % 35;
				case 4: pEnt->GetEyeAngles().y = 45 + rand() % 180;
				case 5: pEnt->GetEyeAngles().y = oldLBY + rand() % 90;
				case 6: pEnt->GetEyeAngles().y = 180 + rand() % 90;
				case 7: pEnt->GetEyeAngles().y = oldLBY + pEnt->GetEyeAngles().y + rand() % 45;
				case 8: pEnt->GetEyeAngles().y = pEnt->GetLowerBodyYawTarget(); break;
				case 9: pEnt->GetEyeAngles().y = pEnt->GetLowerBodyYawTarget() + rand() % 90; break;
				case 10: pEnt->GetEyeAngles().y = pEnt->GetLowerBodyYawTarget() + rand() % 180; break;
				case 11: pEnt->GetEyeAngles().y = oldLBY + rand() % 45; break;
				case 12: pEnt->GetEyeAngles().y = pEnt->GetLowerBodyYawTarget() + rand() % 90; break;
				case 13: pEnt->GetEyeAngles().y = pEnt->GetEyeAngles().y + rand() % 360; break;
				}
			}
		}
	}
}