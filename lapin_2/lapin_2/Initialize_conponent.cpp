#include "Initialize_conponent.h"
#include "DxLib.h"

Initialize_conponent::Initialize_conponent()
{
	return;
}

Initialize_conponent::~Initialize_conponent()
{
	return;
}

void Initialize_conponent::Initialize()
{
	// ウィンドウモード
	ChangeWindowMode(true);
	SetWindowSizeChangeEnableFlag(true, false);
	SetMainWindowText(TITLE);
	SetGraphMode(WIN_WIDTH * 2, WIN_HEIGHT, 32);
	SetWindowSize(800, 450);
	SetWindowSizeExtendRate(1.0);
	SetBackgroundColor(0, 0, 0);
}
