#pragma once
#include "Scene.h"
class Scene_forest : public Scene
{
public:
	Scene_forest();
	void Forest_start();

private:
	void Animation();

private:
	int Worldadjust = 0;
	int Background = 0;
	int BG_position[2] = { 0, 800 };

	//キャラクターint 
	int character_pos_y = 325;
	int character[6];
	int character_jump[8];
	int character_fall[8];
	int animation_frame[3][2];
	bool animation_flag[2] = { false, false };

	//ブロック
	int defoliation_brock[7];
	int jump_brock[5];
	int nomal_block[2];
};

