#pragma once
#include "Scene.h"

#define MAX_FRAME 34
#define ADJUST_FRAME 8

class Scene_Start : public Scene
{
public:
	Scene_Start();
	void Start_Window();

private:
	void menu();

private:
	int user_brocks_pos[3][2];

	//animation
	int frame[2] = { 0, 0 };

	//�w�i
	int Background = 0;
	int Mask_image[MAX_FRAME];
};

