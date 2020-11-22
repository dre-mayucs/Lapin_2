#include "DxLib.h"
#include "Scene_forest.h"

Scene_forest::Scene_forest(int brocks[3][2])
{
	//キャラクター
	Background = LoadGraph("Resources\\Background\\background.png");
	LoadDivGraph("Resources\\chr\\char1.png", 6, 6, 1, 64, 64, character);
	LoadDivGraph("Resources\\chr\\rabbits_jump.png", 8, 8, 1, 64, 64, character_jump);
	LoadDivGraph("Resources\\chr\\rabbits_fall.png", 8, 8, 1, 64, 64, character_fall);

	//ブロック
	goal = LoadGraph("Resources\\Object\\goal.png");
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

bool default_collision;
bool top_collision;
bool under_collision;
bool play_flag[3] = { false, false, false };
bool use_play_flag[3] = { false, false, false };
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

		//終了処理(ゴール)
		if (collision.box_Fanc(
			(double)goal_pos[0] + Worldadjust + 100, (double)goal_pos[0] + Worldadjust + 208, goal_pos[1], (double)goal_pos[1] + 240,
			character_pos_x, (double)character_pos_x + 64, character_pos_y, (double)character_pos_y + 64
		)) {
			DrawGraph(BG_position[0], 0, Background, true);
			DrawGraph(BG_position[1], 0, Background, true);

			//ブロック描画
			DrawBrocks();

			//ゴール描画
			DrawGraph(goal_pos[0] + Worldadjust, goal_pos[1], goal, true);

			//キャラクター動作
			Character_motion();

			//キャラクターアニメーション
			Animation();

			DrawFormatString(character_pos_x + 100, character_pos_y - 100, GetColor(0, 0, 0), "Goal!!!");
			delay_frame++;
			if (delay_frame == 180) { break; }
		}
		else {
			//背景描画/処理
			DrawBackground();

			//当たり判定処理群
			collision_all();

			//ブロック描画
			DrawBrocks();

			//ゴール描画
			DrawGraph(goal_pos[0] + Worldadjust, goal_pos[1], goal, true);

			//キャラクター動作
			Character_motion();

			//キャラクターアニメーション
			Animation();
		}
		/// <summary>
		/// DEBUG_当たり判定可視化処理
		/// </summary>
		//for (auto i = 0; i < World_value; i++) {
		//	DrawCircle((double)character_pos_x + 64, (double)character_pos_y + 32, 4, GetColor(0, 0, 0), true);
		//	DrawCircle((double)character_pos_x + 64, (double)character_pos_y - 32, 4, GetColor(0, 0, 0), true);
		//	/*DrawCircle((double)character_pos_x + 32, (double)character_pos_y + 64, 4, GetColor(0, 0, 0), true);*/
		//	DrawCircle((double)World[i][0] + Worldadjust + 32, (double)World[i][1] + 32, 32, GetColor(0, 0, 0), true);

		//	DrawCircle((double)User_brock_pos[0][0] + 32, (double)User_brock_pos[0][1] + 32, 32, GetColor(0, 0, 0), true);
		//	DrawCircle((double)User_brock_pos[1][0] + 32, (double)User_brock_pos[1][1] + 32, 32, GetColor(0, 0, 0), true);
		//	DrawCircle((double)User_brock_pos[2][0] + 32, (double)User_brock_pos[2][1] + 32, 32, GetColor(0, 0, 0), true);

		//	DrawBox((double)character_pos_x + 30, (double)character_pos_y + 62, (double)character_pos_x + 34, (double)character_pos_y + 66, GetColor(0, 0, 0), true);
		//	DrawBox((double)World[i][0] + Worldadjust, (double)World[i][1], (double)World[i][0] + Worldadjust + 64, (double)World[i][1] + 64, GetColor(0, 0, 0), true);
		//}
		//DrawFormatString(200, 50, GetColor(0, 0, 0), "%s\n%s\n%s", 
		//	play_flag[0] ? "true" : "false",
		//	play_flag[1] ? "true" : "false",
		//	play_flag[2] ? "true" : "false"
		//);

		//DrawFormatString(250, 50, GetColor(0, 0, 0), "%s\n%s\n%s",
		//	use_play_flag[0] ? "true" : "false",
		//	use_play_flag[1] ? "true" : "false",
		//	use_play_flag[2] ? "true" : "false"
		//);

		ScreenFlip();
		WaitTimer(20);
		if (ProcessMessage() == -1) { break; }
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) { break; }
	}
}

