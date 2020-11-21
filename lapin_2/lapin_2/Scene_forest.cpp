#include "DxLib.h"
#include "Scene_forest.h"

Scene_forest::Scene_forest(int brocks[3][2])
{
	//キャラクター
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

	for (auto i = 0; i < 3; i++) {
		User_brock_pos[i][0] = brocks[i][0];
		User_brock_pos[i][1] = brocks[i][1];
	}
}

void Scene_forest::Forest_start()
{
	inport.Inport(World, "stage.scene", &World_value);
	while (true) {
		//IO初期化/前フレームIO情報キャッシュ
		Standard_initialize_IO.key_Initialize(keys, oldkeys);
		Standard_initialize_IO.Mouse_Initialize(&MouseDown, &oldMouseDown, MousePos);
		bool click = keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE];

		//画面クリア
		ClearDrawScreen();

		//背景描画/処理
		DrawBackground();

		//ブロック描画
		DrawBrocks();

		//キャラクター動作
		Character_motion();

		//キャラクターアニメーション
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
	//既存ワールドデータ描画
	for (auto i = 0; i < World_value; i++) {
		DrawGraph(World[i][0] + Worldadjust, World[i][1], nomal_block[0], true); //通常ブロック
	}

	//ユーザー定義ブロック描画
	DrawGraph(User_brock_pos[0][0], User_brock_pos[0][1], defoliation_brock[0], true); //落下ブロック
	DrawGraph(User_brock_pos[1][0], User_brock_pos[1][1], nomal_block[0], true); //通常ブロック
	DrawGraph(User_brock_pos[2][0], User_brock_pos[2][1], jump_brock[0], true); //ジャンプブロック
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

			while (character_pos_y <= 325) {
				character_pos_y++;
			}
		}
		if (animation_frame[1][1] == 3) {
			animation_frame[1][0]++;
			animation_frame[1][1] = 0;
		}
		animation_frame[0][1]++;
	}
	else if (animation_flag[2]) {
		DrawGraph(character_pos_x, character_pos_y, character_fall[animation_frame[1][0]], true);
		if (animation_frame[1][0] == 8) {
			animation_frame[1][0] = 0;
			animation_flag[2] = false;

			character_pos_y += 128;
		}
		if (animation_frame[1][1] == 3) {
			animation_frame[1][0]++;
			animation_frame[1][1] = 0;
		}
		animation_frame[0][1]++;
	}
	else {
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

bool jump_flag = false;
void Scene_forest::Character_motion()
{
	bool center_collision;
	bool under_collision;
	for (auto i = 0; i < World_value; i++) {
		center_collision = collision.Trigonometric_Fanc(
			(double)character_pos_x + 64, (double)character_pos_y + 32, 4,
			(double)World[i][0] + Worldadjust + 32, (double)World[i][1] + 32, 32
		);
		if (center_collision && animation_flag[0] != true) {
			animation_flag[0] = true;
			break;
		}

		under_collision = collision.Trigonometric_Fanc(
			(double)character_pos_x + 32, (double)character_pos_y + 70, 4,
			(double)World[i][0] + Worldadjust + 32, (double)World[i][1] + 32, 32
		);
		if (under_collision == false && character_pos_y <= 325 - 64) {
			character_pos_y++;
		}

		DrawCircle((double)character_pos_x + 32, (double)character_pos_y + 96, 4, GetColor(0, 0, 0), true);
		DrawCircle((double)World[i][0] + Worldadjust + 32, (double)World[i][1] + 32, 32, GetColor(0, 0, 0), true);
	}

	center_collision = collision.Trigonometric_Fanc(
		(double)character_pos_x + 64, (double)character_pos_y + 60, 4,
		(double)User_brock_pos[0][0] + 32, (double)User_brock_pos[0][1] + 32, 32
	);
	if (center_collision) {
		animation_flag[1] = true;
	}

	center_collision = collision.Trigonometric_Fanc(
		(double)character_pos_x + 64, (double)character_pos_y + 32, 4,
		(double)User_brock_pos[1][0] + 32, (double)User_brock_pos[1][1] + 32, 32
	);
	if (center_collision) {
		animation_flag[0] = true;
	}

	center_collision = collision.Trigonometric_Fanc(
		(double)character_pos_x + 64, (double)character_pos_y + 32, 4,
		(double)User_brock_pos[2][0] + 32, (double)User_brock_pos[2][1] + 32, 32
	);
	if (center_collision) {
		animation_flag[2] = true;
	}
}
