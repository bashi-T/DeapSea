#include "Enemy.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

	Enemy::~Enemy()
	{
		eBullets.clear();
	}

	void Enemy::Initialize(Player* player, Whale* whale, int sort)
	{
		player_ = player;
		whale_ = whale;
		object3d = std::make_unique<Object3d>();
		switch (sort)
		{
		case 0:
		{
			std::string enemyModel = "fish/improvisedFish.obj";
			const std::string enemySkin = "Resource/uvChecker.png";
			object3d->Initialize(Object3dCommon::GetInstance(), SRVManager::GetInstance());
			ModelManager::GetInstance()->LoadModel(enemyModel, enemySkin, true);
			object3d->SetModel(enemyModel);
			Model* model = ModelManager::GetInstance()->FindModel(enemyModel);
			Model::ModelData* modelData = model->GetModelData();
			for (Model::VertexData& vertex : modelData->vertices)
			{
				vertex.normal.x = vertex.position.x;
				vertex.normal.y = vertex.position.y;
				vertex.normal.z = vertex.position.z;
			}
			model->Memcpy();
			life = 1;
			break;
		}
		case 1:
		{
			moveInterval = 0;
			std::string enemyModel = "straight/improvisedStraight.obj";
			const std::string enemySkin = "Resource/uvChecker.png";
			object3d->Initialize(Object3dCommon::GetInstance(), SRVManager::GetInstance());
			ModelManager::GetInstance()->LoadModel(enemyModel, enemySkin, true);
			object3d->SetModel(enemyModel);
			Model* model = ModelManager::GetInstance()->FindModel(enemyModel);
			Model::ModelData* modelData = model->GetModelData();
			for (Model::VertexData& vertex : modelData->vertices)
			{
				vertex.normal.x = vertex.position.x;
				vertex.normal.y = vertex.position.y;
				vertex.normal.z = vertex.position.z;
			}
			model->Memcpy();
			life = 1;
			break;
		}
		}
	}

	void Enemy::Update(int sort)
	{
		eBullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet)
			{
				if (bullet->IsDead())
				{
					bullet.reset();
					return true;
				}
				return false;
			});

		switch (sort)
		{
		case 0:
			if (life <= 0)
			{
				escapeTime++;
				if (player_->GetTranslate().x >= object3d->GetTranslate().x)
				{
					enemyVector = { -2.0f,1.0f,2.0f };
					object3d->SetRotate({ -1.0f,0.5f,-0.2f });
				}
				else
				{
					enemyVector = { 2.0f,1.0f,2.0f };
					object3d->SetRotate({ 1.0f,0.5f,-0.2f });
				}
				if (escapeTime>=180)
				{
					isDead = true;
				}
				object3d->SetTranslate(Add(object3d->GetTranslate(), Multiply(0.05f, enemyVector)));

			}
			else
			{
				if (object3d->GetTranslate().z > player_->GetTranslate().z)
				{
					Shot();
				}
				if (shotInterval == 1 && object3d->GetTranslate().z > whale_->GetTranslate().z)
				{
					SetEnemyVector(whale_->GetTranslate());
				}
				else if (shotInterval == 1 && enemyVector.z > 0.0f)
				{
					SetEnemyVector({ enemyVector.x,enemyVector.y,-1.0f });
				}
				object3d->SetTranslate(Add(object3d->GetTranslate(), Multiply(0.05f, enemyVector)));
			}
			object3d->Update(Camera::GetInstance());
			eCollision =
			{
				.min{object3d->GetTranslate().x - 0.1f,object3d->GetTranslate().y - 0.25f,object3d->GetTranslate().z - 1.5f},
				.max{object3d->GetTranslate().x + 0.1f,object3d->GetTranslate().y + 0.25f,object3d->GetTranslate().z + 1.5f}
			};

			for (const auto& bullet : eBullets)
			{
				bullet->Update();
			}
			break;
		case 1:
			if (life <= 0)
			{
				escapeTime++;
				if (player_->GetTranslate().x >= object3d->GetTranslate().x)
				{
					enemyVector = { -2.0f,2.0f,2.0f };
					object3d->SetRotate({ -1.0f,0.5f,-0.2f });
				}
				else
				{
					enemyVector = { 2.0f,2.0f,2.0f };
					object3d->SetRotate({ 1.0f,0.5f,-0.2f });
				}
				if (escapeTime >= 180)
				{
					isDead = true;
				}
				object3d->SetTranslate(Add(object3d->GetTranslate(), Multiply(0.05f, { enemyVector.x * 2,enemyVector.y,enemyVector.z * 2 })));
			}
			else
			{
				moveInterval++;
				if (moveInterval == 120)
				{
					moveInterval = 0;
				}
				if (moveInterval == 1 && object3d->GetTranslate().z > whale_->GetTranslate().z)
				{
					SetEnemyVector(whale_->GetTranslate());
				}
				else if (moveInterval == 1 && enemyVector.z > 0.0f)
				{
					SetEnemyVector({ enemyVector.x,enemyVector.y,-1.0f });
				}
				object3d->SetTranslate(Add(object3d->GetTranslate(), Multiply(0.05f, { enemyVector.x * 2,enemyVector.y,enemyVector.z * 2 })));
			}
			object3d->Update(Camera::GetInstance());
			eCollision =
			{
				.min{object3d->GetTranslate().x - 0.05f,object3d->GetTranslate().y - 0.4f,object3d->GetTranslate().z - 2.5f},
				.max{object3d->GetTranslate().x + 0.05f,object3d->GetTranslate().y + 0.4f,object3d->GetTranslate().z + 2.5f}
			};
			break;
		}
	}

	void Enemy::Draw(int sort)
	{
		switch (sort)
		{
		case 0:
			object3d->Draw(ModelManager::GetInstance()->GetModelCommon());
			for (const auto& bullet : eBullets)
			{
				bullet->Draw();
			}
			break;
		case 1:
			object3d->Draw(ModelManager::GetInstance()->GetModelCommon());
			break;
		}
	}

	void Enemy::Shot()
	{
		shotInterval++;
		if (shotInterval == 1)
		{
			std::unique_ptr<EnemyBullet> newBullet = std::make_unique< EnemyBullet>();
			newBullet->Initialize(object3d->GetTranslate());
			newBullet->SetEnemyBulletVector(player_->GetTranslate());
			eBullets.push_back(std::move(newBullet));
		}
		if (shotInterval == 180)
		{
			shotInterval = 0;
		}
	}

	void Enemy::OnCollision()
	{
		SubtractLife();
		//if (life <= 0)
		//{
		//	isDead = true;
		//}
	}

	void Enemy::SetTranslate(Vector3 translate)
	{
		object3d->SetTranslate(translate);
	}

	void Enemy::SetEnemyVector(Vector3 position)
	{
		enemyVector = Normalize(Subtract(position, object3d->GetTranslate()));
	}