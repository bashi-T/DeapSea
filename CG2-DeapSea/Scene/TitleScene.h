#pragma once
#include"BaseScene.h"
#include"GameScene.h"
#include"SceneManager.h"

#include"Input/Input.h"
#include"Commons/Object3dCommon.h"
#include"Objects/Object3d.h"
#include"Sprites/Sprite.h"
#include"Objects/Particle.h"
#include"GameObject/Player.h"
#include"Managers/AudioManager.h"

namespace MyEngine
{
class TitleScene : public BaseScene
	{
	public:
		void Initialize() override;
	    std::unique_ptr<BaseScene> Update() override;
		void Draw() override;

	private:
	    std::unique_ptr<BaseScene> nextScene = nullptr;

	    Camera* camera_ = nullptr;
		std::vector<std::unique_ptr<Particle>> particles_;
		std::vector<std::unique_ptr<Sprite>> sprites_;
		std::unique_ptr<Player> player_;
		bool isSceneTransition = false;
		bool isStageSelect = false;
	    bool isSceneChange = false;
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
	};
}