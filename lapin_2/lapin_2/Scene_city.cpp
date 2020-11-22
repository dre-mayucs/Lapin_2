#include "Dxlib.h"
#include "Scene_city.h"

Scene_city::Scene_city(int brocks[3][2]) {
	//キャラクター
	Background = LoadGraph("Resources\\Background\\background_2.png");
	LoadDivGraph("Resources\\chr\\char1.png", 6, 6, 1, 64, 64, character);
	LoadDivGraph("Resources\\chr\\rabbits_jump.png", 8, 8, 1, 64, 64, character_jump);
	LoadDivGraph("Resources\\chr\\rabbits_fall.png", 8, 8, 1, 64, 64, character_fall);

	//ブロック
	goal = LoadGraph("Resources\\Object\\goal.png");
	LoadDivGraph("Resources\\Object\\manhole-Sheet.png", 4, 4, 1, 64, 64, manhole_block);
	LoadDivGraph("Resources\\Object\\jump_city_Sheet.png", 2, 2, 1, 64, 64, fan_block);
	LoadDivGraph("Resources\\Object\\block_2.png", 2, 2, 1, 64, 64, normal_block);

	for (auto i = 0; i < 3; i++) {
		animation_frame[i][0] = 0;
		animation_frame[i][1] = 0;
	}

	for (auto i = 0; i < 3; i++) {
		User_brock_pos[i][0] = brocks[i][0];
		User_brock_pos[i][1] = brocks[i][1];
	}
}

void Scene_city::City_start() {
	inport.Inport(World, "stage.scene2", &World_value);
	while (true) {
		//IO初期化/前フレームIO情報キャッシュ
		Standard_initialize_IO.key_Initialize(keys, oldkeys);
		Standard_initialize_IO.Mouse_Initialize(&MouseDown, &oldMouseDown, MousePos);
		bool click = keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE];

		//画面クリア
		ClearDrawScreen();

		//背景描画/処理
		DrawBackground();

		//当たり判定処理群
		collision_all();

		//ブロック描画
		DrawBrocks();
		DrawGraph(goal_pos[0] + Worldadjust, goal_pos[1], goal, true);

		//終了処理(ゴール)
		if (collision.box_Fanc(
			static_cast<double>(goal_pos[0]) + Worldadjust + 100, static_cast<double>(goal_pos[0]) + Worldadjust + 208,
			goal_pos[1], static_cast<double>(goal_pos[1]) + 240,
			character_pos_x, static_cast<double>(character_pos_x) + 64, character_pos_y,
			static_cast<double>(character_pos_y) + 64
		)) {
			break;
		}

		//キャラクター動作
		Character_motion();

		//キャラクターアニメーション
		Animation();

		/// <summary>
		/// DEBUG_当たり判定可視化処理
		/// </summary>
		for (auto i = 0; i < World_value; i++) {
			DrawCircle(static_cast<double>(character_pos_x) + 64, static_cast<double>(character_pos_y) + 32, 4,
			           GetColor(0, 0, 0), true);
			DrawCircle(static_cast<double>(character_pos_x) + 64, static_cast<double>(character_pos_y) - 32, 4,
			           GetColor(0, 0, 0), true);
			DrawCircle(static_cast<double>(character_pos_x) + 32, static_cast<double>(character_pos_y) + 64, 4,
			           GetColor(0, 0, 0), true);
			DrawCircle(static_cast<double>(World[i][0]) + Worldadjust + 32, static_cast<double>(World[i][1]) + 32, 32,
			           GetColor(0, 0, 0), true);

			DrawCircle(static_cast<double>(User_brock_pos[0][0]) + 32, static_cast<double>(User_brock_pos[0][1]) + 32,
			           32, GetColor(0, 0, 0), true);
			DrawCircle(static_cast<double>(User_brock_pos[1][0]) + 32, static_cast<double>(User_brock_pos[1][1]) + 32,
			           32, GetColor(0, 0, 0), true);
			DrawCircle(static_cast<double>(User_brock_pos[2][0]) + 32, static_cast<double>(User_brock_pos[2][1]) + 32,
			           32, GetColor(0, 0, 0), true);
		}
		DrawFormatString(200, 50, GetColor(0, 0, 0), "%s\n%s\n%s",
		                 play_flag[0] ? "true" : "false",
		                 play_flag[1] ? "true" : "false",
		                 play_flag[2] ? "true" : "false"
		);

		DrawFormatString(250, 50, GetColor(0, 0, 0), "%s\n%s\n%s",
		                 use_play_flag[0] ? "true" : "false",
		                 use_play_flag[1] ? "true" : "false",
		                 use_play_flag[2] ? "true" : "false"
		);

		ScreenFlip();
		WaitTimer(20);
		if (ProcessMessage() == -1) { break; }
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) { break; }
	}
}

