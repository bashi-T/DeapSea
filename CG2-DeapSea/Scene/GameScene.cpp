#include "GameScene.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

namespace MyEngine
{
	void GameScene::Initialize()
	{
		camera_ = Camera::GetInstance();
		player_ = std::make_unique<Player>();
		whale_ = std::make_unique<Whale>();
		ground_ = std::make_unique<Ground>();
		tide_ = std::make_unique<Tide>();

		player_->Initialize(camera_);
		player_->SetIsMovable(false);
		whale_->Initialize(player_.get());
		ground_->Initialize();
		tide_->Initialize();

		camera_->SetTranslate({ camera_->GetTranslate().x, ingameCameraY, camera_->GetTranslate().z });
		camera_->SetRotate(ingameCameraRotate);

		std::string PNGs[NumArgument] =
		{
			"Resource/startUI.png",
			"Resource/black.png",
		};

		for (uint32_t i = 0; i < NumArgument; i++)
		{
			std::unique_ptr<UIPlane> uiPlane = std::make_unique<UIPlane>();
			uiPlane->Initialize(Planes[i], PNGs[i]);
			uiPlanes_.push_back(std::move(uiPlane));
		}
		uiPlanes_[Start]->SetScale({ 1.5f,1.0f,0.0f });
		uiPlanes_[Start]->SetTranslate({ 0.0f, -3.0f,camera_->GetTranslate().z + 10.0f });
		uiPlanes_[Start]->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		uiPlanes_[Blackout]->SetScale({ 10.0f,50.0f,0.0f });
		uiPlanes_[Blackout]->SetTranslate({ camera_->GetTranslate().x, camera_->GetTranslate().y, camera_->GetTranslate().z + zfar });
		uiPlanes_[Blackout]->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		enemyPopFile[0] = "Resource/CSV/practiceFile.csv";
		enemyPopFile[1] = "Resource/CSV/STAGE1File.csv";
		enemyPopFile[2] = "Resource/CSV/STAGE2File.csv";
		enemyPopFile[3] = "Resource/CSV/STAGE3File.csv";

		LoadEnemyPopData(enemyPopFile[GameManager::stageNumber], GameManager::stageNumber);
		gameEnd = false;
		bgm = AudioManager::GetInstance()->SoundLoadWave("Resource/Sounds/stage1bgm.wav");
		//AudioManager::GetInstance().get()->SoundPlayWave(AudioManager::GetInstance().get()->GetxAudio2().Get(), bgm);
		time = 0;

		isGameStart = false;
		isGameOver = false;
		isGameClear = false;
		sceneTransitionTime = 0;
		player_->SetTranslate({ camera_->GetTranslate().x ,87.5f ,0.0f });
		whale_->SetTranslate({ 0.0f,0.0f,1.5f });
		particle_ = std::make_unique<Particle>();
		particle_->SetElements(1.0f, 1.0f, 1.0f, 6.0f,
			-7.0f, 7.0f, -5.0f, -5.0f, -2.0f, -2.0f,
			0.0f, 0.0f, 6.0f, 12.0f, 0.0f, 1.0f);
		particle_->Initialize("Resource/clearbabble.png", particle_->GetElements(),100);
		//if (GameManager::stageNumber >= 2)
		//{

		//}
		for (uint32_t i = 0; i < 1; i++)
		{
			std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
			sprite->Initialize("Resource/key.png");
			sprite->SetPosition(spritePos);
			//sprite->SetSize({ sprite->GetSize().x * 2.0f,sprite->GetSize().y * 2.0f });
			sprites_.push_back(std::move(sprite));
		}
	}

