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
		object3d = std::make_unique<Object3d>();
		object3d->Initialize(Object3dCommon::GetInstance(), SRVManager::GetInstance());
		ModelManager::GetInstance()->LoadModel(bulletModel, bulletSkin, true);
		object3d->SetModel(bulletModel);
		Model* model = ModelManager::GetInstance()->FindModel(bulletModel);
		Model::ModelData* modelData = model->GetModelData();
		for (Model::VertexData& vertex : modelData->vertices)
		{
			vertex.normal.x = vertex.position.x;
			vertex.normal.y = vertex.position.y;
			vertex.normal.z = vertex.position.z;
		}
		model->Memcpy();
		object3d->SetTranslate({ firstPos.x /*+ 0.01f*/, firstPos.y,firstPos.z });
		object3d->SetScale({ 0.5f,0.5f,0.5f });
		ebCollision.radius = 0.5f;
		object3d->SetIsAnimation(false);
	}

	void EnemyBullet::Update()
	{
		if (object3d->GetTranslate().z < Camera::GetInstance()->GetTranslate().z)
		{
			isDead = true;
		}
		object3d->SetTranslate(Add(object3d->GetTranslate(), Multiply(0.1f, enemyBulletVector)));
		object3d->Update(Camera::GetInstance());
		ebCollision.center = object3d->GetTranslate();
	}

	void EnemyBullet::Draw()
	{
		object3d->Draw(ModelManager::GetInstance()->GetModelCommon());
	}

	void EnemyBullet::OnCollision()
	{
		isDead = true;
	}

	void EnemyBullet::SetTranslate(Vector3 translate)
	{
		object3d->SetTranslate(translate);
	}

	void EnemyBullet::SetEnemyBulletVector(Vector3 positoin)
	{
		enemyBulletVector = Normalize(Subtract(positoin, object3d->GetTranslate()));
	}