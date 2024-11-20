#include "Sand.h"

Sand::~Sand()
{
}

void Sand::Initialize()
{
	object3d = std::make_unique<Object3d>();
	std::string groundModel = "floor/floor.obj";
	const std::string groundSkin = "Resource/sand.png";
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
	object3d->SetTranslate({ 0.0f,0.0f,0.0f });

}

void Sand::Update()
{
	object3d->Update(Camera::GetInstance());
}

void Sand::Draw()
{
	object3d->Draw(ModelManager::GetInstance()->GetModelCommon());
}
