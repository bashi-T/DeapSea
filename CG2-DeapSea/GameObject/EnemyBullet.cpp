#include "EnemyBullet.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

	EnemyBullet::~EnemyBullet()
	{
	}

	void EnemyBullet::Initialize(Vector3 firstPos)
	{
		object3d_ = std::make_unique<Object3d>();
		modelManager_ = ModelManager::GetInstance();
		
		object3d_->Initialize();
		modelManager_->LoadModel(bulletModel, bulletSkin, true);
		object3d_->SetModel(bulletModel);
		Model* model = modelManager_->FindModel(bulletModel);
		Model::ModelData* modelData = model->GetModelData();
		for (Model::VertexData& vertex : modelData->vertices)
		{
			vertex.normal.x = vertex.position.x;
			vertex.normal.y = vertex.position.y;
			vertex.normal.z = vertex.position.z;
		}
		model->Memcpy();
		object3d_->SetTranslate({ firstPos.x /*+ 0.01f*/, firstPos.y,firstPos.z });
		object3d_->SetScale({ 0.5f,0.5f,0.5f });
		enemyBulletCollision.radius = 0.5f;
		object3d_->SetIsAnimation(false);
	}

	void EnemyBullet::Update()
	{
		if (object3d_->GetTranslate().z < Camera::GetInstance()->GetTranslate().z)
		{
			isDead = true;
		}
		object3d_->SetTranslate(Add(object3d_->GetTranslate(), Multiply(0.1f, enemyBulletVector)));
		object3d_->Update(Camera::GetInstance());
		enemyBulletCollision.center = object3d_->GetTranslate();
	}

	void EnemyBullet::Draw()
	{
		object3d_->Draw(modelManager_->GetModelCommon());
	}

	void EnemyBullet::OnCollision()
	{
		isDead = true;
	}

	void EnemyBullet::SetTranslate(Vector3 translate)
	{
		object3d_->SetTranslate(translate);
	}

	void EnemyBullet::SetEnemyBulletVector(Vector3 positoin)
	{
		enemyBulletVector = Normalize(Subtract(positoin, object3d_->GetTranslate()));
	}