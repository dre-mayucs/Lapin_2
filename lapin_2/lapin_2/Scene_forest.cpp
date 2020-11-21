#include "DxLib.h"
#include "Scene_forest.h"

Scene_forest::Scene_forest()
{
	//�L�����N�^�[
	Background = LoadGraph("Resources\\Background\\background.png");
	LoadDivGraph("Resources\\chr\\char1.png", 6, 6, 1, 64, 64, character);
	LoadDivGraph("Resources\\chr\\rabbits_jump.png", 8, 8, 1, 64, 64, character_jump);
	LoadDivGraph("Resources\\chr\\rabbits_fall.png", 8, 8, 1, 64, 64, character_fall);

	LoadDivGraph("Resources\\Object\\defoliation-Sheet.png", 7, 7, 1, 64, 64, defoliation_brock);
	LoadDivGraph("Resources\\Object\\jump-Sheet.png", 5, 5, 1, 64, 64, jump_brock);
	LoadDivGraph("Resources\\Object\\block.png", 2, 2, 1, 64, 64, nomal_block);

	for (auto i = 0; i < 3; i++) {
		animation_frame[i][0] = 0;
		animation_frame[i][1] = 0;
	}
}

void Scene_forest::Forest_start()
{
	inport.Inport(World, "stage.scene", &World_value);
	while (true) {
		//IO������/�O�t���[��IO���L���b�V��
		Standard_initialize_IO.key_Initialize(keys, oldkeys);
		Standard_initialize_IO.Mouse_Initialize(&MouseDown, &oldMouseDown, MousePos);
		bool click = keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE];

		//��ʃN���A
		ClearDrawScreen();

		//�w�i�`��/����
		DrawBackground();

		//�u���b�N�`��
		DrawBrocks();

		//�L�����N�^�[����
		Character_motion();

		//�L�����N�^�[�A�j���[�V����
		Animation();

		ScreenFlip();
		WaitTimer(20);
		if (ProcessMessage() == -1) { break; }
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) { break; }
	}
}

void Scene_forest::DrawBackground()
{
	Worldadjust -= 2;
	for (auto i = 0; i < 2; i++) {
		if (BG_position[0] == -win_config.WIN_WIDTH) {
			BG_position[0] = win_config.WIN_WIDTH;
		}
		if (BG_position[1] == -win_config.WIN_WIDTH) {
			BG_position[1] = win_config.WIN_WIDTH;
		}

		BG_position[0]--;
		BG_position[1]--;

		for (auto brock_x = 0; brock_x < 3; brock_x++) {
			User_brock_pos[brock_x][0]--;
		}
	}
	DrawGraph(BG_position[0], 0, Background, true);
	DrawGraph(BG_position[1], 0, Background, true);
}

void Scene_forest::DrawBrocks()
{
	//�������[���h�f�[�^�`��
	for (auto i = 0; i < World_value; i++) {
		DrawGraph(World[i][0] + Worldadjust, World[i][1], nomal_block[0], true); //�ʏ�u���b�N
	}

	DrawFormatString(100, 100, GetColor(0, 0, 0), "%d", World_value);

	//���[�U�[��`�u���b�N�`��
	DrawGraph(User_brock_pos[0][0], User_brock_pos[0][1], defoliation_brock[0], true); //�����u���b�N
	DrawGraph(User_brock_pos[1][0], User_brock_pos[1][1], nomal_block[0], true); //�ʏ�u���b�N
	DrawGraph(User_brock_pos[2][0], User_brock_pos[2][1], jump_brock[0], true); //�W�����v�u���b�N
}

void Scene_forest::Animation()
{
	if (animation_flag[0]) {
		DrawGraph(character_pos_x, character_pos_y, character_jump[animation_frame[0][0]], true);
		if (animation_frame[0][0] == 8) {
			animation_frame[0][0] = 0;
			animation_flag[0] = false;

			character_pos_y -= 64;
		}
		if (animation_frame[0][1] == 3) {
			animation_frame[0][0]++;
			animation_frame[0][1] = 0;
		}
		animation_frame[0][1]++;
	}
	else if (animation_flag[1]) {
		DrawGraph(character_pos_x, character_pos_y, character_fall[animation_frame[1][0]], true);
		if (animation_frame[1][0] == 8) {
			animation_frame[1][0] = 0;
			animation_flag[1] = false;

			character_pos_y += 64;
		}
		if (animation_frame[1][1] == 3) {
			animation_frame[1][0]++;
			animation_frame[1][1] = 0;
		}
		animation_frame[0][1]++;
	}
	else {
		/*animation_flag[0] = false;
		animation_flag[1] = false;*/

		DrawGraph(character_pos_x, character_pos_y, character[animation_frame[2][0]], true);
		if (animation_frame[2][0] == 6) {
			animation_frame[2][0] = 0;
		}
		if (animation_frame[2][1] == 3) {
			animation_frame[2][0]++;
			animation_frame[2][1] = 0;
		}
		animation_frame[2][1]++;
	}
}

void Scene_forest::Character_motion()
{
	bool center_collision;
	bool oversize_box_collision;
	bool gravity_flag;
	for (auto i = 0; i < World_value; i++) {
		center_collision = collision.Trigonometric_Fanc(
			(double)character_pos_x + 64, (double)character_pos_y + 32, 4,
			(double)World[i][0] + Worldadjust, World[i][1], 32
		);
		oversize_box_collision = collision.Trigonometric_Fanc(
			(double)character_pos_x + 64, (double)character_pos_y + 64, 4,
			(double)World[i][0] + 64, World[i][1], 4
		);

		if (center_collision && animation_flag[0] != true) {
			animation_flag[0] = true;
			/*break;*/
		}
	}
}
