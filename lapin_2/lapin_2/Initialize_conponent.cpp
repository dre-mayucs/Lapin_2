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
	SetWindowSizeChangeEnableFlag(false, false);
	SetMainWindowText(TITLE);
	SetGraphMode(WIN_WIDTH, WIN_HEIGHT, 32);
	SetWindowSizeExtendRate(1.0);
	SetBackgroundColor(0, 0, 0);
}
