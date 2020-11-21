#pragma once
#include "Scene.h"

#define BLOCK_X 0
#define BLOCK_Y 1
#define BLOCK_SIZE 64		//�u���b�N�T�C�Y
#define DEFOLIATION_NUM 7	//�����t�u���b�N������
#define JUMP_NUM 5			//�W�����v�u���b�N������
#define NOMAL_NUM 2			//�m�[�}���u���b�N������

class Scene_Build : public Scene
{
public:
	Scene_Build();
	void Build_Window();

private:
	void BG_scroll();
	int collision_block_otherblock(int *mouse_x, int *mouse_y, const int(*pos_tmp)[3], int num);
	bool collision_defoliation_normal();
	bool collision_normal_jump();
	bool collision_jump_defoliation();
	bool collision_defoliation_mouse();
	bool collision_normal_mouse();
	bool collision_jump_mouse();

private:
	int World_count = 0;
	int World_x_adjust = 0;

	//Background image
	int BG_X[2] = { 0, 800 };
	int BG = 0;

	//Brocks
	int defoliation_brock[DEFOLIATION_NUM];
	int jump_brock[JUMP_NUM];
	int nomal_block[NOMAL_NUM];

	//UI
	bool Startbutton_flag = false;
	int UI_image = 0;
	int UI_Buttoniamge[2] = {0, 0};
	int Scroll_arrow = 0;
	int IO_Button[2] = { 0 };
	const int arrowWidth = 32;//z
	const int arrowHeight = 64;//z
	const int UI_Button_pos[2][2] = {
		{25, 300},
		{100, 300}
	};
	int brocks_pos[3][2] =
	{
		{10, 220},
		{10, 135},
		{10, 60}
	};


	//cache
	int blocks_tmp[3][3];
};

