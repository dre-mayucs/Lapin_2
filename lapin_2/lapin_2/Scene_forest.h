#pragma once
#include "Scene.h"
#include "World_inport.h"
#include "Win_config.h"
#include "Collision.h"

class Scene_forest : public Scene
{
public:
	Scene_forest(int brocks[3][2]);
	void Forest_start();

private:
	void collision_all();
	void DrawBackground();
	void DrawBrocks();
	void Animation();
	void Character_motion();

private:
	Window_config win_config;
	World_inport inport;
	Collision collision;

	int Worldadjust = 0;
	int adjust_cache = 0;
	int Background = 0;
	int BG_position[2] = { 0, 800 };

	//キャラクター
	int character_pos_x = 100;
	int character_pos_y = 325;
	int character[6];
	int character_jump[8];
	int character_fall[8];
	int animation_frame[3][2];
	bool animation_flag[3] = { false, false, false };
	int jump_brock_cache = 0;


	//ブロック
	int defoliation_brock[7];
	int jump_brock[5];
	int nomal_block[2];
	int goal = 0;
	int goal_pos[2] = {1500, 150};
};

