#include "Cursor.h"

Cursor::~Cursor()
	{
	}

	void Cursor::Initialize()
	{
		object3d_ = std::make_unique<Object3d>();
		modelManager_ = ModelManager::GetInstance();
		object3d_->Initialize();
		modelManager_->LoadModel(cursorModel, cursorSkin, true);
		object3d_->SetModel(cursorModel);
		Model* model = modelManager_->FindModel(cursorModel);
		Model::ModelData* modelData = model->GetModelData();
		for (Model::VertexData& vertex : modelData->vertices)
		{
			vertex.normal.x = vertex.position.x;
			vertex.normal.y = vertex.position.y;
			vertex.normal.z = vertex.position.z;
		}
		model->Memcpy();
	}

	void Cursor::Update()
	{
		object3d_->SetRotate({ object3d_->GetRotate().x + 0.02f,object3d_->GetRotate().y,object3d_->GetRotate().z });
		object3d_->Update(Camera::GetInstance().get());
#ifdef _DEBUG
		ImGui::Begin("cursor");
		ImGui::DragFloat3("cursor.translate", (float*)&object3d_->GetTranslate(), 0.01f);
		ImGui::End();
#endif

	}

	void Cursor::Draw()
	{
		object3d_->Draw(modelManager_->GetModelCommon());
	}