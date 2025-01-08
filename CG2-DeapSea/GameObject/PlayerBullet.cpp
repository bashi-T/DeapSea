#include "PlayerBullet.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

PlayerBullet::~PlayerBullet()
{
}

void PlayerBullet::Initialize(Vector3 firstPos, Matrix4x4 angle)
{
	object3d_ = std::make_unique<Object3d>();
	modelManager_ = ModelManager::GetInstance();

	object3d_->Initialize();
	modelManager_->LoadModel(bulletModel, bulletSkin, true);
	object3d_->SetModel(bulletModel);
	MyEngine::Model* model = modelManager_->FindModel(bulletModel);

	MyEngine::Model::ModelData* modelData = model->GetModelData();
	for (MyEngine::Model::VertexData& vertex : modelData->vertices)
	{
		vertex.normal.x = vertex.position.x;
		vertex.normal.y = vertex.position.y;
		vertex.normal.z = vertex.position.z;
	}
	model->Memcpy();
	object3d_->SetTranslate({ firstPos.x + 0.01f, firstPos.y,firstPos.z });
	playerBulletCollision.radius = 0.5f;
	Vector3 bulletSpeeds = TransformNormal(bulletSpeed, angle);


	object3d_->SetScale({ 0.5f,0.5f,0.5f });
	object3d_->SetIsAnimation(false);
}

void PlayerBullet::Update()
{
	if (--deathTimer <= 0)
	{
		isDead = true;
	}
	object3d_->SetTranslate(Add(object3d_->GetTranslate(), bulletSpeed));
	object3d_->Update(Camera::GetInstance());

	playerBulletCollision.center = object3d_->GetTranslate();

}

void PlayerBullet::Draw()
{
	object3d_->Draw(modelManager_->GetModelCommon());
}

void PlayerBullet::OnCollision()
{
	isDead = true;
}

void PlayerBullet::SetTranslate(Vector3 translate)
{
	object3d_->SetTranslate(translate);
}

