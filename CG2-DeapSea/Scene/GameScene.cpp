#include "GameScene.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

void GameScene::Init()
{
	player_ = new Player;
	whale_ = new Whale;
	ground = new Ground;
	player_->Initialize();
	whale_->Initialize();
	ground->Initialize();
	enemyPopFile[0] = "Resource/CSV/practiceFile.csv";

	for (uint32_t i = 0; i < 1; i++)
	{
		LoadEnemyPopData(enemyPopFile[i], i);
	}
	gameEnd = false;
}

void GameScene::Update()
{
	enemys_.remove_if([](Enemy* enemy)
		{
			if (enemy->IsDead())
			{
				delete enemy;
				return true;
			}
			return false;
		});
	if (whale_->GetLife() == 0)
	{
		enemys_.resize(0);
		sceneNo = GAMEOVER;
	}
	else if (enemys_.size() == 0 && gameEnd)
	{
		sceneNo = CLEAR;
	}else
	if (Input::GetInstance()->TriggerKey(DIK_S))
	{
		sceneNo = TITLE;
	}

	if (whale_->GetTranslate().x > player_->GetTranslate().x + whale_->GetMaxDistance())
	{
		whale_->SetTranslate({ player_->GetTranslate().x + whale_->GetMaxDistance(),whale_->GetTranslate().y,whale_->GetTranslate().z });
	}
	if (whale_->GetTranslate().x < player_->GetTranslate().x - whale_->GetMaxDistance())
	{
		whale_->SetTranslate({ player_->GetTranslate().x - whale_->GetMaxDistance(),whale_->GetTranslate().y,whale_->GetTranslate().z });
	}

	if (whale_->GetTranslate().z > player_->GetTranslate().z + whale_->GetMaxDistance())
	{
		whale_->SetTranslate({ whale_->GetTranslate().x,whale_->GetTranslate().y,player_->GetTranslate().z + whale_->GetMaxDistance() });
	}
	if (whale_->GetTranslate().z < player_->GetTranslate().z - whale_->GetMaxDistance())
	{
		whale_->SetTranslate({ whale_->GetTranslate().x,whale_->GetTranslate().y,player_->GetTranslate().z - whale_->GetMaxDistance() });
	}

	if (player_->GetTranslate().x <= -10.0f)
	{
		player_->SetTranslate({ -10.0f,player_->GetTranslate().y,player_->GetTranslate().z });
	}

	if (player_->GetTranslate().x >= 10.0f)
	{
		player_->SetTranslate({ 10.0f,player_->GetTranslate().y,player_->GetTranslate().z });
	}
	ground->Update();
	player_->Update();
	whale_->Update();
	UpdateEnemyPopCommands(GameManager::stageNumber);
	for(Enemy*enemy_:enemys_)
	{
		enemy_->Update(enemy_->GetSort());
	}
	CheckAllCollisions();
}

void GameScene::Draw()
{
	ground->Draw();
	player_->Draw();
	whale_->Draw();
	for (Enemy* enemy_ : enemys_)
	{
		enemy_->Draw(enemy_->GetSort());
	}
}

void GameScene::Finalize()
{
	for (Sprite* sprite : sprites)
	{
		delete sprite;
		sprite = NULL;
	}
	sprites.clear();
	delete ground;
	ground = NULL;
	delete whale_;
	whale_ = NULL;
	player_->Finalize();
	delete player_;
	player_ == NULL;
	for (Enemy* enemy_ : enemys_)
	{
		delete enemy_;
		enemy_ = NULL;
	}
	enemys_.clear();
	enemyPopFile[0].clear();
	enemyPopFile[0].shrink_to_fit();
	enemyPopCommands[0].str("");
	enemyPopCommands[0].clear(std::stringstream::goodbit);
}

