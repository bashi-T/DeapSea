#include "PlayerBullet.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

PlayerBullet::~PlayerBullet()
{
	delete object3d;
}

void PlayerBullet::Initialize(Vector3 firstPos,Vector3 angle)
{
	object3d = new Object3d;
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
	object3d->SetTranslate({ firstPos.x + 0.01f, firstPos.y,firstPos.z });

	Vector3 direction = { firstPos.x + 0.01f, firstPos.y,firstPos.z + 1.0f };
	bulletSpeed = Normalize(
		Vector3{
			(direction.x - firstPos.x) * std::cos(angle.y) + (direction.z - firstPos.z) * std::sin(angle.y) + firstPos.x,
			direction.y + firstPos.y,
			-(direction.x - firstPos.x) * std::sin(angle.y) + (direction.z - firstPos.z) * std::cos(angle.y) + firstPos.z,
		});

	object3d->SetScale({ 0.5f,0.5f,0.5f });
	object3d->SetIsAnimation(false);
}

void PlayerBullet::Update()
{
	if (--deathTimer <= 0)
	{
		isDead = true;
	}
	object3d->SetTranslate(Add(object3d->GetTranslate(), bulletSpeed));
	object3d->Update(Camera::GetInstance());
}

void PlayerBullet::Draw()
{
	object3d->Draw(ModelManager::GetInstance()->GetModelCommon());
}

void PlayerBullet::OnCollision()
{
	isDead = true;
}

void PlayerBullet::SetTranslate(Vector3 translate)
{
	object3d->SetTranslate(translate);
}
