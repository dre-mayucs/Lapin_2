#include "DxLib.h"

#include "Scene_Start.h"
#include "IO.h"

#define BASE_FRAME 0
#define CACHE_FRAME 1
Scene_Start::Scene_Start()
{
	Background = LoadGraph("Resources\\Background\\background.png", true);
	LoadDivGraph("Resources\\Background\\animation_title.png", 34, 34, 1, 800, 450, Mask_image);
}

void Scene_Start::Start_Window()
{
	//ゲームメインループ
	while (true)
	{
		//IO初期化/前フレームIO情報キャッシュ
		Standard_initialize_IO.key_Initialize(keys, oldkeys);
		Standard_initialize_IO.Mouse_Initialize(&MouseDown, &oldMouseDown, MousePos);
		bool click = keys[KEY_INPUT_SPACE] && !oldkeys[KEY_INPUT_SPACE];

		//画面クリア
		ClearDrawScreen();

		DrawGraph(0, 0, Background, false);
		DrawGraph(0, 0, Mask_image[frame[BASE_FRAME]], true);

		if (frame[BASE_FRAME] == MAX_FRAME - 1) {
			frame[BASE_FRAME] = 0;
		}
		if (frame[CACHE_FRAME] == ADJUST_FRAME) {
			frame[BASE_FRAME]++;
			frame[CACHE_FRAME] = 0;
		}
		frame[CACHE_FRAME]++;

		ScreenFlip();
		WaitTimer(20);
		if (click) { break; }
		if (ProcessMessage() == -1) { break; }
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) { break; }
	}
}