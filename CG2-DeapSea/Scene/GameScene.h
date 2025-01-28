#pragma once
#include"Input/Input.h"
#include"SceneManager.h"
#include"Sprites/Sprite.h"
#include"Commons/SpriteCommon.h"
#include"GameObject/Player.h"
#include"GameObject/Whale.h"
#include"GameObject/Enemy.h"
#include "GameManager.h"
#include"GameObject/ground.h"
#include"GameObject/UIPlane.h"
#include"GameObject/Tide.h"
#include"Managers/AudioManager.h"
#include <random>

namespace MyEngine
{
	class GameScene :public SceneManager
	{
		enum EnemySort { NORMAL, STRAIGHT };

	public:
		void Initialize() override;
		void Update() override;
		void Draw() override;
		void Finalize() override;

		void LoadEnemyPopData(std::string filePath, int32_t fileNum);
		void UpdateEnemyPopCommands(int32_t fileNum);

		void CheckAllCollisions();

	private:
		Camera* camera_ = nullptr;
		std::unique_ptr<Particle> particle_;
		std::vector<std::unique_ptr<Sprite>>sprites_;
		std::vector<std::unique_ptr<UIPlane>>uiPlanes_;
		std::unique_ptr<Player> player_;
		std::unique_ptr<Whale> whale_;
		std::list<std::unique_ptr<Enemy>> enemys_;
		std::unique_ptr<Tide>tide_;
		std::unique_ptr<Ground>ground_;

		std::stringstream enemyPopCommands[10];
		std::string enemyPopFile[10];
		bool isWait = false;
		int32_t WaitTimer = 1;
		bool gameEnd = false;
		AudioManager::SoundData bgm = {};
		int32_t time = 0;
		bool isGameStart = false;
		bool isGameOver = false;
		bool isGameClear = false;
		int32_t sceneTransitionTime = 0;

		enum UIType
		{
			Start,
			Blackout,
			moveKey,
			attackKey,
			lookKey,
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
		Vector3 v = {};
		Vector3 zoomPos = {};
		float ingameCameraY = 76.0f;
		Vector3 ingameCameraRotate = { 0.2f,0.0f,0.0f };
		const float zfar = 2.0f;
		Vector2 spritePos = { 10.0f,-50.0f };
		bool isMove;
		bool isAttack;
	    bool isLook;
	};
}