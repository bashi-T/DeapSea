#include "SceneManager.h"

SceneManager::SceneManager()
{
	currentScene = std::make_unique<GameScene>(); // 最初のシーン
}

void SceneManager::Initialize() { currentScene->Initialize(); }

void SceneManager::Update()
{
	std::unique_ptr<BaseScene> nextScene = currentScene->Update();

	if (nextScene)
	{
		currentScene = std::move(nextScene); // シーンを入れ替える
		currentScene->Initialize();
	}
}

SceneManager* SceneManager::GetInstance()
{
	if (instance == NULL)
	{
		instance = std::make_unique<SceneManager>();
	}
	return instance.get();
}
