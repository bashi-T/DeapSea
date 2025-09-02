#include "Enemy.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

	Enemy::~Enemy()
	{
		eBullets.clear();
	}

	void Enemy::Initialize(Player* player)
	{
		player_ = player;
		object3d_ = std::make_unique<Object3d>();
		modelManager_ = ModelManager::GetInstance();
	    isShot = false;
	    isDead = false;
	    isPractice_ = false;

			std::string enemyModel = "fish/improvisedFish.obj";
			const std::string enemySkin = "Resource/whale5.png";
			object3d_->Initialize();
			modelManager_->LoadModel(enemyModel, enemySkin, true);
			object3d_->SetModel(enemyModel);
			Model* model = modelManager_->FindModel(enemyModel);
			Model::ModelData* modelData = model->GetModelData();
			for (Model::VertexData& vertex : modelData->vertices)
			{
				vertex.normal.x = vertex.position.x;
				vertex.normal.y = vertex.position.y;
				vertex.normal.z = vertex.position.z;
			}
			model->Memcpy();
    }

	void Enemy::Update(int sort)
	{

		switch (sort) {
	    case 0:
		    if (life <= 0) {
			    object3d_->Update(Camera::GetInstance());
			    enemyCollision = {
			        .min{object3d_->GetTranslate().x - 0.1f, object3d_->GetTranslate().y - 0.25f, object3d_->GetTranslate().z - 1.5f},
			        .max{object3d_->GetTranslate().x + 0.1f, object3d_->GetTranslate().y + 0.25f, object3d_->GetTranslate().z + 1.5f}
                };

			    break;
		    case 1:
			    object3d_->Update(Camera::GetInstance());
			    enemyCollision = {
			        .min{object3d_->GetTranslate().x - 0.05f, object3d_->GetTranslate().y - 0.4f, object3d_->GetTranslate().z - 2.5f},
			        .max{object3d_->GetTranslate().x + 0.05f, object3d_->GetTranslate().y + 0.4f, object3d_->GetTranslate().z + 2.5f}
                };
			    break;
		    }
	    }

	}

	void Enemy::Draw(int sort)
	{
		switch (sort)
		{
		case 0:
			object3d_->Draw(modelManager_->GetModelCommon());
			break;
		case 1:
			object3d_->Draw(modelManager_->GetModelCommon());
			break;
		}
	}

	void Enemy::Shot()
	{
    }

    void Enemy::Move() { object3d_->SetTranslate(Add(object3d_->GetTranslate(), Multiply(0.05f, enemyVector))); }

    void Enemy::Run() { }

	void Enemy::OnCollision()
	{
	    particle_ = std::make_unique<Particle>();
	    particle_->SetElements(
	        1.0f, 1.0f, 1.0f, 2.0f, object3d_->GetTranslate().x, object3d_->GetTranslate().x, object3d_->GetTranslate().y, object3d_->GetTranslate().y,
			object3d_->GetTranslate().z, object3d_->GetTranslate().z, -0.05f, 0.05f, 0.0f, 0.0f, -0.05f, 0.05f);
	    particle_->Initialize("Resource/blue.png", particle_->GetElements(), 30);
	    particle_->SetScale({0.25f, 0.25f, 0.25f});
	    SubtractLife();
	}

	void Enemy::SetTranslate(Vector3 translate)
	{
		object3d_->SetTranslate(translate);
	}

	void Enemy::SetEnemyVector(Vector3 position)
	{
		enemyVector = Normalize(Subtract(position, object3d_->GetTranslate()));
	}