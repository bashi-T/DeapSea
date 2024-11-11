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

class GameScene:public SceneManager
{
	enum EnemySort { NORMAL, STRAIGHT };

public:
	//~GameScene();
	void Init() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

	void LoadEnemyPopData(std::string filePath, int fileNum);
	void UpdateEnemyPopCommands(int fileNum);


	void CheckAllCollisions();
private:
	std::vector<Sprite*> sprites;
	std::unique_ptr<Player> player_;
	std::unique_ptr<Whale> whale_;
	std::list<Enemy*> enemys_;
	std::vector <UIPlane*>uiPlanes;
	std::vector <Particle*> particles;
	Tide* tide_;
	Ground* ground;
	
	std::stringstream enemyPopCommands[10];
	std::string enemyPopFile[10];
	bool isWait = false;
	int32_t WaitTimer = 1;
	bool gameEnd = false;
	AudioManager::SoundData bgm;
	int time = 0;
	bool isGameStart = false;
	bool isGameOver = false;
	bool isGameClear = false;
	int sceneTransitionTime = 0;
	std::string Planes[6]
	{
		"plane/plane.obj",
		"plane/plane.obj",
		"plane/plane.obj",
		"plane/plane.obj",
		"plane/plane.obj",
		"plane/plane.obj",
	};
	Vector3 v;
	Vector3 zoomPos;
};

