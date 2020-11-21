#include "DxLib.h"

#include "Scene_Build.h"
#include "IO.h"
#include "Win_config.h"
#include "Collision.h"
#include "World_inport.h"

Scene_Build::Scene_Build()
{
	//ワールド読み込み
	World_inport inport;
	inport.Inport(World, "scene.scene", &World_value);

	//素材定義
	BG = LoadGraph("Resources\\Background\\background.png");

	LoadDivGraph("Resources\\Object\\defoliation-Sheet.png", DEFOLIATION_NUM, DEFOLIATION_NUM, 1, BLOCK_SIZE, BLOCK_SIZE, defoliation_brock);
	LoadDivGraph("Resources\\Object\\jump-Sheet.png", JUMP_NUM, JUMP_NUM, 1, BLOCK_SIZE, BLOCK_SIZE, jump_brock);
	LoadDivGraph("Resources\\Object\\block.png", NOMAL_NUM, NOMAL_NUM, 1, BLOCK_SIZE, BLOCK_SIZE, nomal_block);

	UI_image = LoadGraph("Resources\\UI\\bar.png");
	Scroll_arrow = LoadGraph("Resources\\UI\\scroll.png");

	UI_Buttoniamge[0] = LoadGraph("Resources\\UI\\start1.png");
	UI_Buttoniamge[1] = LoadGraph("Resources\\UI\\stop1.png");

	for (int i = 0; i < 3; i++) {
		blocks_tmp[i][0] = brocks_pos[i][0];
		blocks_tmp[i][1] = brocks_pos[i][1];
	}
}

void Scene_Build::Build_Window()
{
	World_inport inport;
	Window_config Win_config;
	Collision collision;
	while (true)
	{
		//IO初期化/前フレームIO情報キャッシュ
		Standard_initialize_IO.key_Initialize(keys, oldkeys);
		Standard_initialize_IO.Mouse_Initialize(&MouseDown, &oldMouseDown, MousePos);
		bool click = keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE];

		//画面クリア
		ClearDrawScreen();

		BG_scroll();

		//背景描画
		DrawGraph(BG_X[0], 0, BG, TRUE);//z
		DrawGraph(BG_X[1], 0, BG, TRUE);//z

		//既存ワールドデータ描画
		for (auto i = 0; i < World_value; i++) {
			DrawGraph(World[i][0] + World_x_adjust, World[i][1], nomal_block[0], true); //通常ブロック
		}

		//インターフェース描画
		DrawGraph(0, 0, UI_image, true);
		DrawGraph(Win_config.WIN_WIDTH - arrowWidth, Win_config.WIN_HEIGHT / 2 - arrowHeight / 2, Scroll_arrow, TRUE);//前進
		DrawTurnGraph(192, Win_config.WIN_HEIGHT / 2 - arrowHeight / 2, Scroll_arrow, TRUE);//後退

		for (auto i = 0; i < 2; i++) {
			//当たり判定
			bool collision_start = collision.Trigonometric_Fanc(UI_Button_pos[i][0], UI_Button_pos[i][1], 32, MousePos[0], MousePos[1], 32);
			//描画処理
			DrawGraph(UI_Button_pos[i][0], UI_Button_pos[i][1], UI_Buttoniamge[i], true);

			//マウスが再生ボタンを押した場合ゲームスタート
			if (i == 0 && collision_start == true) {
				if (MouseDown != true && oldMouseDown == true) {
					Startbutton_flag = true;
				}
			}
		}

		//ブロック描画
		for (auto i = 0; i < 3; i++) {
			if (brocks_pos[i][BLOCK_X] == 10) {
				if (i == 0) {
					DrawGraph(brocks_pos[0][BLOCK_X], brocks_pos[0][BLOCK_Y], defoliation_brock[0], true); //落下ブロック
				}
				else if (i == 1) {
					DrawGraph(brocks_pos[1][BLOCK_X], brocks_pos[1][BLOCK_Y], nomal_block[0], true); //通常ブロック
				}
				else {
					DrawGraph(brocks_pos[2][BLOCK_X], brocks_pos[2][BLOCK_Y], jump_brock[0], true); //ジャンプブロック
				}
			}
			else {
				if (i == 0) {
					DrawGraph(brocks_pos[0][BLOCK_X] + World_x_adjust, brocks_pos[0][BLOCK_Y], defoliation_brock[0], true); //落下ブロック
				}
				else if (i == 1) {
					DrawGraph(brocks_pos[1][BLOCK_X] + World_x_adjust, brocks_pos[1][BLOCK_Y], nomal_block[0], true); //通常ブロック
				}
				else {
					DrawGraph(brocks_pos[2][BLOCK_X] + World_x_adjust, brocks_pos[2][BLOCK_Y], jump_brock[0], true); //ジャンプブロック
				}
			}
		}

		ScreenFlip();
		WaitTimer(20);
		/*if (click) { break; }*/
		if (Startbutton_flag == true) { break; }
		if (ProcessMessage() == -1) { break; }
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) { break; }
	}
}

