#include "DxLib.h"

#include "Scene_forest.h"
#include "World_inport.h"
#include "Win_config.h"

Scene_forest::Scene_forest()
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
}

void Scene_forest::Forest_start()
{
	Window_config win_config;
	World_inport inport;
	inport.Inport(World, "stage.scene", &World_value);
	while (true) {
		//IO初期化/前フレームIO情報キャッシュ
		Standard_initialize_IO.key_Initialize(keys, oldkeys);
		Standard_initialize_IO.Mouse_Initialize(&MouseDown, &oldMouseDown, MousePos);
		bool click = keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE];

		//画面クリア
		ClearDrawScreen();

		for (auto i = 0; i < 2; i++) {
			if (BG_position[0] == -win_config.WIN_WIDTH) {
				BG_position[0] = win_config.WIN_WIDTH;
			}
			if (BG_position[1] == -win_config.WIN_WIDTH) {
				BG_position[1] = win_config.WIN_WIDTH;
			}

			BG_position[0]--;
			BG_position[1]--;

			for (auto brock_x = 0; brock_x < World_value; brock_x++) {
				Worldadjust--;
			}
			for (auto brock_x = 0; brock_x < 3; brock_x++) {
				User_brock_pos[brock_x][0]--;
			}
		}
		DrawGraph(BG_position[0], 0, Background, true);
		DrawGraph(BG_position[1], 0, Background, true);

		//既存ワールドデータ描画
		for (auto i = 0; i < World_value; i++) {
			DrawGraph(World[i][0] + Worldadjust, World[i][1], nomal_block[0], true); //通常ブロック
		}

		DrawFormatString(100, 100, GetColor(0, 0, 0), "%d", World_value);

		//ユーザー定義ブロック描画
		DrawGraph(User_brock_pos[0][0], User_brock_pos[0][1], defoliation_brock[0], true); //落下ブロック
		DrawGraph(User_brock_pos[1][0], User_brock_pos[1][1], nomal_block[0], true); //通常ブロック
		DrawGraph(User_brock_pos[2][0], User_brock_pos[2][1], jump_brock[0], true); //ジャンプブロック

		//キャラクターアニメーション
		Animation();

		ScreenFlip();
		WaitTimer(20);
		if (ProcessMessage() == -1) { break; }
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) { break; }
	}
}

void Scene_forest::Animation()
{
	if (animation_flag[0]) {
		DrawGraph(100, character_pos_y, character_jump[animation_frame[0][0]], true);
		if (animation_frame[0][0] == 8) {
			animation_frame[0][0] = 0;
		}
		if (animation_frame[0][1] == 3) {
			animation_frame[0][0]++;
			animation_frame[0][1] = 0;
		}
		animation_frame[0][1]++;
	}
	else if (animation_flag[1]) {
		DrawGraph(100, character_pos_y, character_fall[animation_frame[1][0]], true);
		if (animation_frame[1][0] == 8) {
			animation_frame[1][0] = 0;
		}
		if (animation_frame[1][1] == 3) {
			animation_frame[1][0]++;
			animation_frame[1][1] = 0;
		}
		animation_frame[0][1]++;
	}
	else {
		DrawGraph(100, character_pos_y, character[animation_frame[2][0]], true);
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
