#pragma once
#include"BaseScene.h"
#include"GameOverScene.h"
#include"ClearScene.h"

#include"Input/Input.h"
#include"Sprites/Sprite.h"
#include"Commons/SpriteCommon.h"
#include"GameObject/Player.h"
#include"GameObject/Enemy.h"
#include"GameObject/ground.h"
#include"Managers/AudioManager.h"
#include <random>

namespace MyEngine
{
    class GameScene : public BaseScene
	{

	public:
		void Initialize() override;
	    std::unique_ptr<BaseScene> Update() override;
		void Draw() override;

		void LoadEnemyPopData(std::string filePath, int32_t fileNum);
		void UpdateEnemyPopCommands(int32_t fileNum);

		void CheckAllCollisions();//当たり判定

		void DirectGameStart();//シーン演出
	    void DirectGameOver();//シーン演出
	    void DirectGameClear();//シーン演出

	private:
	    std::unique_ptr<BaseScene> nextScene = nullptr;

	    Camera* camera_ = nullptr;
		std::unique_ptr<Particle> particle_;
		std::vector<std::unique_ptr<Sprite>>sprites_;
		std::unique_ptr<Player> player_;
	    std::list<std::unique_ptr<Enemy>> enemys_;
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


	};
}