//背景スクロール
void Scene_Build::BG_scroll()//z
{
	Window_config Win_config;
	bool MosueLeftInput = MouseDown && !oldMouseDown;
	bool scrollLeftFlag = MousePos[0] > 192 && MousePos[0] < 192 + 32 && MousePos[1] > Win_config.WIN_HEIGHT / 2 - 32 && MousePos[1] < Win_config.WIN_HEIGHT / 2 + 32;
	bool scrollRightFlag = MousePos[0] > Win_config.WIN_WIDTH - 32 && MousePos[0] < Win_config.WIN_WIDTH && MousePos[1] > Win_config.WIN_HEIGHT / 2 - 32 && MousePos[1] < Win_config.WIN_HEIGHT / 2 + 32;

	int scrollVel = 100;

	if (scrollRightFlag && MosueLeftInput)
	{
		BG_X[0] -= scrollVel;
		BG_X[1] -= scrollVel;
		World_x_adjust -= scrollVel;
	}
	if (scrollLeftFlag && MosueLeftInput)
	{
		BG_X[0] += scrollVel;
		BG_X[1] += scrollVel;
		World_x_adjust += scrollVel;
	}
	for (int i = 0; i < 2; ++i)
	{
		if (BG_X[i] < -Win_config.WIN_WIDTH)
		{
			BG_X[i] = Win_config.WIN_WIDTH - scrollVel;
		}
		if (BG_X[i] > Win_config.WIN_WIDTH)
		{
			BG_X[i] = -Win_config.WIN_WIDTH + scrollVel;
		}
	}
}

//落ち葉ブロックと通常ブロックの衝突判定
bool Scene_Build::collision_defoliation_normal() {
	Collision collision;
	return collision.box_Fanc(
		brocks_pos[0][BLOCK_X], static_cast<double>(brocks_pos[0][BLOCK_X]) + BLOCK_SIZE,
		brocks_pos[0][BLOCK_Y], static_cast<double>(brocks_pos[0][BLOCK_Y]) + BLOCK_SIZE,
		brocks_pos[1][BLOCK_X], static_cast<double>(brocks_pos[1][BLOCK_X]) + BLOCK_SIZE,
		brocks_pos[1][BLOCK_Y], static_cast<double>(brocks_pos[1][BLOCK_Y]) + BLOCK_SIZE);
}

//通常ブロックとジャンプブロックの衝突判定
bool Scene_Build::collision_normal_jump() {
	Collision collision;
	return collision.box_Fanc(
		brocks_pos[1][BLOCK_X], static_cast<double>(brocks_pos[1][BLOCK_X]) + BLOCK_SIZE,
		brocks_pos[1][BLOCK_Y], static_cast<double>(brocks_pos[1][BLOCK_Y]) + BLOCK_SIZE,
		brocks_pos[2][BLOCK_X], static_cast<double>(brocks_pos[2][BLOCK_X]) + BLOCK_SIZE,
		brocks_pos[2][BLOCK_Y], static_cast<double>(brocks_pos[2][BLOCK_Y]) + BLOCK_SIZE);
}

