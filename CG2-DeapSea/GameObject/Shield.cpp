#include "Shield.h"

Shield::~Shield()
{
}

void Shield::Initialize()
{
    object3d_ = std::make_unique<Object3d>();
    object3d_->Initialize();
    modelManager_ = ModelManager::GetInstance();
    modelManager_->LoadModel(shieldModel, shieldSkin, true);
    object3d_->SetModel(shieldModel);
    Model* model = modelManager_->FindModel(shieldModel);
    Model::ModelData* modelData = model->GetModelData();
    for (Model::VertexData& vertex : modelData->vertices)
	{
	    vertex.normal.x = vertex.position.x;
	    vertex.normal.y = vertex.position.y;
	    vertex.normal.z = vertex.position.z;
    }
    model->Memcpy();
    isDead = true;
    collision.center = {0.0f, 0.0f, 0.0f};
    collision.radius = 0.0f;
   }

void Shield::Update(Vector3 playerPos,Vector3 whalePos) 
{
    object3d_->SetTranslate({(playerPos.x + whalePos.x) / 2, (playerPos.y + whalePos.y) / 2, (playerPos.z + whalePos.z) / 2});
    if (isDead == false)
	{
	    if (collision.radius <= Length(Subtract(playerPos,object3d_->GetTranslate())) + 2.0f)
		{
		    collision.radius += 0.2f;
	    }
	}
	else if (collision.radius > 0.0f)
	{
	    collision.radius -= 0.2f;
	}
    object3d_->SetScale({collision.radius, collision.radius, collision.radius});
    collision.center = object3d_->GetTranslate();
    object3d_->Update(Camera::GetInstance());
}

void Shield::Draw()
{
	object3d_->Draw(modelManager_->GetModelCommon());
}