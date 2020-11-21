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
	inport.Inport(World, "scene.scene", &World_count);

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
	World_inport inport;
	Window_config Win_config;
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
		for (auto i = 0; i < World_count; i++) {
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