void Scene_forest::collision_all()
{
	for (auto i = 0; i < World_value; i++) {
		default_collision = collision.Trigonometric_Fanc(
			(double)character_pos_x + 64, (double)character_pos_y + 32, 4,
			(double)World[i][0] + Worldadjust + 32, (double)World[i][1] + 32, 32
		);
		if (default_collision) {
			play_flag[0] = false;
			break;
		}
		else {
			play_flag[0] = true;
		}
	}

	for (auto i = 0; i < World_value; i++) {
		top_collision = collision.Trigonometric_Fanc(
			(double)character_pos_x + 64, (double)character_pos_y - 32, 4,
			(double)World[i][0] + Worldadjust + 32, (double)World[i][1] + 32, 32
		);
		if (top_collision) {
			play_flag[1] = false;
			break;
		}
		else {
			play_flag[1] = true;
		}
	}

	for (auto i = 0; i < World_value; i++) {
		under_collision = collision.box_Fanc(
			(double)character_pos_x + 30, (double)character_pos_x + 34, (double)character_pos_y + 64, (double)character_pos_y + 75,
			(double)World[i][0] + Worldadjust, (double)World[i][0] + Worldadjust + 64, (double)World[i][1], (double)World[i][1] + 64
		);
		/*under_collision = collision.Trigonometric_Fanc(
			(double)character_pos_x + 32, (double)character_pos_y + 64, 4,
			(double)World[i][0] + Worldadjust + 32, (double)World[i][1] + 32, 32
		);*/
		if (under_collision) {
			play_flag[2] = false;
			break;
		}
		else {
			play_flag[2] = true;
		}
	}

	use_play_flag[0] = !collision.Trigonometric_Fanc(
		(double)character_pos_x + 64, (double)character_pos_y + 32, 4,
		(double)User_brock_pos[0][0]+ 32, (double)User_brock_pos[0][1] + 32, 32
	);

	/*use_play_flag[1] = !collision.box_Fanc(
		(double)character_pos_x + 30, (double)character_pos_x + 34, (double)character_pos_y + 64, (double)character_pos_y + 75,
		User_brock_pos[1][0], (double)User_brock_pos[1][0] + 64, User_brock_pos[1][1], (double)User_brock_pos[1][1] + 64
	);*/
	use_play_flag[1] = !collision.Trigonometric_Fanc(
		(double)character_pos_x + 64, (double)character_pos_y + 32, 4,
		(double)User_brock_pos[1][0] + 32, (double)User_brock_pos[1][1] + 32, 32
	);

	use_play_flag[2] = !collision.Trigonometric_Fanc(
		(double)character_pos_x + 64, (double)character_pos_y + 32, 4,
		(double)User_brock_pos[2][0] + 32, (double)User_brock_pos[2][1] + 32, 32
	);
}

