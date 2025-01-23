#pragma once
#include"Input/Input.h"
#include"SceneManager.h"
#include"Commons/Object3dCommon.h"
#include"Objects/Object3d.h"
#include"Sprites/Sprite.h"
#include"Objects/Particle.h"
#include "GameManager.h"
#include"GameObject/Cursor.h"
#include"GameObject/UIPlane.h"
#include"GameObject/Whale.h"
#include"GameObject/Player.h"
#include"Managers/AudioManager.h"

namespace MyEngine
{
	class TitleScene :public SceneManager
	{
	public:
		void Initialize() override;
		void Update() override;
		void Draw() override;
		void Finalize() override;
	private:
		//Input* input;
		//Object3dCommon* object3dCommon;
		//SRVManager* srvManager;
		Camera* camera_ = nullptr;
		std::vector<std::unique_ptr<Particle>> particles_;
		std::vector<std::unique_ptr<Sprite>> sprites_;
		std::vector<std::unique_ptr<UIPlane>> uiPlanes_;
		std::unique_ptr<Cursor> cursor_;
		std::unique_ptr<Player> player_;
		std::unique_ptr<Whale> whale_;
		bool isSceneTransition = false;
		bool isStageSelect = false;
		int numStage = 0;
		int sceneTransitionTime = 0;
		int nowStage = 0;
		int cooltime = 0;
		int floatingTime = 0;

		AudioManager::SoundData bgm = {};
		AudioManager::SoundData enterSound = {};
		AudioManager::SoundData moveSound = {};
		//std::string PNGs[6];

		enum UIType
		{
			Title,
			Practice,
			Stage1,
			Stage2,
			Stage3,
			Blackout,
			NumArgument
		};
		std::string Planes[6]
		{
			"plane/plane.obj",
			"plane/plane.obj",
			"plane/plane.obj",
			"plane/plane.obj",
			"plane/plane.obj",
			"plane/plane.obj",
		};
		Vector3 UIPos[6]
		{
			{ 0.0f,1.6f,-16.0f },
			{ 0.0f,2.1f,0.0f },
			{ 0.0f,0.7f,0.0f },
			{ 0.0f,-0.7f,0.0f },
			{ 0.0f,-2.1f,0.0f },
			{0.0f,0.0f,0.0f},
		};
		Vector3 UIScale[6]
		{
			{ 1.0f,1.0f,1.0f },
			{ 1.5f,1.0f,0.0f },
			{ 1.5f,1.0f,0.0f },
			{ 1.5f,1.0f,0.0f },
			{ 0.0f,-2.1f,0.0f },
			{ 10.0f,10.0f,0.0f },
		};
		const float zfar = 2.0f;
		const float cursorX = -2.5f;
		const int maxCoolTime = 10;
		const int floatingTimes[2]//折り返しの時間
		{
			120,
			240
		};
		const int sceneTransitionTimes[3]
		{
			20,
			40,
			90
		};

		const float whaleRotateY = 0.003f;
		const float whaleFloating = 0.01f;
		const float uiFloating = 0.001f;
	};
}