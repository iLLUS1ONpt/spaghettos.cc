#pragma once

class CAntiAim
{
public:
	void Run( CUserCmd* cmd, bool& sendpacket );
	bool bUpdateLby = false;
	void MovementFix();
private:
	void Fakewalk();
	bool FakeLby();
	void SelectTarget();
	void ChooseAngles();
	void ChooseFake();
	void AntiAims();
	bool IsViable( C_BasePlayer* pPlayer );

	bool*					m_bSendPacket;
	CUserCmd*				m_pCmd;
	C_BasePlayer*			m_pLocalPlayer;
	std::vector<AntiaimData_t>	Entities;
};