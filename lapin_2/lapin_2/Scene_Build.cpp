#include "DxLib.h"

#include "Scene_Build.h"
#include "World_inport.h"
#include "IO.h"
#include "Win_config.h"

Scene_Build::Scene_Build()
{
	//ワールド読み込み
	World_inport inport;
	inport.Inport(World, "scene.scene");

	//素材定義
	BG = LoadGraph("Resources\\Background\\background.png");

	LoadDivGraph("Resources\\Object\\defoliation-Sheet.png", DEFOLIATION_NUM, DEFOLIATION_NUM, 1, BLOCK_SIZE, BLOCK_SIZE, defoliation_brock);
	LoadDivGraph("Resources\\Object\\jump-Sheet.png", JUMP_NUM, JUMP_NUM, 1, BLOCK_SIZE, BLOCK_SIZE, jump_brock);
	LoadDivGraph("Resources\\Object\\block.png", NOMAL_NUM, NOMAL_NUM, 1, BLOCK_SIZE, BLOCK_SIZE, nomal_block);

	UI_image = LoadGraph("Resources\\UI\\bar.png");
	Scroll_arrow = LoadGraph("Resources\\UI\\scroll.png");

	UI_Buttoniamge[0] = LoadGraph("Resources\\UI\\start1.png");
	UI_Buttoniamge[0] = LoadGraph("Resources\\UI\\stop1.png");

	for (int i = 0; i < 3; i++) {
		blocks_tmp[i][0] = brocks_pos[i][0];
		blocks_tmp[i][1] = brocks_pos[i][1];
	}
}

void Scene_Build::Build_Window()
{
	Window_config Win_config;
	while (true)
	{
		//IO初期化/前フレームIO情報キャッシュ
		Standard_initialize_IO.key_Initialize(keys, oldkeys);
		Standard_initialize_IO.Mouse_Initialize(&MouseDown, &oldMouseDown, MousePos);
		bool click = keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE];

		//画面クリア
		ClearDrawScreen();

		BG_scroll(BG_X, &World_x_adjust, MousePos[0], MousePos[1]);//z

		//背景描画
		DrawGraph(BG_X[0], 0, BG, TRUE);//z
		DrawGraph(BG_X[1], 0, BG, TRUE);//z

		//既存ワールドデータ描画
		for (auto i = 0; i < World_x_adjust; i++) {
			DrawGraph(World[i][0] + World_x_adjust, World[i][1], nomal_block[0], true); //通常ブロック
		}

		//インターフェース描画
		DrawGraph(0, 0, UI_image, true);
		DrawGraph(Win_config.WIN_WIDTH - arrowWidth, Win_config.WIN_HEIGHT / 2 - arrowHeight / 2, Scroll_arrow, TRUE);//前進
		DrawTurnGraph(192, Win_config.WIN_HEIGHT / 2 - arrowHeight / 2, Scroll_arrow, TRUE);//後退

		ScreenFlip();
		WaitTimer(20);
		/*if (click) { break; }*/
		if (ProcessMessage() == -1) { break; }
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) { break; }
	}
}

void Scene_Build::BG_scroll(int *BgX, int *adjust, const int &mouseX, const int &mouseY)//z
{
	Window_config Win_config;

	bool keyLeftInput = MouseDown == MOUSE_INPUT_LEFT && oldMouseDown != MOUSE_INPUT_LEFT;
	bool scrollLeftFlag = mouseX > 192 && mouseX < 192 + 32 && mouseY > 2 - 32 && mouseY < Win_config.WIN_HEIGHT / 2 + 32;
	bool scrollRightFlag = mouseX > Win_config.WIN_WIDTH - 32 && mouseX < Win_config.WIN_WIDTH &&mouseY > Win_config.WIN_HEIGHT / 2 - 32 && mouseY < Win_config.WIN_HEIGHT / 2 + 32;

	int scrollVel = 100;

	if (scrollRightFlag && keyLeftInput)
	{
		BgX[0] -= scrollVel;
		BgX[1] -= scrollVel;
		*adjust -= scrollVel;
	}
	if (scrollLeftFlag && keyLeftInput)
	{
		BgX[0] += scrollVel;
		BgX[1] += scrollVel;
		*adjust += scrollVel;
	}
	for (int i = 0; i < 2; ++i)
	{
		if (BgX[i] < -Win_config.WIN_WIDTH)
		{
			BgX[i] = Win_config.WIN_WIDTH - scrollVel;
		}
		if (BgX[i] > Win_config.WIN_WIDTH)
		{
			BgX[i] = -Win_config.WIN_WIDTH + scrollVel;
		}
	}
}