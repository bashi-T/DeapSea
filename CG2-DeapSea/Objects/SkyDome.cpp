#include "SkyDome.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

namespace MyEngine
{
	SkyDome::~SkyDome()
	{
	}

	void SkyDome::Initialize()
	{
		object3d_ = std::make_unique<Object3d>();
		object3d_->Initialize();
		std::string model_ = "world/skyDome.obj";
		std::string skin = "Resource/sea2.png";
		modelManager_ = ModelManager::GetInstance();
		modelManager_->LoadModel(model_, skin, true);
		object3d_->SetModel(model_);
		Model* model = modelManager_->FindModel(model_);
		Model::ModelData* modelData = model->GetModelData();
		for (Model::VertexData& vertex : modelData->vertices)
		{
			vertex.normal.x = vertex.position.x;
			vertex.normal.y = vertex.position.y;
			vertex.normal.z = vertex.position.z;
		}
		model->Memcpy();
		object3d_->SetScale({ -400.0f,400.0f,400.0f });
	}

	void SkyDome::Update()
	{
		object3d_->Update(Camera::GetInstance().get());
	}

	void SkyDome::Draw()
	{
		object3d_->Draw(modelManager_->GetModelCommon());
	}

}
