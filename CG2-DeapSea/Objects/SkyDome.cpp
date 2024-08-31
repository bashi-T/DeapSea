#include "SkyDome.h"

SkyDome::~SkyDome()
{
	delete object3d;
	object3d = NULL;
}

void SkyDome::Initialize()
{
	object3d = new Object3d;
	object3d->Initialize(Object3dCommon::GetInstance(), SRVManager::GetInstance());
	std::string model_ = "world/skyDome.obj";
	std::string skin = "Resource/sea2.png";
	ModelManager::GetInstance()->LoadModel(model_, skin);
	object3d->SetModel(model_);
	Model* model = ModelManager::GetInstance()->FindModel(model_);
	Model::ModelData* modelData = model->GetModelData();
	for (Model::VertexData& vertex : modelData->vertices)
	{
		vertex.normal.x = vertex.position.x;
		vertex.normal.y = vertex.position.y;
		vertex.normal.z = vertex.position.z;
	}
	model->Memcpy();
	object3d->SetScale({ -400.0f,400.0f,400.0f });
}

void SkyDome::Update()
{
	object3d->Update(Camera::GetInstance());
}

void SkyDome::Draw()
{
	object3d->Draw(ModelManager::GetInstance()->GetModelCommon());
}

//void SkyDome::SphereInitialize()
//{
//
//}
//
//void SkyDome::SphereUpdate()
//{
//}
//
//void SkyDome::SphereDraw()
//{
//}
//
//void SkyDome::ResetDXC()
//{
//}
//
//void SkyDome::MakePSO()
//{
//}
