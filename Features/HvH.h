#pragma once

class CHvH
{
public:
	void Run( CUserCmd* cmd );
	void NoVisRecoil( CViewSetup& vsView );
private:
	void NoRecoil();

	
	
	CUserCmd*		m_pCmd;
	C_BasePlayer*	m_pLocalPlayer;
};
