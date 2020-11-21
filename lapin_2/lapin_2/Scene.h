#pragma once
#include "IO.h"
class Scene
{
public:
	IO Standard_initialize_IO;

	int World_value = 0;
	int World[100][2];
	int User_brock_pos[3][2];

	//キーボード入力情報
	char keys[256] = { 0 };
	char oldkeys[256] = { 0 };

	//マウス入力情報
	bool MouseDown = false, oldMouseDown = false;
	int MousePos[2] = { 0 };
	int oldMouse_pos[2] = { 0 };

public:
	Scene();
	~Scene();
};