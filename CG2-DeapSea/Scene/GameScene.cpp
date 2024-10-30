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
	whale_->Initialize(player_);
	ground->Initialize();
	
	std::string PNGs[6] =
	{
		"Resource/startUI.png",
		"Resource/black.png",
	};

	for (uint32_t i = 0; i < 2; i++)
	{
		UIPlane* uiPlane = new UIPlane;
		uiPlane->Initialize(Planes[i], PNGs[i]);
		uiPlanes.push_back(uiPlane);
	}
	uiPlanes[0]->SetScale({ 1.5f,1.0f,0.0f });
	uiPlanes[0]->SetTranslate({ 0.0f,-6.0f,-10.0f });
	uiPlanes[0]->SetColor({ 1.0f,1.0f,1.0f,1.0f });

	uiPlanes[1]->SetScale({ 10.0f,10.0f,0.0f });
	uiPlanes[1]->SetTranslate({ Camera::GetInstance()->GetTranslate().x, Camera::GetInstance()->GetTranslate().y, Camera::GetInstance()->GetTranslate().z+2.0f });
	uiPlanes[1]->SetColor({ 0.0f,0.0f,0.0f,1.0f });

	enemyPopFile[0] = "Resource/CSV/practiceFile.csv";
	enemyPopFile[1] = "Resource/CSV/STAGE1File.csv";
	enemyPopFile[2] = "Resource/CSV/STAGE2File.csv";
	enemyPopFile[3] = "Resource/CSV/STAGE3File.csv";

	LoadEnemyPopData(enemyPopFile[GameManager::stageNumber], GameManager::stageNumber);
	gameEnd = false;
	bgm = AudioManager::GetInstance()->SoundLoadWave("Resource/Sounds/stage1bgm.wav");
	//AudioManager::GetInstance()->SoundPlayWave(AudioManager::GetInstance()->GetxAudio2().Get(), bgm);
	time = 0;

	isGameStart = false;
	isGameOver = false;
	isGameClear = false;
	sceneTransitionTime = 0;
	Camera::GetInstance()->SetTranslate({ Camera::GetInstance()->GetTranslate().x, 73.0f, Camera::GetInstance()->GetTranslate().z });
	player_->SetTranslate({ Camera::GetInstance()->GetTranslate().x ,87.5f ,0.0f });
	whale_->SetTranslate({ 0.0f,0.0f,1.5f });
	for (uint32_t i = 0; i < 1; i++)
	{
		Particle* particle = new Particle;
		particle->SetElements(1.0f, 1.0f, 1.0f, 6.0f,
			-7.0f, 7.0f, -6.0f, -6.0f, -2.0f, -2.0f,
			0.0f, 0.0f, 6.0f, 12.0f, 0.0f, 0.0f);
		particle->Initialize("Resource/clearbabble.png", ParticleCommon::GetInstance(), SRVManager::GetInstance(), Object3dCommon::GetInstance(), particle->GetElements());
		particles.push_back(particle);
	}
	if (GameManager::stageNumber >= 2)
	{

	}
	
}