void Scene_city::collision_all() {
	for (auto i = 0; i < World_value; i++) {
		default_collision = collision.Trigonometric_Fanc(
			static_cast<double>(character_pos_x) + 64, static_cast<double>(character_pos_y) + 32, 4,
			static_cast<double>(World[i][0]) + Worldadjust + 32, static_cast<double>(World[i][1]) + 32, 32
		);
		if (default_collision) {
			play_flag[0] = false;
			break;
		}
		play_flag[0] = true;
	}

	for (auto i = 0; i < World_value; i++) {
		top_collision = collision.Trigonometric_Fanc(
			static_cast<double>(character_pos_x) + 64, static_cast<double>(character_pos_y) - 32, 4,
			static_cast<double>(World[i][0]) + Worldadjust + 32, static_cast<double>(World[i][1]) + 32, 32
		);
		if (top_collision) {
			play_flag[1] = false;
			break;
		}
		play_flag[1] = true;
	}

	for (auto i = 0; i < World_value; i++) {
		under_collision = collision.Trigonometric_Fanc(
			static_cast<double>(character_pos_x) + 32, static_cast<double>(character_pos_y) + 64, 4,
			static_cast<double>(World[i][0]) + Worldadjust + 32, static_cast<double>(World[i][1]) + 32, 32
		);
		if (under_collision) {
			play_flag[2] = false;
			break;
		}
		play_flag[2] = true;
	}

	use_play_flag[0] = !collision.Trigonometric_Fanc(
		static_cast<double>(character_pos_x) + 64, static_cast<double>(character_pos_y) + 32, 4,
		static_cast<double>(User_brock_pos[0][0]) + 32, static_cast<double>(User_brock_pos[0][1]) + 32, 32);

	use_play_flag[1] = !collision.Trigonometric_Fanc(
		static_cast<double>(character_pos_x) + 64, static_cast<double>(character_pos_y) + 32, 4,
		static_cast<double>(User_brock_pos[1][0]) + 32, static_cast<double>(User_brock_pos[1][1]) + 32, 32);

	use_play_flag[2] = !collision.Trigonometric_Fanc(
		static_cast<double>(character_pos_x) + 64, static_cast<double>(character_pos_y) + 32, 4,
		static_cast<double>(User_brock_pos[2][0]) + 32, static_cast<double>(User_brock_pos[2][1]) + 32, 32);
}

void Scene_city::DrawBackground() {
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

void Scene_city::DrawBrocks() {
	//既存ワールドデータ描画
	for (auto i = 0; i < World_value; i++) {
		DrawGraph(World[i][0] + Worldadjust, World[i][1], normal_block[0], true); //通常ブロック
	}

	//ユーザー定義ブロック描画
	DrawGraph(User_brock_pos[0][0], User_brock_pos[0][1], manhole_block[0], true); //落下ブロック
	DrawGraph(User_brock_pos[1][0], User_brock_pos[1][1], normal_block[0], true); //通常ブロック
	DrawGraph(User_brock_pos[2][0], User_brock_pos[2][1], fan_block[0], true); //ジャンプブロック
}

void Scene_city::Animation() {
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
		animation_frame[1][1]++;
	}
	else if (animation_flag[2]) {
		DrawGraph(character_pos_x, character_pos_y, character_fall[animation_frame[1][0]], true);
		if (animation_frame[1][0] == 4) {
			character_pos_y += 32;
		}
		if (animation_frame[1][0] == 8) {
			animation_frame[1][0] = 0;
			animation_flag[2] = false;
			character_pos_y += 32;
		}
		if (animation_frame[1][1] == 3) {
			animation_frame[1][0]++;
			animation_frame[1][1] = 0;
		}
		animation_frame[1][1]++;
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

//bool under_collision;
//bool under_front_collision;
//bool gravity_flag = false;
//int brock_collision_num_cache = 0;
void Scene_city::Character_motion() {
	//通常ブロック(ジャンプ)
	if (play_flag[0] == false && play_flag[1] == true) {
		animation_flag[0] = true;
	}

	//落下処理
	if (play_flag[0] == true && play_flag[1] == true && play_flag[2] == true && animation_flag[0] == false &&
		use_play_flag[1] == true && character_pos_y < 325) {
		animation_flag[2] = true;
	}

	//ユーザー定義ブロック(落下)
	if (use_play_flag[0] == false) {
		animation_flag[2] = true;
	}

	//ユーザー定義ブロック(ジャンプ)
	if (use_play_flag[1] == false) {
		animation_flag[0] = true;
	}
}