	void GameScene::Update()
	{
		if (GameManager::stageNumber == 0)
		{
			sprites_[0]->Update();
		}
		if (isGameStart == true && whale_->GetLife() != 0)//gameScene
		{
#ifdef _DEBUG
			//time++;
			//if (time == 150)
			//{
			//	//isGameStart = false;
			//	//isGameClear = true;
			//	whale_->SetLife(0);
			//}
#endif
			if (uiPlanes_[Start]->GetTranslate().y >= 2.0f && uiPlanes_[Start]->GetTranslate().y <= 3.0f)
			{
				uiPlanes_[Start]->SetTranslate({ uiPlanes_[Start]->GetTranslate().x, uiPlanes_[Start]->GetTranslate().y + 0.1f,camera_->GetTranslate().z + 10.0f });
			}
			else if (uiPlanes_[Start]->GetTranslate().y >= 3.0f)
			{
				uiPlanes_[Start]->SetTranslate({ uiPlanes_[Start]->GetTranslate().x, uiPlanes_[Start]->GetTranslate().y + 0.2f, camera_->GetTranslate().z + 10.0f });
				uiPlanes_[Start]->SetRotate({ uiPlanes_[Start]->GetRotate().x,uiPlanes_[Start]->GetRotate().y + 0.1f,uiPlanes_[Start]->GetRotate().z });
			}
			else
			{
				uiPlanes_[Start]->SetTranslate({ uiPlanes_[Start]->GetTranslate().x, uiPlanes_[Start]->GetTranslate().y + 0.2f,camera_->GetTranslate().z + 10.0f });
			}
			uiPlanes_[Start]->Update();
			particle_->Update(false, particle_->GetElements());

			player_->SetMaxPosition(player_->GetTranslate().x, 10.0f);
			whale_->SetMaxPosition(whale_->GetTranslate().x, player_->GetTranslate().x + whale_->GetMaxDistance());
			whale_->SetMaxPosition(whale_->GetTranslate().x, player_->GetTranslate().x - whale_->GetMaxDistance());
			whale_->SetMaxPosition(whale_->GetTranslate().z, player_->GetTranslate().z + whale_->GetMaxDistance());
			whale_->SetMaxPosition(whale_->GetTranslate().z, player_->GetTranslate().z - whale_->GetMaxDistance());
			whale_->SetMaxPosition(whale_->GetTranslate().x, 10.0f);

			UpdateEnemyPopCommands(GameManager::stageNumber);
			for (const auto& enemy_ : enemys_)
			{
				enemy_->Update(enemy_->GetSort());
			}

			CheckAllCollisions();
			camera_->SetTranslate({ player_->GetTranslate().x,player_->GetTranslate().y + 6.0f,player_->GetTranslate().z - 20.0f });
			ground_->SetTranslate({ ground_->GetTranslate().x, ground_->GetTranslate().y, ground_->GetTranslate().z - 0.1f });
			tide_->SetTranslate({ tide_->GetTranslate().x, tide_->GetTranslate().y, tide_->GetTranslate().z - 0.1f });
		}
		else if (isGameOver == true)//GameOverSceneへ遷移
		{
			player_->SetIsMovable(false);
			if (camera_->GetTranslate().z >= zoomPos.z - 0.1f)
			{
				if (sceneTransitionTime == -90)
				{
					sceneNo = GAMEOVER;
				}
				if (sceneTransitionTime <= -30)
				{
					whale_->SetTranslate({ whale_->GetTranslate().x,whale_->GetTranslate().y - 0.05f,whale_->GetTranslate().z });
					uiPlanes_[Blackout]->SetTranslate({ camera_->GetTranslate().x, camera_->GetTranslate().y, camera_->GetTranslate().z + 2.0f });
					uiPlanes_[Blackout]->SetColor({ uiPlanes_[Blackout]->GetColor().x,uiPlanes_[Blackout]->GetColor().y,uiPlanes_[Blackout]->GetColor().z,uiPlanes_[Blackout]->GetColor().a + 0.02f });
					uiPlanes_[Blackout]->Update();
				}
				sceneTransitionTime--;
			}
			else
			{
				if (isGameStart)
				{
					zoomPos = { whale_->GetTranslate().x, whale_->GetTranslate().y + 1.0f, whale_->GetTranslate().z - 10.0f };
					v = Subtract(zoomPos, camera_->GetTranslate());
					isGameStart = false;
				}
				camera_->SetTranslate(
					{
					camera_->GetTranslate().x + v.x / 50.0f,
					camera_->GetTranslate().y + v.y / 50.0f,
					camera_->GetTranslate().z + v.z / 50.0f
					});
			}
		}
		else if (isGameClear == true)//GameClearSceneへ遷移
		{
			player_->SetIsMovable(false);


			if (sceneTransitionTime == -90)
			{
				sceneNo = CLEAR;
			}
			if (sceneTransitionTime <= -35)
			{
				whale_->SetTranslate({ whale_->GetTranslate().x, whale_->GetTranslate().y - 1.0f, whale_->GetTranslate().z });
			}
			if (sceneTransitionTime <= -30)
			{
				uiPlanes_[Blackout]->SetTranslate({ camera_->GetTranslate().x, camera_->GetTranslate().y, camera_->GetTranslate().z + 2.0f });
				uiPlanes_[Blackout]->SetColor({ uiPlanes_[Blackout]->GetColor().x,uiPlanes_[Blackout]->GetColor().y,uiPlanes_[Blackout]->GetColor().z,uiPlanes_[Blackout]->GetColor().a + 0.02f });
				uiPlanes_[Blackout]->Update();
				player_->SetTranslate({ player_->GetTranslate().x, player_->GetTranslate().y - 1.0f , player_->GetTranslate().z });
			}
			if (sceneTransitionTime <= -20 && sceneTransitionTime >= -30)
			{
				player_->SetTranslate({ player_->GetTranslate().x, player_->GetTranslate().y + 0.2f , player_->GetTranslate().z });
			}

			if (player_->GetTranslate().z <= ground_->GetTranslate().z + 70.0f)
			{
				player_->SetTranslate({ player_->GetTranslate().x, player_->GetTranslate().y, player_->GetTranslate().z + 2.0f });
				whale_->SetTranslate({ whale_->GetTranslate().x, whale_->GetTranslate().y, whale_->GetTranslate().z + 2.0f });
				sceneTransitionTime++;
			}
			sceneTransitionTime--;

			uiPlanes_[Blackout]->Update();
		}
		else//導入遷移中
		{
			sceneTransitionTime++;
			if (sceneTransitionTime <= 60)
			{
				player_->SetTranslate({ player_->GetTranslate().x,player_->GetTranslate().y - 1.25f, player_->GetTranslate().z });
				camera_->SetTranslate({ camera_->GetTranslate().x, camera_->GetTranslate().y - 1.0f, camera_->GetTranslate().z });
				uiPlanes_[Blackout]->SetTranslate({ camera_->GetTranslate().x, camera_->GetTranslate().y, camera_->GetTranslate().z + 2.0f });
				uiPlanes_[Blackout]->SetColor({ uiPlanes_[Blackout]->GetColor().x,uiPlanes_[Blackout]->GetColor().y,uiPlanes_[Blackout]->GetColor().z,uiPlanes_[Blackout]->GetColor().a - 0.02f });
				uiPlanes_[Blackout]->Update();
			}
			if (sceneTransitionTime >= 61 && sceneTransitionTime <= 80)
			{
				player_->SetTranslate({ player_->GetTranslate().x,player_->GetTranslate().y - 1.25f / 2, player_->GetTranslate().z });
				camera_->SetTranslate({ camera_->GetTranslate().x, camera_->GetTranslate().y - 0.5f,camera_->GetTranslate().z });
			}
			if (sceneTransitionTime >= 120)
			{
				isGameStart = true;
				sceneTransitionTime = 0;
				player_->SetIsMovable(true);
			}
		}
		enemys_.remove_if([](std::unique_ptr<Enemy>& enemy)
			{
				if (enemy->IsDead())
				{
					enemy.reset();
					return true;
				}
				return false;
			});
		if (whale_->GetLife() == 0)
		{
			enemys_.resize(0);
			isGameOver = true;
		}
		else if (enemys_.size() == 0 && gameEnd == true || Input::GetInstance()->TriggerKey(DIK_RETURN))
		{
			enemys_.resize(0);
			isGameStart = false;
			isGameClear = true;
		}
		else if (Input::GetInstance()->TriggerKey(DIK_S))
		{
#ifdef DEBUG
			enemys_.resize(0);
			sceneNo = TITLE;
#endif // DEBUG
		}

		if (isGameStart == false)
		{
			player_->SetIsShot(false);
		}
		ground_->Update();
		player_->Update();
		whale_->Update();
		tide_->Update();
		for (const auto& enemy_ : enemys_)
		{
			enemy_->Update(enemy_->GetSort());
		}
	}

