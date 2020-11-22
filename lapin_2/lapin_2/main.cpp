#include "DxLib.h"
#include "Color.h"
#include "Initialize_conponent.h"
#include "Scene_Start.h"

int WINAPI WinMain(_In_ HINSTANCE hInstance, _In_opt_ HINSTANCE hPrevInstance, _In_ LPSTR lpCmdLine, _In_ int nCmdShow)
{
	int user_brocks_pos[3][2];

	//������
	Initialize_conponent Init;
	Init.Initialize();
	if (DxLib_Init() == -1) { return -1; }
	SetDrawScreen(DX_SCREEN_BACK);

	//�Q�[���X�^�[�g
	Scene_Start *start = new Scene_Start;
	start->Start_Window();
 	delete start;

	//�I������
	DxLib_End();
	return 0;
}