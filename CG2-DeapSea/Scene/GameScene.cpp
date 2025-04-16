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
			"Resource/black_1.png",
			"Resource/moveKey.png",
	        "Resource/attackKey.png",
			"Resource/frontKey.png",
	    };

		for (uint32_t i = 0; i < NumArgument; i++)
		{
			std::unique_ptr<UIPlane> uiPlane = std::make_unique<UIPlane>();
			uiPlane->Initialize(Planes[i], PNGs[i]);
			uiPlanes_.push_back(std::move(uiPlane));
		}
	    uiPlanes_[Start]->SetScale({1.5f, 1.0f, 0.0f});
	    uiPlanes_[Start]->SetRotate({-1.6f, -3.2f, 0.0f});
	    uiPlanes_[Start]->SetTranslate({0.0f, 58.0f, camera_->GetTranslate().z - 1.0f});
		uiPlanes_[Start]->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		uiPlanes_[Blackout]->SetScale({ 10.0f,50.0f,0.0f });
	    uiPlanes_[Blackout]->SetTranslate({camera_->GetTranslate().x, camera_->GetTranslate().y, camera_->GetTranslate().z + zfar});
		uiPlanes_[Blackout]->SetColor({ 1.0f,1.0f,1.0f,1.0f });

		uiPlanes_[moveKey]->SetScale({ 2.5f,1.5f,0.0f });
		uiPlanes_[moveKey]->SetColor({ 1.0f,1.0f,1.0f,0.0f });
	    uiPlanes_[moveKey]->SetRotate({-1.6f, -3.2f, 0.0f});

		uiPlanes_[attackKey]->SetScale({2.5f, 1.5f, 0.0f});
	    uiPlanes_[attackKey]->SetColor({1.0f, 1.0f, 1.0f, 0.0f});
	    uiPlanes_[attackKey]->SetRotate({-1.6f, -3.2f, 0.0f});

		uiPlanes_[lookKey]->SetScale({2.5f, 1.5f, 0.0f});
	    uiPlanes_[lookKey]->SetColor({1.0f, 1.0f, 1.0f, 0.0f});
	    uiPlanes_[lookKey]->SetRotate({-1.6f, -3.2f, 0.0f});

	    enemyPopFile[0] = "Resource/CSV/practiceFile.csv";
		enemyPopFile[1] = "Resource/CSV/STAGE1File.csv";
		enemyPopFile[2] = "Resource/CSV/STAGE2File.csv";
		enemyPopFile[3] = "Resource/CSV/STAGE3File.csv";

		LoadEnemyPopData(enemyPopFile[SceneManager::GetInstance()->GetStageNumber()], SceneManager::GetInstance()->GetStageNumber());
		gameEnd = false;
		bgm = AudioManager::GetInstance()->SoundLoadWave("Resource/Sounds/stage1bgm.wav");
		//AudioManager::GetInstance().get()->SoundPlayWave(AudioManager::GetInstance().get()->GetxAudio2().Get(), bgm);
		time = 0;

		isGameStart = false;
		isGameOver = false;
		isGameClear = false;
	    isGameOverScene = false;
	    isGameClearScene = false;

		isMove = false;
	    isAttack = false;
	    isLook = false;
	    isPose = false;

		sceneTransitionTime = 0;
		player_->SetTranslate({ camera_->GetTranslate().x ,87.5f ,0.0f });
		whale_->SetTranslate({ 0.0f,0.0f,1.5f });
	    distance = camera_->GetTranslate().z - player_->GetTranslate().z;

		particle_ = std::make_unique<Particle>();
		particle_->SetElements(1.0f, 1.0f, 1.0f, 6.0f,
			-7.0f, 7.0f, 55.0f, 55.0f, -4.0f, -4.0f,
			0.0f, 0.0f, 0.0f, 0.0f, 0.1f, 0.2f);
		particle_->Initialize("Resource/clearbabble.png", particle_->GetElements(),100);
		//if (BaseScene::stageNumber >= 2)
		//{
		//
		//}
		//for (uint32_t i = 0; i < 1; i++)
		//{
		//	std::unique_ptr<Sprite> sprite = std::make_unique<Sprite>();
		//	sprite->Initialize("Resource/key.png");
		//	sprite->SetPosition(spritePos);
		//	sprite->SetSize({ sprite->GetSize().x * 1.2f,sprite->GetSize().y * 1.2f });
		//	sprites_.push_back(std::move(sprite));
		//}
	}

	std::unique_ptr<BaseScene> GameScene::Update()
	{
		if (isGameStart == true && whale_->GetLife() != 0)//gameScene
		{
			time++;

#ifdef _DEBUG
			//if (time == 150)
			//{
			//	isGameStart = false;
			//	isGameClear = true;
			//	//whale_->SetLife(0);
			//}
		 //   if (Input::GetInstance()->PushKey(DIK_D))
			//{
			//    camera_->SetRotate({camera_->GetRotate().x, camera_->GetRotate().y, camera_->GetRotate().z + 0.1f});
		 //   }
		 //   if (Input::GetInstance()->PushKey(DIK_A))
			//{
			//    camera_->SetRotate({camera_->GetRotate().x, camera_->GetRotate().y, camera_->GetRotate().z - 0.1f});
		 //   }
		 //   if (Input::GetInstance()->PushKey(DIK_W))
			//{
			//    camera_->SetRotate({camera_->GetRotate().x + 0.1f, camera_->GetRotate().y, camera_->GetRotate().z});
		 //   }
		 //   if (Input::GetInstance()->PushKey(DIK_S)) 
			//{
			//    camera_->SetRotate({camera_->GetRotate().x - 0.1f, camera_->GetRotate().y, camera_->GetRotate().z});
		 //   }
		 //   offset = Subtract(camera_->GetTranslate(), player_->GetTranslate());
		 //   camera_->Offset({offset.x, offset.z, offset.y}, player_->GetTranslate());
#endif
		    if (SceneManager::GetInstance()->GetStageNumber() == 0 && time > 60)
			{

				if (Input::GetInstance()->PushKey(DIK_RIGHT)||Input::GetInstance()->PushKey(DIK_LEFT)||Input::GetInstance()->PushKey(DIK_UP)||Input::GetInstance()->PushKey(DIK_DOWN))
				{
					isMove = true;
				}
			    if (Input::GetInstance()->PushKey(DIK_SPACE))
				{
				    isAttack = true;
			    }
			    if (Input::GetInstance()->PushKey(DIK_F))
				{
				    isLook = true;
			    }

				if (isMove == false)
				{
				    //uiPlanes_[moveKey]->SetRotate(camera_->GetRotate());
				    uiPlanes_[moveKey]->SetTranslate({player_->GetTranslate().x, player_->GetTranslate().y + 40.0f, player_->GetTranslate().z - 1.0f});
					uiPlanes_[moveKey]->SetColor({ 1.0f,1.0f,1.0f,1.0f });
				}
				else
				{
				    uiPlanes_[moveKey]->SetTranslate({uiPlanes_[moveKey]->GetTranslate().x, uiPlanes_[moveKey]->GetTranslate().y, uiPlanes_[moveKey]->GetTranslate().z + 0.01f});
					uiPlanes_[moveKey]->SetColor({ 1.0f,1.0f,1.0f,uiPlanes_[moveKey]->GetColor().a-0.02f});
				}
				if (isAttack == false)
				{
				    //uiPlanes_[attackKey]->SetRotate(camera_->GetRotate());
				    uiPlanes_[attackKey]->SetTranslate({player_->GetTranslate().x, player_->GetTranslate().y + 40.0f, player_->GetTranslate().z - 2.0f});
				    uiPlanes_[attackKey]->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
			    } 
				else
				{
				    uiPlanes_[attackKey]->SetTranslate({uiPlanes_[attackKey]->GetTranslate().x, uiPlanes_[attackKey]->GetTranslate().y, uiPlanes_[attackKey]->GetTranslate().z + 0.01f});
				    uiPlanes_[attackKey]->SetColor({1.0f, 1.0f, 1.0f, uiPlanes_[attackKey]->GetColor().a - 0.02f});
			    }
			    if (isLook == false)
				{
				    //uiPlanes_[lookKey]->SetRotate({camera_->GetRotate().x, camera_->GetRotate().y - std::numbers::pi_v<float>, camera_->GetRotate().z});
				    uiPlanes_[lookKey]->SetTranslate({player_->GetTranslate().x, player_->GetTranslate().y + 40.0f, player_->GetTranslate().z - 3.0f});
				    uiPlanes_[lookKey]->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
			    } 
				else
				{
				    uiPlanes_[lookKey]->SetTranslate({uiPlanes_[lookKey]->GetTranslate().x, uiPlanes_[lookKey]->GetTranslate().y, uiPlanes_[lookKey]->GetTranslate().z + 0.01f});
				    uiPlanes_[lookKey]->SetColor({1.0f, 1.0f, 1.0f, uiPlanes_[lookKey]->GetColor().a - 0.02f});
			    }
			    uiPlanes_[moveKey]->Update();
			    uiPlanes_[attackKey]->Update();
			    uiPlanes_[lookKey]->Update();
		    }

		    if (Input::GetInstance()->PushKey(DIK_F))
			{
			    if (camera_->GetRotate().x >= 1.4f)
				{
				    camera_->SetRotate({camera_->GetRotate().x - 0.01f, camera_->GetRotate().y, camera_->GetRotate().z});
			    }
		    }
			else
			{
			    if (camera_->GetRotate().x <= camera_->GetDefaultAngle().x)
				{
				    camera_->SetRotate({camera_->GetRotate().x + 0.01f, camera_->GetRotate().y, camera_->GetRotate().z});
			    }
		    }

			if (distanceCtoUI.z >= -1.0f && distanceCtoUI.z <= 1.0f)
			{
			    distanceCtoUI.z += 0.1f;
			}
			else if (distanceCtoUI.z >=  1.0f)
			{
			    uiPlanes_[Start]->SetRotate({uiPlanes_[Start]->GetRotate().x, uiPlanes_[Start]->GetRotate().y, uiPlanes_[Start]->GetRotate().z + 0.1f});
			    distanceCtoUI.z += 0.2f;
		    }
			else
			{
			    distanceCtoUI.z += 0.5f;
		    }
		    uiPlanes_[Start]->SetTranslate(Add(camera_->GetTranslate(), distanceCtoUI));
			uiPlanes_[Start]->Update();
			particle_->Update(false, particle_->GetElements());

			player_->SetMaxPosition(player_->GetTranslate().x, 20.0f);
			whale_->SetMaxPosition(whale_->GetTranslate().x,0.0f, 10.0f);
		    whale_->SetMaxPosition(whale_->GetTranslate().x, player_->GetTranslate().x, whale_->GetMaxDistance());
		    whale_->SetMaxPosition(whale_->GetTranslate().z, player_->GetTranslate().z, whale_->GetMaxDistance());

			UpdateEnemyPopCommands(SceneManager::GetInstance()->GetStageNumber());
			for (const auto& enemy_ : enemys_)
			{
				enemy_->Update(enemy_->GetSort());
			}

			CheckAllCollisions();
		    camera_->SetTranslate({player_->GetTranslate().x, camera_->GetTranslate().y, player_->GetTranslate().z});
			//ground_->SetTranslate({ ground_->GetTranslate().x, ground_->GetTranslate().y, ground_->GetTranslate().z - 0.1f });
			tide_->SetTranslate({ tide_->GetTranslate().x, tide_->GetTranslate().y, tide_->GetTranslate().z - 0.1f });
		}
		else if (isGameOver == true)//GameOverSceneへ遷移
		{
		    zoomPos = {whale_->GetTranslate().x, whale_->GetTranslate().y + 1.0f, whale_->GetTranslate().z - 10.0f};
		    player_->SetIsMovable(false);
			if (camera_->GetTranslate().y >= 0.0f)
			{
			    float rotateX = camera_->GetRotate().x;
			    camera_->SetRotate({rotateX - 0.02f, camera_->GetRotate().y, camera_->GetRotate().z});
			    camera_->Offset({0.0f, 2.0f, -distance}, whale_->GetTranslate());
		    }
			else if (camera_->GetTranslate().y <= 0.0f)
			{
			    if (gameOverSceneTransitionTime == 90)
				{
				    isGameOverScene = true;
				}
			    if (gameOverSceneTransitionTime >= 30)
				{
					whale_->SetTranslate({ whale_->GetTranslate().x,whale_->GetTranslate().y - 0.05f,whale_->GetTranslate().z });
					uiPlanes_[Blackout]->SetTranslate({ camera_->GetTranslate().x, camera_->GetTranslate().y, camera_->GetTranslate().z + 2.0f });
				    uiPlanes_[Blackout]->SetRotate(camera_->GetRotate());
					uiPlanes_[Blackout]->SetColor({ uiPlanes_[Blackout]->GetColor().x,uiPlanes_[Blackout]->GetColor().y,uiPlanes_[Blackout]->GetColor().z,uiPlanes_[Blackout]->GetColor().a + 0.02f });
					uiPlanes_[Blackout]->Update();
				}
			    if (camera_->GetTranslate().z - whale_->GetTranslate().z <= -10.0f)
				{
				    camera_->SetTranslate({whale_->GetTranslate().x, camera_->GetTranslate().y, camera_->GetTranslate().z + 2.0f});
			    }
			    gameOverSceneTransitionTime++;
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

			if (clearSceneTransitionTime<=60)
			{
			    float pRotateY = player_->GetRotate().y;
			    player_->SetRotate({player_->GetRotate().x, pRotateY - 0.1f, camera_->GetRotate().z});
			    whale_->SetTranslate({whale_->GetTranslate().x, player_->GetTranslate().y, whale_->GetTranslate().z});
			    player_->SetTranslate({player_->GetTranslate().x, player_->GetTranslate().y + 0.5f, player_->GetTranslate().z});
			    Vector3 distanceWtoP = Subtract(player_->GetTranslate(), whale_->GetTranslate());
			    float wRotateY = whale_->GetRotate().y;
			    whale_->SetRotate({whale_->GetRotate().x, wRotateY - 0.1f, whale_->GetRotate().z});
			    whale_->SetTranslate(Revolution({distanceWtoP.x, distanceWtoP.y, distanceWtoP.z}, player_->GetTranslate(), whale_->GetRotate()));
		    }
			if (clearSceneTransitionTime >= 61)
			{
			    player_->SetTranslate({player_->GetTranslate().x, player_->GetTranslate().y, player_->GetTranslate().z + 1.25f});
			    whale_->SetTranslate({whale_->GetTranslate().x, whale_->GetTranslate().y, whale_->GetTranslate().z + 1.25f});
			    uiPlanes_[Blackout]->SetTranslate({camera_->GetTranslate().x, camera_->GetTranslate().y, camera_->GetTranslate().z + 2.0f});
			    uiPlanes_[Blackout]->SetRotate(camera_->GetRotate());
			    uiPlanes_[Blackout]->SetColor({uiPlanes_[Blackout]->GetColor().x, uiPlanes_[Blackout]->GetColor().y, uiPlanes_[Blackout]->GetColor().z, uiPlanes_[Blackout]->GetColor().a + 0.02f});
			    uiPlanes_[Blackout]->Update();
			}
			if (uiPlanes_[Blackout]->GetColor().a >= 1.0f)
			{
			    isGameClearScene = true;
			}
		    clearSceneTransitionTime++;

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
			    if (camera_->GetTranslate().z != 0.0f)
				{
				    float rotateX = camera_->GetRotate().x;
				    camera_->SetRotate({rotateX + 0.02f, camera_->GetRotate().y, camera_->GetRotate().z});
				    camera_->Offset({0.0f, 2.0f, distance}, player_->GetTranslate());
			    }
			    if (camera_->GetTranslate().z <= 0.2f && camera_->GetTranslate().z >= -0.02f)
				{
				    camera_->SetTranslate({camera_->GetTranslate().x, camera_->GetTranslate().y, 0.0f});
			    }
			    if (camera_->GetTranslate().z == 0.0f)
				{
				    camera_->SetTranslate({camera_->GetTranslate().x, camera_->GetTranslate().y + 0.4f, camera_->GetTranslate().z});
			    }
			    if (camera_->GetTranslate().y >= 70.0f)
				{
				    camera_->SetTranslate({camera_->GetTranslate().x, 70.0f, camera_->GetTranslate().z});
				    isGameStart = true;
				    sceneTransitionTime = 0;
				    player_->SetIsMovable(true);
				    camera_->SetDefaultAngle(camera_->GetRotate());
			    }
			}
		}

	    for (const auto& enemy_ : enemys_)
		{
		    if (enemy_->GetTranslate().z <= camera_->GetTranslate().z - enemyDeadLine)
			{
			    enemy_->OnCollision();
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

		if (whale_->GetLife() == 0 && isGameOver == false)
		{
			enemys_.resize(0);
		    distance = camera_->GetTranslate().y - whale_->GetTranslate().y;
			isGameOver = true;
		}
	    else if(enemys_.size() == 0 && gameEnd == true)
		{
		    isGameStart = false;
		    isGameClear = true;
		}
#ifdef _DEBUG
		else if (enemys_.size() == 0 && gameEnd == true || Input::GetInstance()->TriggerKey(DIK_RETURN))
		{
		    enemys_.resize(0);
			isGameStart = false;
			isGameClear = true;
	    }
		else if (Input::GetInstance()->TriggerKey(DIK_K))
		{
		    whale_->SetLife(0);
		}
#endif // DEBUG

		if (isGameStart == false)
		{
			player_->SetIsShot(false);
		}
		ground_->Update();
		player_->Update(whale_->GetTranslate());
		whale_->Update();
		tide_->Update();
		for (const auto& enemy_ : enemys_)
		{
			enemy_->Update(enemy_->GetSort());
		}

		if (isGameOverScene == true)
		{
		    return std::make_unique<GameOverScene>();
		}
		else if (isGameClearScene == true)
		{
		    return std::make_unique<ClearScene>();
		}
		else
		{
		    return nullptr;
		}
	}

	void GameScene::Draw()
	{
		ground_->Draw();
		whale_->Draw();
		player_->Draw();
		for (uint32_t i = 0; i < NumArgument; i++)
		{
			uiPlanes_[i]->Draw();
		}
		for (const auto& enemy_ : enemys_)
		{
			enemy_->Draw(enemy_->GetSort());
		}
		particle_->Draw();
		tide_->Draw();
	}

	void GameScene::CheckAllCollisions()
	{
	    const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player_->GetBullets();
	    const std::unique_ptr<Shield>& shield = player_->GetShield();
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

	    if (shield!=nullptr&&shield->IsDead() == false)
		{
#pragma region シールドと敵の当たり判定
		    for (const auto& enemy_ : enemys_)
			{
			    if (isCollision(enemy_->GetCollision(), shield->GetCollision()))
				{
				    enemy_->OnCollision();
			    }
		    }
#pragma endregion

#pragma region シールドと敵弾の当たり判定
		    for (const auto& enemy_ : enemys_)
			{
			    const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy_->GetBullets();
			    for (const auto& bullet : enemyBullets)
				{
				    if (isCollision(shield->GetCollision(), bullet->GetCollision()))
					{
					    bullet->OnCollision();
				    }
			    }
		    }
#pragma endregion
	    }

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
				float z = (float)std::atof(word.c_str()) + whale_->GetTranslate().z;

				std::random_device seedGenerator;
				std::mt19937 randomEngine(seedGenerator());
			    std::uniform_real_distribution<float> enemySort(0.0f, (float)(SceneManager::GetInstance()->GetStageNumber() + 1));
				std::unique_ptr<Enemy> enemy_ = std::make_unique<Enemy>();
				enemy_->SetSort((int)enemySort(randomEngine));
				enemy_->Initialize(player_.get(), whale_.get(), enemy_->GetSort());
			    if (SceneManager::GetInstance()->GetStageNumber() == 0)
				{
					enemy_->SetIsPractice(true);
				}
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