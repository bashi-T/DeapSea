#pragma once
#include"BaseScene.h"
#include"GameOverScene.h"
#include"ClearScene.h"

#include"Input/Input.h"
#include"Sprites/Sprite.h"
#include"Commons/SpriteCommon.h"
#include"GameObject/Player.h"
#include"GameObject/Whale.h"
#include"GameObject/Enemy.h"
#include"GameObject/ground.h"
#include"GameObject/UIPlane.h"
#include"GameObject/Tide.h"
#include"Managers/AudioManager.h"
#include"Systems/FileLoader.h"
#include <random>

namespace MyEngine
{
    class GameScene : public BaseScene
	{
		enum EnemySort { NORMAL, STRAIGHT };

	public:
		void Initialize() override;
	    std::unique_ptr<BaseScene> Update() override;
		void Draw() override;

		void LoadEnemyPopData(std::string filePath, int32_t fileNum);
		void UpdateEnemyPopCommands(int32_t fileNum);

		void CheckAllCollisions();

		void DirectGameStart();
	    void DirectGameOver();
	    void DirectGameClear();

	private:
	    std::unique_ptr<BaseScene> nextScene = nullptr;

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
		int32_t WaitTimer = 1;
		bool isWait = false;
		bool gameEnd = false;

		AudioManager::SoundData bgm = {};
		int32_t time = 0;

		bool isGameStart = false;
		bool isGameOver = false;
		bool isGameClear = false;
	    int32_t sceneTransitionTime = 0;
	    int32_t gameOverSceneTransitionTime = 0;
	    int32_t clearSceneTransitionTime = 0;

		float distance = 0.0f;

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

		std::string UIVectorFile[20]
		{
			"Resource/CSV/startUI/scale.csv",
	        "Resource/CSV/startUI/rotate.csv",
	        "Resource/CSV/startUI/translate.csv",
	        "Resource/CSV/startUI/defaultColor.csv",

			"Resource/CSV/blackOut/scale.csv",
			"Resource/CSV/blackOut/defaultColor.csv",

			"Resource/CSV/key/moveKey/scale.csv",
			"Resource/CSV/key/moveKey/rotate.csv",
			"Resource/CSV/key/moveKey/color.csv",

			"Resource/CSV/key/attackKey/scale.csv",
			"Resource/CSV/key/attackKey/rotate.csv",
			"Resource/CSV/key/attackKey/color.csv",

	   		"Resource/CSV/key/lookKey/scale.csv",
			"Resource/CSV/key/lookKey/rotate.csv",
			"Resource/CSV/key/lookKey/color.csv",

			"Resource/CSV/key/barrierKey/scale.csv",
			"Resource/CSV/key/barrierKey/rotate.csv",
			"Resource/CSV/key/barrierKey/color.csv",
};
		Vector3 v = {};
		Vector3 zoomPos = {};
		float ingameCameraY = 76.0f;
		Vector3 ingameCameraRotate = { 0.2f,0.0f,0.0f };
	    const float zfar = 2.0f;
	    Vector2 spritePos = {10.0f, -50.0f};
	    float enemyDeadLine = 15.0f;
	    const float reverse = radianPerFrequency * 180;
	    Vector3 distanceCtoUI = {0.0f, -10.0f, -20.0f};

		bool isMove = false;
	    bool isAttack = false;
	    bool isLook = false;
	    bool isPose = false;
	    bool isGameOverScene = false;
	    bool isGameClearScene = false;
	    Vector3 offset;
	};
}