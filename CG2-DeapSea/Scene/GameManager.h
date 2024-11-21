#pragma once
#include<memory>
#include<iostream>
#include"SceneManager.h"
#include"TitleScene.h"
#include"GameScene.h"
#include"GameOverScene.h"
#include"ClearScene.h"

#include"Systems/WindowApp.h"
#include"Systems/Debug.h"
#include"Input/Input.h"
#include"Commons/SpriteCommon.h"
#include"Commons/Object3dCommon.h"
#include"Commons/ParticleCommon.h"
#include"Managers/ModelManager.h"
#include"Managers/AudioManager.h"
#include"Managers/TextureManager.h"
#include"Managers/SRVManager.h"
#include"Objects/model.h"
#include"Objects/SkyDome.h"
#include"Objects/Mesh.h"


namespace MyEngine
{
	class GameManager
	{
	private:
		std::unique_ptr<SceneManager> sceneArr_[4];
		int currentSceneNo_ = 0;
		int prevSceneNo_ = 0;
		WinAPP* winAPP;
		DX12Common* dx12Common;
		SRVManager* srvManager;
		Input* input;
		MyImGui* imgui;
		SpriteCommon* SPCommon;
		Object3dCommon* object3dCommon;
		ParticleCommon* particleCommon;
		AudioManager* audioManager;
		std::unique_ptr< Object3d> object3d;
		std::unique_ptr<Camera> camera;
		std::unique_ptr<SkyDome> skyDome;

		HRESULT hr;
	public:
		GameManager();
		~GameManager();
		int Run();
		static int stageNumber;
	};
}