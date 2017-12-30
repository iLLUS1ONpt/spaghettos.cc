#pragma once

class COffsets
{
public:
	void GrabOffsets();
	//sigs
	uintptr_t d3d9Device;
	uintptr_t dwCAM_Think;
};

extern COffsets* Offsets;