//ジャンプブロックと落ち葉ブロックの衝突判定
bool Scene_Build::collision_jump_defoliation() {
	Collision collision;
	return collision.box_Fanc(
		brocks_pos[0][BLOCK_X], static_cast<double>(brocks_pos[0][BLOCK_X]) + BLOCK_SIZE,
		brocks_pos[0][BLOCK_Y], static_cast<double>(brocks_pos[0][BLOCK_Y]) + BLOCK_SIZE,
		brocks_pos[2][BLOCK_X], static_cast<double>(brocks_pos[2][BLOCK_X]) + BLOCK_SIZE,
		brocks_pos[2][BLOCK_Y], static_cast<double>(brocks_pos[2][BLOCK_Y]) + BLOCK_SIZE);
}

//ブロック同士の衝突処理
int Scene_Build::collision_block_otherblock(int *mouse_x, int *mouse_y, const int(*pos_tmp)[3], int num) {
	if (mouse_x == nullptr || mouse_y == nullptr) {
		return true;
	}

	if (collision_defoliation_normal() == true && (num == 0 || num == 1)) {
		brocks_pos[num][BLOCK_X] = pos_tmp[num][BLOCK_X];
		brocks_pos[num][BLOCK_Y] = pos_tmp[num][BLOCK_Y];
	}
	else if (collision_normal_jump() == true && (num == 1 || num == 2)) {
		brocks_pos[num][BLOCK_X] = pos_tmp[num][BLOCK_X];
		brocks_pos[num][BLOCK_Y] = pos_tmp[num][BLOCK_Y];
	}
	else if (collision_jump_defoliation() == true && (num == 2 || num == 0)) {
		brocks_pos[num][BLOCK_X] = pos_tmp[num][BLOCK_X];
		brocks_pos[num][BLOCK_Y] = pos_tmp[num][BLOCK_Y];
	}
	else {
		brocks_pos[num][BLOCK_X] = *mouse_x - BLOCK_SIZE / 2;
		brocks_pos[num][BLOCK_Y] = *mouse_y - BLOCK_SIZE / 2;
	}

	return false;
}

//マウスと落ち葉ブロックの衝突判定
bool Scene_Build::collision_defoliation_mouse() {
	Collision collision;
	return collision.box_Fanc(
		MousePos[0], MousePos[0],
		MousePos[1], MousePos[1],
		brocks_pos[0][BLOCK_X], static_cast<double>(brocks_pos[0][BLOCK_X]) + BLOCK_SIZE,
		brocks_pos[0][BLOCK_Y], static_cast<double>(brocks_pos[0][BLOCK_Y]) + BLOCK_SIZE);
}

//マウスと通常ブロックの衝突判定
bool Scene_Build::collision_normal_mouse() {
	Collision collision;
	return collision.box_Fanc(
		MousePos[0], MousePos[0],
		MousePos[1], MousePos[1],
		brocks_pos[1][BLOCK_X], static_cast<double>(brocks_pos[1][BLOCK_X]) + BLOCK_SIZE,
		brocks_pos[1][BLOCK_Y], static_cast<double>(brocks_pos[1][BLOCK_Y]) + BLOCK_SIZE);
}

//マウスとジャンプブロックの衝突判定
bool Scene_Build::collision_jump_mouse() {
	Collision collision;
	return collision.box_Fanc(
		MousePos[0], MousePos[0],
		MousePos[1], MousePos[1],
		brocks_pos[2][BLOCK_X], static_cast<double>(brocks_pos[2][BLOCK_X]) + BLOCK_SIZE,
		brocks_pos[2][BLOCK_Y], static_cast<double>(brocks_pos[2][BLOCK_Y]) + BLOCK_SIZE);
}