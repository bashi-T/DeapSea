#pragma once
#include<memory>
#include"SceneManager.h"
#include"TitleScene.h"
#include"GameScene.h"

#include"Systems/WindowApp.h"
#include"Systems/Debug.h"
#include"Input/Input.h"
#include"Commons/SpriteCommon.h"
#include"Commons/FullScreenSpriteCommon.h"
#include"Objects/FullScreenSprite.h"
#include"Commons/Object3dCommon.h"
#include"Managers/ModelManager.h"
#include"Objects/model.h"
#include"Managers/TextureManager.h"
#include"Managers/SRVManager.h"

class GameManager
{
private:
	std::unique_ptr<SceneManager> sceneArr_[2];
	int currentSceneNo_ = 0;
	int prevSceneNo_ = 0;
	WinAPP* winAPP;
	DX12Common* dx12Common;
	SRVManager* srvManager;
	Input* input;
	MyImGui* imgui;
	SpriteCommon* SPCommon;
	FullScreenSpriteCommon* FSSPCommon;
	Object3dCommon* object3dCommon;
	ModelCommon* modelCommon;
	Camera* camera;
	Particle* particle;
	FullScreenSprite* fullScreenSprite;

public: 
	GameManager();
	~GameManager();
	int Run();
};

