#pragma once

class CVisuals
{
public:
	~CVisuals() { Entities.clear(); Impacts.clear(); };
	void Run();
	void SetupListener();
	void RemoveListener();
	void BulletTracers();
private:
	void PlayerESP( int index );
	void Skeleton( C_BasePlayer * pPlayer, Color col );
	void HealthBar( const int32_t x, const int32_t y, const int32_t w, const int32_t h, int health, float alpha );
	void Info( const int32_t x, const int32_t y, const int32_t w, const int32_t h, C_BasePlayer * pPlayer, Color col );
	void CustomCrosshair();
	void Hitmarker();
	void LBYIndicator();
	void CapsuleOverlay( int index, Color col, float duration );
	void SetHitmarkerValues( float hurttime, int health, int victim );
	void SpreadCircle();
	void DrawAngles();

	float	flHurtTime;
	int		iHealth;
	int		iVictim;

	C_BasePlayer* m_pLocalPlayer;
	CUserCmd* m_pCmd;

	class EventListener : public IGameEventListener2
	{
	public:
		EventListener()
		{
			con_filter_text_out = g_pCvar->FindVar( "con_filter_text_out" );
			con_filter_text = g_pCvar->FindVar( "con_filter_text" );
			g_pGameEventManager->AddListener( this, "player_hurt", false );
			g_pGameEventManager->AddListener( this, "bullet_impact", false );
			g_pGameEventManager->AddListener( this, "item_purchase", false );
		};
		~EventListener() { g_pGameEventManager->RemoveListener( this ); };
		virtual void	FireGameEvent( IGameEvent* pEvent );
		int				GetEventDebugID() override { return 0x2A; };
	private:
		ConVar* con_filter_text_out;
		ConVar* con_filter_text;
	};

	EventListener*	pEventListener = nullptr;

public:
	VisualsInfo_t									PlayerList[ 63 ];
private:
	std::vector<std::pair<int, float>>				Entities;
	std::deque<BulletImpact_t>						Impacts;
};