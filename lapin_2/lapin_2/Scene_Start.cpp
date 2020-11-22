#include "DxLib.h"
#include "Scene_Start.h"
#include "IO.h"
#include "Scene_Build.h"
#include "Scene_forest.h"
#include "Scene_city.h"

#define BASE_FRAME 0
#define CACHE_FRAME 1
Scene_Start::Scene_Start()
{
	Background = LoadGraph("Resources\\Background\\background.png", true);
	LoadDivGraph("Resources\\Background\\animation_title.png", 34, 34, 1, 800, 450, Mask_image);

	Select_BG = LoadGraph("Resources\\Background\\select.png");
}

void Scene_Start::Start_Window()
{
	ChangeVolumeSoundMem(128, BGM);
	ChangeVolumeSoundMem(128, Game_BGM);

	PlaySoundMem(BGM, DX_PLAYTYPE_LOOP);
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
		if (click) { 
			menu();
			break; 
		}
		if (ProcessMessage() == -1) { break; }
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) { break; }
	}
}
void Scene_Start::menu()
{
	Scene_Build *build;
	Scene_forest *forest;
	Scene_city *city;
	//ゲームメインループ
	while (true)
	{
		//IO初期化/前フレームIO情報キャッシュ
		Standard_initialize_IO.key_Initialize(keys, oldkeys);
		Standard_initialize_IO.Mouse_Initialize(&MouseDown, &oldMouseDown, MousePos);

		//画面クリア
		ClearDrawScreen();

		DrawGraph(0, 0, Select_BG, false);

		//チュートリアル、徳井...w
		if (MouseDown == true && 
			collision.box_Fanc(
				(double)window.WIN_WIDTH - 160 - 250, (double)window.WIN_WIDTH - 160, (double)window.WIN_HEIGHT - 139 * 2 - 16 * 2, (double)window.WIN_HEIGHT - 139 - 16 * 2,
				(double)MousePos[0] - 5, (double)MousePos[0] + 5, (double)MousePos[1] - 5, (double)MousePos[1] + 5
		)) {
			StopSoundMem(BGM);
			PlaySoundMem(Game_BGM, DX_PLAYTYPE_LOOP);
			SetWindowSize(1600, 450);
			build = new Scene_Build("forest");
			build->Build_Window();
			build->output_brokpos(user_brocks_pos);
			delete build;

			SetWindowSize(800, 450);
			forest = new Scene_forest(user_brocks_pos);
			forest->Forest_start();
			delete forest;
			StopSoundMem(Game_BGM);
			PlaySoundMem(BGM, DX_PLAYTYPE_LOOP);
		}

		//都市
		if (MouseDown == true && 
			collision.box_Fanc(
				(double)window.WIN_WIDTH - 16 - 250, (double)window.WIN_WIDTH - 16, (double)window.WIN_HEIGHT - 16 - 139, (double)window.WIN_HEIGHT - 16,
				(double)MousePos[0] - 5, (double)MousePos[0] + 5, (double)MousePos[1] - 5, (double)MousePos[1] + 5
		)) {
			StopSoundMem(BGM);
			PlaySoundMem(Game_BGM, DX_PLAYTYPE_LOOP);
			SetWindowSize(1600, 450);
			build = new Scene_Build("city");
			build->Build_Window();
			build->output_brokpos(user_brocks_pos);
			delete build;

			SetWindowSize(800, 450);
			city = new Scene_city(user_brocks_pos);
			city->City_start();
			delete city;
			StopSoundMem(Game_BGM);
			PlaySoundMem(BGM, DX_PLAYTYPE_LOOP);
		}
		ScreenFlip();
		WaitTimer(20);
		if (ProcessMessage() == -1) { break; }
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1) { break; }
	}
}
/*

	//ステージ実行シーン
	SetWindowSize(800, 450);
	Scene_forest *forest = new Scene_forest(user_brocks_pos);
	forest->Forest_start();
	delete forest;
*/