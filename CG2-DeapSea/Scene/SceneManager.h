#pragma once
#include"BaseScene.h"
#include"TitleScene.h"
#include"GameScene.h"
#include"GameOverScene.h"
#include"ClearScene.h"


class SceneManager
{
public:
	SceneManager();

	void Initialize();
	void Update();
	void Draw() { currentScene->Draw(); }

	static SceneManager* GetInstance();

	void SetStageNumber(int32_t number) { stageNumber = number; }
	int32_t GetStageNumber() { return stageNumber; }

protected:
	int32_t stageNumber = 0;

private:
	std::unique_ptr<BaseScene> currentScene;
	static inline std::unique_ptr<SceneManager> instance;
};

