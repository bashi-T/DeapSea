#include "Sand.h"

	Sand::~Sand()
	{
	}

	void Sand::Initialize()
	{
		object3d_ = std::make_unique<Object3d>();
		object3d_->Initialize();
		ModelManager::GetInstance()->LoadModel(groundModel, groundSkin, true);
		object3d_->SetModel(groundModel);
		Model* model = ModelManager::GetInstance()->FindModel(groundModel);
		Model::ModelData* modelData = model->GetModelData();
		for (Model::VertexData& vertex : modelData->vertices)
		{
			vertex.normal.x = vertex.position.x;
			vertex.normal.y = vertex.position.y;
			vertex.normal.z = vertex.position.z;
		}
		model->Memcpy();
		object3d_->SetTranslate({ 0.0f,0.0f,0.0f });

	}

	void Sand::Update()
	{
		object3d_->Update(Camera::GetInstance().get());
	}

	void Sand::Draw()
	{
		object3d_->Draw();
	}