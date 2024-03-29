#include "titleScene.h"

void TitleScene::Init()
{
	//object3dCommon = new Object3dCommon; 
	//input = new Input;
	//srvManager = new SRVManager;
	//camera = new Camera();
	std::string textureFilePath[10] =
	{
		"Resource/civ6.png",
		"Resource/uvChecker.png",
		"Resource/monsterBall.png",
		"Resource/worldMap.png",
		"Resource/world.png",
		"Resource/circle.png",
		"Resource/uvChecker.png",
		"Resource/uvChecker.png",
		"Resource/uvChecker.png",
		"Resource/cursor.png"
	};
	std::string objFilePath[10] =
	{
		"plane.obj",
		"axis.obj",
		"world.obj",
	};
	for (uint32_t i = 0; i < 1; i++)
	{
		Object3d* object3d = new Object3d;
		object3d->Initialize(Object3dCommon::GetInstance(), SRVManager::GetInstance());
		ModelManager::GetInstance()->LoadModel(objFilePath[2], textureFilePath[4]);
		object3d->SetModel(objFilePath[2]);
		Model* model = ModelManager::GetInstance()->FindModel(objFilePath[2]);
		Model::ModelData* modelData = model->GetModelData();
		for (Model::VertexData& vertex : modelData->vertices)
		{
			vertex.normal.x = vertex.position.x;
			vertex.normal.y = vertex.position.y;
			vertex.normal.z = vertex.position.z;
		}
		model->Memcpy();
		object3d->SetTranslate({ 0.2f * i, 0.2f * i, 0.2f * i });
		object3d->SetScale({ 0.02f, 0.02f, 0.02f });
		objects3d.push_back(object3d);
	};
	Object3dCommon::GetInstance()->SetDefaultCamera(Camera::GetInstance());
}

void TitleScene::Update()
{
	for (Object3d* object3d : objects3d)
	{
		if (Input::GetInstance()->PushKey(DIK_D))
		{
			object3d->SetTranslate({ object3d->GetTranslate().x + 0.01f ,object3d->GetTranslate().y ,object3d->GetTranslate().z });
		}
		if (Input::GetInstance()->PushKey(DIK_A))
		{
			object3d->SetTranslate({ object3d->GetTranslate().x - 0.01f ,object3d->GetTranslate().y ,object3d->GetTranslate().z });
		}
		object3d->Update(Camera::GetInstance());
	}
	if (Input::GetInstance()->PushKey(DIK_SPACE))
	{
		sceneNo = INGAME;
	}
}

void TitleScene::Draw()
{
	for (Object3d* object3d : objects3d)
	{
		object3d->Draw(Object3dCommon::GetInstance(), ModelManager::GetInstance()->GetModelCommon());
	}
}

void TitleScene::Finalize()
{
	for (Object3d* object3d : objects3d)
	{
		delete object3d;
	}
}
