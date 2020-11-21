#pragma once
class IO
{
public:
	void Mouse_Initialize(bool *Mouse_Down, bool *oldMouse_Down, int *MousePos);
	void key_Initialize(char *keys, char *oldkeys);
};

