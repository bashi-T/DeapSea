#include "SceneManager.h"
int SceneManager::sceneNo = TITLE;

SceneManager::~SceneManager()
{
}

int SceneManager::GetSceneNo()
{
	return sceneNo;
}
