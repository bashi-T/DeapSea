#include "SceneManager.h"

SceneManager::~SceneManager()
{
	scene_->Finalize();
	delete scene_;
}

void SceneManager::Update() {
	if (nextScene_)//次シーンがあるとき
	{
		if (scene_)
		{//旧シーン終了
			scene_->Finalize();
			delete scene_;
		}
		scene_ = nextScene_;//シーン切り替え
		nextScene_ = nullptr;
		scene_->Initialize();
	}
}

void SceneManager::Draw()
{ 
	scene_->Draw();
}
