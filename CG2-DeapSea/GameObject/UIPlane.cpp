#include "UIPlane.h"

UIPlane::~UIPlane()
{
	delete object3d;
	object3d = NULL;
	ModelManager::GetInstance()->EraseModel(planeModel, planeSkin);
}

void UIPlane::Initialize(std::string plane,std::string skin)
{
	object3d = new Object3d;
	 planeModel = plane;
	 planeSkin = skin;
	object3d->Initialize(Object3dCommon::GetInstance(), SRVManager::GetInstance());
	ModelManager::GetInstance()->LoadModel(planeModel, planeSkin, false);
	object3d->SetModel(planeModel);
	model = ModelManager::GetInstance()->FindModel(planeModel);
	Model::ModelData* modelData = model->GetModelData();
	for (Model::VertexData& vertex : modelData->vertices)
	{
		vertex.normal.x = vertex.position.x;
		vertex.normal.y = vertex.position.y;
		vertex.normal.z = vertex.position.z;
	}
	object3d->SetRotate({ 0.0f,3.15f,0.0f });
	model->Memcpy();
}

void UIPlane::Update()
{
	object3d->Update(Camera::GetInstance());
}

void UIPlane::Draw()
{
	object3d->Draw(ModelManager::GetInstance()->GetModelCommon());
}

void UIPlane::SetColor(Vector4 color)
{
	object3d->SetColor(color);
}