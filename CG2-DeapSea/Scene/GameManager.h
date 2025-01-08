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
#include"Commons/MeshCommon.h"

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
	public:
		GameManager();
		~GameManager();
		/// <summary>
		/// メインループ
		/// </summary>
		/// <returns></returns>
		int32_t Run();
		static int32_t stageNumber;

	private:
		std::unique_ptr<SceneManager> sceneArr_[4];
		int32_t currentSceneNo_ = 0;
		int32_t prevSceneNo_ = 0;
		WinAPP* winAPP = nullptr;
		Input* input = nullptr;
		MyImGui* imgui = nullptr;
		HRESULT hr = NULL;

		DX12Common* dx12Common = nullptr;
		SRVManager* srvManager = nullptr;
		AudioManager* audioManager = nullptr;
		TextureManager* textureManager = nullptr;
		ModelManager* modelManager = nullptr;
		SpriteCommon* spriteCommon = nullptr;
		Object3dCommon* object3dCommon = nullptr;
		ParticleCommon* particleCommon = nullptr;
		MeshCommon* meshCommon = nullptr;
		Camera* camera;

		std::unique_ptr<Object3d> object3d = nullptr;
		std::unique_ptr<SkyDome> skyDome = nullptr;

	};
}