	void GameScene::Draw()
	{
		ground_->Draw();
		whale_->Draw();
		player_->Draw();
		for (int i = 0; i < NumArgument; i++)
		{
			uiPlanes_[i]->Draw();
		}
		for (const auto& enemy_ : enemys_)
		{
			enemy_->Draw(enemy_->GetSort());
		}
		particle_->Draw();
		tide_->Draw();
		if (GameManager::stageNumber == 0)
		{
			sprites_[0]->Draw();
		}
	}

	void GameScene::Finalize()
	{
		uiPlanes_.clear();
		player_.reset();
		whale_.reset();
		enemys_.clear();
		sprites_.clear();
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
		const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
		if (player_->GetIsHit() == false && player_->GetIsHitTimer() == 0)
		{
#pragma region 自機と敵弾の当たり判定
			for (const auto& enemy_ : enemys_)
			{
				const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();
				for (const auto& bullet : enemyBullets)
				{
					if (isCollision(player_->GetAABBCollision(), bullet->GetCollision()))
					{
						player_->OnCollision();
						bullet->OnCollision();
					}
				}
			}
#pragma endregion
#pragma region 自機と敵の当たり判定
			for (const auto& enemy_ : enemys_)
			{
				if (isCollision(enemy_->GetCollision(), player_->GetAABBCollision()))
				{
					player_->OnCollision();
				}
			}
#pragma endregion
		}
#pragma region 敵と自弾の当たり判定
		for (const auto& enemy_ : enemys_)
		{
			for (const auto& bullet : playerBullets)
			{
				if (isCollision(enemy_->GetCollision(), bullet->GetCollision()))
				{
					enemy_->OnCollision();
					bullet->OnCollision();
				}
			}
		}
#pragma endregion
#pragma region 自弾と敵弾の当たり判定
		for (const auto& enemy_ : enemys_)
		{
			const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();
			for (const auto& pBullet : playerBullets)
			{
				for (const auto& eBullet : enemyBullets)
				{
					if (isCollision(pBullet->GetCollision(), eBullet->GetCollision()))
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
			for (const auto& enemy_ : enemys_)
			{
				if (isCollision(whale_->GetCollision(), enemy_->GetCollision()))
				{
					whale_->OnCollision();
					enemy_->OnCollision();
					return;
				}
			}
#pragma endregion
#pragma region 敵弾とクジラの当たり判定
			for (const auto& enemy_ : enemys_)
			{
				const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();
				for (const auto& bullet : enemyBullets)
				{
					if (isCollision(whale_->GetCollision(), bullet->GetCollision()))
					{
						whale_->OnCollision();
						bullet->OnCollision();
						return;
					}
				}
			}
#pragma endregion
		}
#pragma region 潮流とクジラの当たり判定
		if (isCollision(whale_->GetCollision(), tide_->GetCollision()))
		{
			whale_->OnTideCollision(tide_->GetTideVector());
		}
#pragma endregion
	}

	void GameScene::LoadEnemyPopData(std::string filePath, int32_t fileNum)
	{
		std::ifstream file;
		file.open(filePath);
		assert(file.is_open());

		enemyPopCommands[fileNum] << file.rdbuf();
		file.close();
	}

	void GameScene::UpdateEnemyPopCommands(int32_t fileNum)
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
				std::unique_ptr<Enemy> enemy_ = std::make_unique<Enemy>();
				enemy_->SetSort((int)enemySort(randomEngine));
				enemy_->Initialize(player_.get(), whale_.get(), enemy_->GetSort());
				enemy_->SetTranslate({ x,y,z });
				enemy_->SetEnemyVector(whale_->GetTranslate());
				enemys_.push_back(std::move(enemy_));
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
}