#pragma once
#include"BaseScene.h"

class SceneManager
{
public:
	~SceneManager();
	void SetNextScene(BaseScene* nextScene) { nextScene_ = nextScene; }
	void Update();
	void Draw();

private:
	BaseScene* scene_ = nullptr;
	BaseScene* nextScene_ = nullptr;
};