void GameScene::Update()
{
	if (isGameStart == true&&whale_->GetLife()!=0)//ゲーム中
	{
		time++;
		//if (time == 150)
		//{
		//	whale_->SetLife(0);
		//}
		if (uiPlanes[0]->GetTranslate().y >= -0.5f && uiPlanes[0]->GetTranslate().y <= 0.5f)
		{
			uiPlanes[0]->SetTranslate({ uiPlanes[0]->GetTranslate().x, uiPlanes[0]->GetTranslate().y + 0.1f, uiPlanes[0]->GetTranslate().z });
		}
		else if (uiPlanes[0]->GetTranslate().y >= 0.5f)
		{
			uiPlanes[0]->SetTranslate({ uiPlanes[0]->GetTranslate().x, uiPlanes[0]->GetTranslate().y + 0.2f, uiPlanes[0]->GetTranslate().z });
			uiPlanes[0]->SetRotate({ uiPlanes[0]->GetRotate().x,uiPlanes[0]->GetRotate().y + 0.1f,uiPlanes[0]->GetRotate().z });
		}
		else
		{
			uiPlanes[0]->SetTranslate({ uiPlanes[0]->GetTranslate().x, uiPlanes[0]->GetTranslate().y + 0.2f, uiPlanes[0]->GetTranslate().z });
		}
		uiPlanes[0]->Update();
		for (Particle* particle : particles)
		{
			particle->Update(false, particle->GetElements());
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
		UpdateEnemyPopCommands(GameManager::stageNumber);
		for (Enemy* enemy_ : enemys_)
		{
			enemy_->Update(enemy_->GetSort());
		}

		CheckAllCollisions();
		Camera::GetInstance()->SetTranslate({ player_->GetTranslate().x,player_->GetTranslate().y + 3.0f,player_->GetTranslate().z - 20.0f });
		ground->SetTranslate({ ground->GetTranslate().x, ground->GetTranslate().y, ground->GetTranslate().z - 0.1f });

	}
	else if (isGameOver == true)//GameOverSceneへ遷移
	{
		if(Camera::GetInstance()->GetTranslate().z >= zoomPos.z-0.1f)
		{
			if (sceneTransitionTime == -90)
			{
				sceneNo = GAMEOVER;
			}
			if(sceneTransitionTime <= -30)
			{
				whale_->SetTranslate({ whale_->GetTranslate().x,whale_->GetTranslate().y - 0.05f,whale_->GetTranslate().z });
				uiPlanes[1]->SetColor({ uiPlanes[1]->GetColor().x,uiPlanes[1]->GetColor().y,uiPlanes[1]->GetColor().z,uiPlanes[1]->GetColor().a + 0.15f });
				uiPlanes[1]->Update();
			}
			sceneTransitionTime--;
		}
		else
		{
			if(isGameStart)
			{
				zoomPos = { whale_->GetTranslate().x, whale_->GetTranslate().y+1.0f, whale_->GetTranslate().z - 10.0f };
				v = Subtract(zoomPos, Camera::GetInstance()->GetTranslate());
				isGameStart = false;
			}
			Camera::GetInstance()->SetTranslate(
				{ 
				Camera::GetInstance()->GetTranslate().x + v.x / 50.0f,
				Camera::GetInstance()->GetTranslate().y + v.y / 50.0f,
				Camera::GetInstance()->GetTranslate().z + v.z / 50.0f 
				});
		}
	}
	else if (isGameClear == true)//GameClearSceneへ遷移
	{
		sceneTransitionTime++;
	}
	else//遷移中
	{
		sceneTransitionTime++;
		if (sceneTransitionTime <= 60)
		{
			player_->SetTranslate({ player_->GetTranslate().x,player_->GetTranslate().y - 1.25f, player_->GetTranslate().z });
			Camera::GetInstance()->SetTranslate({ Camera::GetInstance()->GetTranslate().x, Camera::GetInstance()->GetTranslate().y - 1.0f, Camera::GetInstance()->GetTranslate().z });
			uiPlanes[1]->SetTranslate({ Camera::GetInstance()->GetTranslate().x, Camera::GetInstance()->GetTranslate().y, Camera::GetInstance()->GetTranslate().z + 2.0f });
			uiPlanes[1]->SetColor({ uiPlanes[1]->GetColor().x,uiPlanes[1]->GetColor().y,uiPlanes[1]->GetColor().z,uiPlanes[1]->GetColor().a - 0.02f });
			uiPlanes[1]->Update();
		}
		if (sceneTransitionTime >= 61 && sceneTransitionTime <= 80)
		{
			player_->SetTranslate({ player_->GetTranslate().x,player_->GetTranslate().y - 1.25f / 2, player_->GetTranslate().z });
			Camera::GetInstance()->SetTranslate({ Camera::GetInstance()->GetTranslate().x, Camera::GetInstance()->GetTranslate().y - 0.5f, Camera::GetInstance()->GetTranslate().z });
		}
		if (sceneTransitionTime >= 120)
		{
			isGameStart = true;
			sceneTransitionTime = 0;
		}
	}
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
		isGameOver = true;
	}
	else if (enemys_.size() == 0 && gameEnd || Input::GetInstance()->TriggerKey(DIK_RETURN))
	{
		enemys_.resize(0);
		sceneNo = CLEAR;
		isGameClear = true;

	}
	else if (Input::GetInstance()->TriggerKey(DIK_S))
	{
#ifdef DEBUG
		enemys_.resize(0);
		sceneNo = TITLE;
#endif // DEBUG
	}

	ground->Update();
	player_->Update();
	whale_->Update();
	for (Enemy* enemy_ : enemys_)
	{
		enemy_->Update(enemy_->GetSort());
	}

	
}

void GameScene::Draw()
{
	ground->Draw();
	player_->Draw();
	whale_->Draw();
	uiPlanes[0]->Draw();
	if(isGameStart==false)
	{
		uiPlanes[1]->Draw();
	}
	for (Enemy* enemy_ : enemys_)
	{
		enemy_->Draw(enemy_->GetSort());
	}
	for (Particle* particle : particles)
	{
		particle->Draw();
	}

}

void GameScene::Finalize()
{
	for (UIPlane* uiPlane : uiPlanes)
	{
		delete uiPlane;
		uiPlane = NULL;
	}
	uiPlanes.clear();
	delete ground;
	ground = NULL;
	delete whale_;
	whale_ = NULL;
	player_->Finalize();
	delete player_;
	player_ = NULL;
	for (Enemy* enemy_ : enemys_)
	{
		delete enemy_;
		enemy_ = NULL;
	}
	enemys_.clear();
	for (uint32_t i = 0; i < 4; i++)
	{
		enemyPopFile[i].clear();
		enemyPopFile[i].shrink_to_fit();
		enemyPopCommands[i].str("");
		enemyPopCommands[i].clear(std::stringstream::goodbit);
	}
	AudioManager::GetInstance()->SoundUnload(&bgm);
	GameManager::stageNumber = 0;
}

void GameScene::CheckAllCollisions()
{
	Vector3 posA, posB;
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	if (player_->GetIsHit() == false && player_->GetIsHitTimer() == 0)
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
			std::uniform_real_distribution<float> enemySort(0.0f, (float)(GameManager::stageNumber + 1));
			Enemy* enemy_ = new Enemy;
			enemy_->SetSort((int)enemySort(randomEngine));
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
