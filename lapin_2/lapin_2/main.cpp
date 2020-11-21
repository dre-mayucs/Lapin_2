#include "DxLib.h"
#include "Color.h"
#include "Initialize_conponent.h"
#include "Scene_Start.h"
#include "Scene_Build.h"
#include "Scene_forest.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	int user_brocks_pos[3][2];

	//������
	Initialize_conponent Init;
	Init.Initialize();
	if (DxLib_Init() == -1) { return -1; }
	SetDrawScreen(DX_SCREEN_BACK);

	//�^�C�g��
	Scene_Start *start = new Scene_Start;
	start->Start_Window();
	delete start;

	//�X�e�[�W����V�[��
	SetWindowSize(1600, 450);
	Scene_Build *Build = new Scene_Build;
	Build->Build_Window();
	Build->output_brokpos(user_brocks_pos);
	delete Build;

	//�X�e�[�W���s�V�[��
	SetWindowSize(800, 450);
	Scene_forest *forest = new Scene_forest(user_brocks_pos);
	forest->Forest_start();
	delete forest;

	//�I������
	DxLib_End();
	return 0;
}