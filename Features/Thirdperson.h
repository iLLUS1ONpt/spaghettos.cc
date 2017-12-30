#pragma once

class CThirdperson
{
public:
	void			Run( ClientFrameStage_t stage );
	void			Run( CViewSetup & setup );
	QAngle			m_angle = QAngle( 0, 0, 0 );
private:
};