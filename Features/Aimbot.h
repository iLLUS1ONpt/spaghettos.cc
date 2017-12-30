#pragma once

class CAimbot
{
public:
	void						Run( CUserCmd* cmd, bool& sendpacket );
private:
	bool						CanShoot();
	void						SelectTarget();
	void						ChooseAngles();
	bool						IsViable( C_BasePlayer* pPlayer );
	bool*						m_bSendPacket;
	CUserCmd*					m_pCmd;
	C_BasePlayer*				m_pLocalPlayer;
	C_BaseEntity*				pEntity;
	std::vector<AimbotData_t>	Entities;
};