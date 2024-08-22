#include "SkyDome.h"

void SkyDome::Initialize()
{
	object3d = new Object3d;
	object3d->Initialize(Object3dCommon::GetInstance(), SRVManager::GetInstance());
	std::string model_ = "world/skyDome.obj";
	std::string skin = "Resource/civ6.png";
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
	object3d->SetScale({ -200.0f,200.0f,200.0f });
}

void SkyDome::Update()
{
	object3d->Update(Camera::GetInstance());
}

void SkyDome::Draw()
{
	object3d->Draw(ModelManager::GetInstance()->GetModelCommon());
}

void SkyDome::SphereInitialize()
{

	//Sphere sphere = { { 0.0f,0.0f,0.0f },1.0f };
	//transformMatrixSphere = {
	//{1.0f, 1.0f, 1.0f},
	//{0.0f, 0.0f, 0.0f},
	//{0.0f, 0.0f, 0.0f}
	//};

	//vertexResourceSphere = CreateBufferResource(sizeof(VertexData) * 6 * kSubdivision * kSubdivision);
	//materialResourceSphere = CreateBufferResource(sizeof(Material));
	//transformationMatrixResourceSphere = CreateBufferResource(sizeof(TransformationMatrix));
	//directionalLightResource = CreateBufferResource(sizeof(DirectionalLight));
	//directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&DirectionalLightData));

	//indexResourceSphere = CreateBufferResource(sizeof(uint32_t) * 6 * kSubdivision * kSubdivision);
}

void SkyDome::SphereUpdate()
{
}

void SkyDome::SphereDraw()
{
}

void SkyDome::ResetDXC()
{
}

void SkyDome::MakePSO()
{
}
