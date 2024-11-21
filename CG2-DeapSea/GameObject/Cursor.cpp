#include "Cursor.h"

Cursor::~Cursor()
	{
	}

	void Cursor::Initialize()
	{
		object3d = std::make_unique<Object3d>();
		object3d->Initialize(Object3dCommon::GetInstance(), SRVManager::GetInstance());
		ModelManager::GetInstance()->LoadModel(cursorModel, cursorSkin, true);
		object3d->SetModel(cursorModel);
		Model* model = ModelManager::GetInstance()->FindModel(cursorModel);
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
		object3d->SetRotate({ object3d->GetRotate().x + 0.02f,object3d->GetRotate().y,object3d->GetRotate().z });
		object3d->Update(Camera::GetInstance());
#ifdef _DEBUG
		ImGui::Begin("cursor");
		ImGui::DragFloat3("cursor.translate", (float*)&object3d->GetTranslate(), 0.01f);
		ImGui::End();
#endif

	}

	void Cursor::Draw()
	{
		object3d->Draw(ModelManager::GetInstance()->GetModelCommon());
	}