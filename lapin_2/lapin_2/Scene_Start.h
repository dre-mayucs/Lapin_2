#pragma once
#include "Scene.h"
#include "Win_config.h"
#include "Collision.h"

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
	Collision collision;
	Window_config window;

	int user_brocks_pos[3][2];

	//animation
	int frame[2] = { 0, 0 };

	//îwåi
	int Background = 0;
	int Mask_image[MAX_FRAME];

	//sound
	int BGM = LoadSoundMem("Resources\\sounds\\title.mp3");
	int Game_BGM = LoadSoundMem("Resources\\sounds\\game.mp3");
	int Select_BG;
};

