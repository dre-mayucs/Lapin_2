#pragma once
#include "Scene.h"
#include "World_inport.h"
#include "Win_config.h"
#include "Collision.h"

class Scene_city : public Scene {
public:
	Scene_city(int brocks[3][2]);
	void City_start();

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

	int delay_frame = 0;

	int __frame_cache = 0;

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
	bool animation_flag[4] = { false, false, false, false };

	bool user_brocks_animation_flag_cache[2] = { false , false };
	int user_brocks_animation[2] = { 0, 0 };
	int user_brocks_animation_cache[2] = { 0, 0 };
	int jump_brock_cache = 0;

	bool default_collision = false;
	bool top_collision = false;
	bool under_collision = false;
	bool play_flag[3] = { false, false, false };
	bool use_play_flag[3] = { false, false, false };
	bool break_flag = false;


	//ブロック
	int manhole_block[4];
	int fan_block[2];
	int normal_block2[2];
	int goal = 0;
	int goal_pos[2] = {1500, 150};

	//SE
	int manhole_SE = LoadSoundMem("Resources\\sounds\\manhole.mp3");
	int fan_SE = LoadSoundMem("Resources\\sounds\\fun.mp3");
};
