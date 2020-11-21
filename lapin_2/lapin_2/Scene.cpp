#include "DxLib.h"
#include "Scene.h"
#include "IO.h"

Scene::Scene()
{
	for (auto i = 0; i < 100; i++) {
		this->World[i][0] = 0;
		this->World[i][1] = 0;
	}

	for (auto i = 0; i < 3; i++) {
		this->User_brock_pos[i][0] = 0;
		this->User_brock_pos[i][1] = 0;
	}
	return;
}

Scene::~Scene()
{
	return;
}