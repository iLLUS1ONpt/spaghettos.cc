#pragma once

class IBaseClientDll
{
public:
	ClientClass* GetAllClasses();
	void CreateMove( int sequence_number, float frametime, bool active );
};