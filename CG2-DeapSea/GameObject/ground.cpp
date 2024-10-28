#include "ground.h"

Ground::~Ground()
{
	delete object3d;
	object3d=NULL;
}

void Ground::Initialize()
{
	object3d = new Object3d;
	std::string groundModel = "ground/newground.obj";
	const std::string groundSkin = "Resource/ganban.png";
	object3d->Initialize(Object3dCommon::GetInstance(), SRVManager::GetInstance());
	ModelManager::GetInstance()->LoadModel(groundModel, groundSkin, true);
	object3d->SetModel(groundModel);
	Model* model = ModelManager::GetInstance()->FindModel(groundModel);
	Model::ModelData* modelData = model->GetModelData();
	for (Model::VertexData& vertex : modelData->vertices)
	{
		vertex.normal.x = vertex.position.x;
		vertex.normal.y = vertex.position.y;
		vertex.normal.z = vertex.position.z;
	}
	model->Memcpy();
	object3d->SetTranslate({ 0.0f,-25.0f,100.0f });
}

void Ground::Update()
{
	object3d->Update(Camera::GetInstance());
#ifdef _DEBUG
	ImGui::Begin("ground");
	ImGui::DragFloat3("ground.translate", (float*)&object3d->GetTranslate(), 0.01f);
	ImGui::End();
#endif
}

void Ground::Draw()
{
	object3d->Draw(ModelManager::GetInstance()->GetModelCommon());
}
