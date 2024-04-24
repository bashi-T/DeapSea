#include "titleScene.h"

void TitleScene::Init()
{
	//object3dCommon = new Object3dCommon; 
	//input = new Input;
	//srvManager = new SRVManager;
	//camera = new Camera();
	std::string textureFilePath[10] =//textureの左右が反転してる
	{
		"Resource/uvChecker.png",//一番最初のテクスチャがうまく読み込まれない
		"Resource/monsterBall.png",
		"Resource/AnimatedCube/AnimatedCube_BaseColor.png",
		"Resource/worldMap.png",
		"Resource/world.png",
		"Resource/ganban.png",
		"Resource/uvChecker.png",
		"Resource/uvChecker.png",
		"Resource/circle.png",
		"Resource/cursor.png"
	};
	TextureManager::GetInstance()->LoadTexture(textureFilePath[0]);
	std::string objFilePath[10] =
	{
		"world/world.obj",
		"AnimatedCube/AnimatedCube.gltf",
		"axis/axis.obj",
		"plane/plane.gltf",
	};
	for (uint32_t i = 0; i < 2; i++)
	{
		Object3d* object3d = new Object3d;
		Particle* particle = new Particle;
		if(i==0)
		{
			object3d->Initialize(Object3dCommon::GetInstance(), SRVManager::GetInstance());
			ModelManager::GetInstance()->LoadModel(objFilePath[i], textureFilePath[i + 1]);
			object3d->SetModel(objFilePath[i]);
		}else if(i==1)
		{
			object3d->Initialize(Object3dCommon::GetInstance(), SRVManager::GetInstance());
			ModelManager::GetInstance()->LoadAnimationModel(objFilePath[i], textureFilePath[i + 1]);
			object3d->SetModel(objFilePath[i]);
		}
		Model* model = ModelManager::GetInstance()->FindModel(objFilePath[i]);
		Model::ModelData* modelData = model->GetModelData();
		for (Model::VertexData& vertex : modelData->vertices)
		{
			vertex.normal.x = vertex.position.x;
			vertex.normal.y = vertex.position.y;
			vertex.normal.z = vertex.position.z;
		}
		model->Memcpy();
		object3d->SetTranslate({ 1.0f -(2.0f* i), 0.0f, 0.0f });
		object3d->SetScale({ i + 0.005f, i + 0.005f , i + 0.005f  });
		objects3d.push_back(object3d);
		particle->Initialize(textureFilePath[i+1], SRVManager::GetInstance(), Object3dCommon::GetInstance(), DX12Common::GetInstance());
		particles.push_back(particle);
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
	}
		objects3d[0]->Update(Camera::GetInstance());
		objects3d[1]->AnimationUpdate(Camera::GetInstance());
	for (Particle* particle : particles)
	{
		particle->Update();
	}
	if (Input::GetInstance()->TriggerKey(DIK_SPACE))
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
	for (Particle* particle : particles)
	{
		particle->Draw();
	}
}

void TitleScene::Finalize()
{
	for (Object3d* object3d : objects3d)
	{
		delete object3d;
	}
}
