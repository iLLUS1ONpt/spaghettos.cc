#pragma once

enum CustomMaterials : int
{
	CMAT_NODRAW = 0,
	CMAT_WIREFRAME,
	CMAT_GOLD,
	CMAT_PLATINUM,
	CMAT_GLASS,
	CMAT_CRYSTAL,
	CMAT_CHROME,
	CMAT_GLOW,
	CMAT_GLOSS
};

class CChams
{
public:
	CChams();
	~CChams();
	void Run();
	bool Run( const ModelRenderInfo_t & info );
	QAngle m_angReal = QAngle( 0, 0, 0 );
private:
	void DrawPlayers();
	void DrawAngles();
	void NightMode();
	void AsusWalls();
	void ForceMaterial( IMaterial* material, Color color );

	C_BasePlayer* m_pLocalPlayer;
	std::vector<std::pair<C_BasePlayer*, float>>	Entities;
};