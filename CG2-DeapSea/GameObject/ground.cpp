#include "ground.h"

Ground::~Ground()
	{
	}

	void Ground::Initialize()
	{
		object3d_ = std::make_unique<Object3d>();
		object3d_->Initialize();
		modelManager_ = ModelManager::GetInstance();
		modelManager_->LoadModel(groundModel, groundSkin, true);
		object3d_->SetModel(groundModel);
		Model* model = modelManager_->FindModel(groundModel);
		Model::ModelData* modelData = model->GetModelData();
		for (Model::VertexData& vertex : modelData->vertices)
		{
			vertex.normal.x = vertex.position.x;
			vertex.normal.y = vertex.position.y;
			vertex.normal.z = vertex.position.z;
		}
		model->Memcpy();
		object3d_->SetTranslate({ 0.0f,-25.0f,100.0f });
	}

	void Ground::Update()
	{
		object3d_->Update(Camera::GetInstance().get());
#ifdef _DEBUG
		ImGui::Begin("ground");
		ImGui::DragFloat3("ground.translate", (float*)&object3d_->GetTranslate(), 0.01f);
		ImGui::End();
#endif
	}

	void Ground::Draw()
	{
		object3d_->Draw();
	}

