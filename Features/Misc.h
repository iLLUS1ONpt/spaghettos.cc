#pragma once

class CMisc
{
public:
	void Radar();
	bool FinishHalftimeFix( CUserCmd* cmd );
	void SetupHalftimeFix( CUserCmd* cmd );
	void FixTickbase( int & tickbase, CUserCmd * cmd );
	void Bhop( CUserCmd * cmd );
	void AutoStrafe( CUserCmd * cmd );
	void CircleStrafer(CUserCmd * cmd);
	void SetName( const char * name );
	void Fakelag();
private:
	bool m_bWasFrozen;
	QAngle m_oldangles;
	CUserCmd* m_pOldCmd;
	C_BasePlayer* m_pLocalPlayer;
	int m_nTickBase;
	bool bSendPacket = true;
};