#pragma once

class CPrediction
{
public:
	void StartPrediction( int & tickbase, CUserCmd * cmd );
	void EndPrediction();

private:
	float m_flOldCurtime;
	float m_flOldFrametime;
	CMoveData m_MoveData;

	int* m_pPredictionRandomSeed;
	C_BasePlayer* pLocalPlayer;
};