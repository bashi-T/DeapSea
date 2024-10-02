#include "SceneManager.h"
int SceneManager::sceneNo = INGAME;

SceneManager::~SceneManager()
{
}

int SceneManager::GetSceneNo()
{
	return sceneNo;
}
