#include "UIPlane.h"

UIPlane::~UIPlane()
	{
	}

	void UIPlane::Initialize(std::string plane, std::string skin)
	{
		object3d_ = std::make_unique<Object3d>();
		modelManager_ = ModelManager::GetInstance();

		planeModel = plane;
		planeSkin = skin;
		object3d_->Initialize();
		modelManager_->LoadModel(planeModel, planeSkin, false);
		object3d_->SetModel(planeModel);
		model = modelManager_->FindModel(planeModel);
		Model::ModelData* modelData = model->GetModelData();
		for (Model::VertexData& vertex : modelData->vertices)
		{
			vertex.normal.x = vertex.position.x;
			vertex.normal.y = vertex.position.y;
			vertex.normal.z = vertex.position.z;
		}
		object3d_->SetRotate({ 0.0f,3.15f,0.0f });
		model->Memcpy();
	}

	void UIPlane::Update()
	{
		object3d_->Update(Camera::GetInstance());
	}

	void UIPlane::Draw()
	{
		object3d_->Draw(modelManager_->GetModelCommon());
	}

	void UIPlane::SetColor(Vector4 color)
	{
		object3d_->SetColor(color);
	}
