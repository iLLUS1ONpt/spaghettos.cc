#pragma once

class CBaseHudChat
{
public:
	void ChatPrintf( int iPlayerIndex, int iFilter, const char* fmt, ... );

};

class IClientModeShared
{
public:
	// Message mode handling
	// All modes share a common chat interface
	char pad[ 28 ];
	CBaseHudChat			*m_pChatElement;
};