void GameScene::CheckAllCollisions()
{
	Vector3 posA, posB;
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	if(player_->GetIsHit()==false&&player_->GetIsHitTimer()==0)
	{
#pragma region 自機と敵弾の当たり判定
		for (Enemy* enemy_ : enemys_)
		{
			const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

			posA = player_->GetTranslate();
			for (EnemyBullet* bullet : enemyBullets)
			{
				posB = bullet->GetTranslate();
				Vector3 distance = Subtract(posA, posB);
				if ((distance.x * distance.x) + (distance.y * distance.y) +
					(distance.z * distance.z) <= 4)
				{
					player_->OnCollision();
					bullet->OnCollision();
				}
			}
		}
#pragma endregion
#pragma region 自機と敵の当たり判定
		for (Enemy* enemy_ : enemys_)
		{
			posA = player_->GetTranslate();
			posB = enemy_->GetTranslate();
			Vector3 distance = Subtract(posA, posB);
			if ((distance.x * distance.x) + (distance.y * distance.y) +
				(distance.z * distance.z) <= 4)
			{
				player_->OnCollision();
			}
		}
#pragma endregion
	}
#pragma region 敵と自弾の当たり判定
	for (Enemy* enemy_ : enemys_)
	{
		posA = enemy_->GetTranslate();
		for (PlayerBullet* bullet : playerBullets)
		{
			posB = bullet->GetTranslate();
			Vector3 distance = Subtract(posA, posB);
			if ((distance.x * distance.x) + (distance.y * distance.y) +
				(distance.z * distance.z) <= 2)
			{
				enemy_->OnCollision();
				bullet->OnCollision();
			}
		}
	}
#pragma endregion
#pragma region 自弾と敵弾の当たり判定
	for (Enemy* enemy_ : enemys_)
	{
		const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

		for (PlayerBullet* pBullet : playerBullets)
		{
			for (EnemyBullet* eBullet : enemyBullets)
			{
				posA = pBullet->GetTranslate();
				posB = eBullet->GetTranslate();
				Vector3 distance = Subtract(posA, posB);
				if ((distance.x * distance.x) + (distance.y * distance.y) +
					(distance.z * distance.z) <= 3)
				{
					pBullet->OnCollision();
					eBullet->OnCollision();
				}
			}
		}
	}
#pragma endregion
	if (whale_->GetIsHit() == false)
	{
#pragma region 敵とクジラの当たり判定
		for (Enemy* enemy_ : enemys_)
		{
			posA = whale_->GetTranslate();
			posB = enemy_->GetTranslate();
			Vector3 distance = Subtract(posA, posB);
			if ((distance.x * distance.x) + (distance.y * distance.y) +
				(distance.z * distance.z) <= 4)
			{
				whale_->OnCollision();
				enemy_->OnCollision();
				return;
			}
		}
#pragma endregion
#pragma region 敵弾とクジラの当たり判定
		for (Enemy* enemy_ : enemys_)
		{
			const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

			posA = whale_->GetTranslate();
			for (EnemyBullet* bullet : enemyBullets)
			{
				posB = bullet->GetTranslate();
				Vector3 distance = Subtract(posA, posB);
				if ((distance.x * distance.x) + (distance.y * distance.y) +
					(distance.z * distance.z) <= 4)
				{
					whale_->OnCollision();
					bullet->OnCollision();
					return;
				}
			}
		}
#pragma endregion
	}
}

void GameScene::LoadEnemyPopData(std::string filePath,int fileNum)
{
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	enemyPopCommands[fileNum] << file.rdbuf();
	file.close();
}

void GameScene::UpdateEnemyPopCommands(int fileNum)
{
	if (isWait)
	{
		WaitTimer--;
		if (WaitTimer <= 0)
		{
			isWait = false;
		}
		return;
	}
	std::string line;
	while (getline(enemyPopCommands[fileNum], line))
	{
		std::istringstream line_stream(line);
		std::string word;
		getline(line_stream, word, ',');

		if (word.find("//") == 0)
		{
			continue;
		}

		if (word.find("POP") == 0)
		{
			//x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			//y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			//z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str()) + player_->GetTranslate().z;

			std::random_device seedGenerator;
			std::mt19937 randomEngine(seedGenerator());
			std::uniform_real_distribution<float> enemySort(0, 2);
			Enemy* enemy_ = new Enemy;
			enemy_ = new Enemy;
			enemy_->SetSort(int(enemySort(randomEngine)));
			enemy_->Initialize(player_, whale_, enemy_->GetSort());
			enemys_.push_back(enemy_);
			enemy_->SetTranslate({ x,y,z });
			enemy_->SetEnemyVector(whale_->GetTranslate());
		}
		else if (word.find("WAIT") == 0)
		{
			getline(line_stream, word, ',');
			int32_t waitTime = atoi(word.c_str());
			isWait = true;
			WaitTimer = waitTime;
			break;
		}
		else if (word.find("END") == 0)
		{
			gameEnd = true;
			word.clear();
			line_stream.clear();
			line.clear();
		}
	}
}