void Scene_forest::DrawBackground()
{
	if (play_flag[0] == true || play_flag[1] == true) {
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
	if (user_brocks_animation_flag_cache[0] == true) {
		if (user_brocks_animation[0] == 6) {
			user_brocks_animation[0] = 0;
			user_brocks_animation_flag_cache[0] = false;
		}
		if (user_brocks_animation_cache[0] == 4) {
			user_brocks_animation_cache[0] = 0;
			user_brocks_animation[0]++;
		}
		user_brocks_animation_cache[0]++;

		DrawGraph(User_brock_pos[0][0], User_brock_pos[0][1], defoliation_brock[user_brocks_animation[0]], true); //落下ブロック
	}
	else {
		DrawGraph(User_brock_pos[0][0], User_brock_pos[0][1], defoliation_brock[0], true); //落下ブロック
	}

	if (user_brocks_animation_flag_cache[1] == true) {
		if (user_brocks_animation[1] == 4) {
			user_brocks_animation[1] = 0;
			user_brocks_animation_flag_cache[1] = false;
		}
		if (user_brocks_animation_cache[1] == 4) {
			user_brocks_animation_cache[1] = 0;
			user_brocks_animation[1]++;
		}
		user_brocks_animation_cache[1]++;

		DrawGraph(User_brock_pos[2][0], User_brock_pos[2][1], jump_brock[user_brocks_animation[1]], true); //ジャンプブロック
	}
	else {
		DrawGraph(User_brock_pos[2][0], User_brock_pos[2][1], jump_brock[0], true); //ジャンプブロック
	}

	DrawGraph(User_brock_pos[1][0], User_brock_pos[1][1], nomal_block[0], true); //通常ブロック
}

bool break_flag = false;
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
	if (animation_flag[1]) {
		DrawGraph(character_pos_x, character_pos_y, character_jump[animation_frame[1][0]], true);
		if (animation_frame[1][0] == 8 && __frame_cache == 0) {
			character_pos_y -= 64 * 2;
			animation_frame[1][0] = 0;
			__frame_cache = 1;
		}
		else if (__frame_cache != 0) {
			if (__frame_cache >= 45) {
				animation_flag[1] = false;
				__frame_cache = 0;
			}
			else { __frame_cache++; }
		}
		if (animation_frame[1][1] == 3) {
			animation_frame[1][0]++;
			animation_frame[1][1] = 0;
		}
		animation_frame[1][1]++;
	}
	if (animation_flag[2]) {
		DrawGraph(character_pos_x, character_pos_y, character_fall[animation_frame[2][0]], true);
		if (animation_frame[2][0] == 8) {
			animation_frame[2][0] = 0;
			animation_flag[2] = false;

			while (true) {
				for (auto i = 0; i < World_value; i++) {
					if (collision.box_Fanc(
						(double)character_pos_x + 30, (double)character_pos_x + 34, (double)character_pos_y + 64, (double)character_pos_y + 75,
						(double)World[i][0] + Worldadjust, (double)World[i][0] + Worldadjust + 64, (double)World[i][1], (double)World[i][1] + 64
					)) {
						break_flag = true;
						break;
					}
				}
				if (character_pos_y >= 325 || break_flag == true) { break_flag = false; break; }
				character_pos_y++;
			}
		}
		if (animation_frame[2][1] == 3) {
			animation_frame[2][0]++;
			animation_frame[2][1] = 0;
		}
		animation_frame[2][1]++;
	}
	if (animation_flag[3]) {
		DrawGraph(character_pos_x, character_pos_y, character_fall[animation_frame[2][0]], true);
		if (animation_frame[2][0] == 8) {
			animation_frame[2][0] = 0;
			animation_flag[3] = false;
			character_pos_y += 128;
		}
		if (animation_frame[2][1] == 3) {
			animation_frame[2][0]++;
			animation_frame[2][1] = 0;
		}
		animation_frame[2][1]++;
	}
	if(!animation_flag[0] && !animation_flag[1] && !animation_flag[2] && !animation_flag[3]) {
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
	bool cache = collision.box_Fanc(
		(double)character_pos_x + 30, (double)character_pos_x + 34, (double)character_pos_y + 64, (double)character_pos_y + 75,
		User_brock_pos[1][0], (double)User_brock_pos[1][0] + 64, User_brock_pos[1][1], (double)User_brock_pos[1][1] + 64
	);

	//通常ブロック(ジャンプ)
	if (play_flag[0] == false && play_flag[1] == true) {
		animation_flag[0] = true;
	}

	//落下処理
	if (play_flag[0] == true && /*play_flag[1] == true &&*/ play_flag[2] == true && animation_flag[0] == false && 
		character_pos_y < 325 && animation_flag[2] != true && cache == false && animation_flag[1] == false && animation_flag[2] == false) {
		animation_flag[2] = true;
	}

	//ユーザー定義ブロック(落下)
	if (use_play_flag[0] == false && use_play_flag[1] == true && use_play_flag[2] == true) {
		user_brocks_animation_flag_cache[0] = true;
		animation_flag[3] = true;
		PlaySoundMem(defo_SE, DX_PLAYTYPE_BACK);
	}

	//ユーザー定義ブロック(ノーマル)
	if (use_play_flag[0] == true && use_play_flag[1] == false && use_play_flag[2] == true) {
		animation_flag[0] = true;
	}

	//ユーザー定義ブロック(ジャンプ)
	if (use_play_flag[0] == true && use_play_flag[1] == true && use_play_flag[2] == false) {
		user_brocks_animation_flag_cache[1] = true;
		animation_flag[1] = true;
		PlaySoundMem(mush_SE, DX_PLAYTYPE_BACK);
	}
}