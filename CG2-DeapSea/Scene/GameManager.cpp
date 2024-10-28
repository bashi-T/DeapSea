#include "GameManager.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

Vector4* vertexData = nullptr;
int GameManager::stageNumber = 0;

GameManager::GameManager()
{
	sceneArr_[TITLE] = std::make_unique<TitleScene>();
	sceneArr_[INGAME] = std::make_unique<GameScene>();
	sceneArr_[GAMEOVER] = std::make_unique<GameOverScene>();
	sceneArr_[CLEAR] = std::make_unique<ClearScene>();
}

GameManager::~GameManager() {}

int GameManager::Run()
{
	CoInitializeEx(0, COINIT_MULTITHREADED);
	Debug::D3DResourceLeakChecker* leakCheck = new Debug::D3DResourceLeakChecker;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	winAPP = WinAPP::GetInstance();
	dx12Common = DX12Common::GetInstance();
	srvManager = SRVManager::GetInstance();
	audioManager = AudioManager::GetInstance();
	input = Input::GetInstance();
	MSG NewMSG = winAPP->GetMSG();
	imgui = MyImGui::GetInstance();
	SPCommon = SpriteCommon::GetInstance();
	object3dCommon = Object3dCommon::GetInstance();
	particleCommon = ParticleCommon::GetInstance();
	object3d = new Object3d;
	camera = new Camera();
	skyDome = new SkyDome;
	std::vector<Model*> models;



	winAPP->Initialize(WinAPP::clientWidth_, WinAPP::clientHeight_, L"GE3");
	dx12Common->Initialize(WinAPP::clientWidth_, WinAPP::clientHeight_, winAPP);
	srvManager->Initialize(dx12Common);
	audioManager->Initialize();
	input->Initialize(winAPP);
	imgui->Initialize(
		winAPP->GetHWND(),
		dx12Common->GetDevice().Get(),
		dx12Common->GetSwapChainDesc(),
		dx12Common->GetRtvDesc(),
		srvManager->GetSrvDescriptorHeap().Get());
	TextureManager::GetInstance()->Initialize(dx12Common, srvManager);

	object3dCommon->Initialize(dx12Common);
	ModelManager::GetInstance()->Initialize(dx12Common);
	camera->GetInstance()->SetRotate({0.1f,0.0f,0.0f});
	camera->GetInstance()->SetTranslate({ 0.0f,3.0f,-20.0f });
	object3dCommon->SetDefaultCamera(camera->GetInstance());
	object3d->Initialize(object3dCommon, srvManager);
	SPCommon->Initialize(dx12Common);
	particleCommon->Initialize(dx12Common);
	skyDome->Initialize();
	sceneArr_[TITLE]->Init();

	while (NewMSG.message != WM_QUIT)
	{
		dx12Common->update();
		Input::GetInstance()->Update();

#ifdef DEBUG
		if (Input::GetInstance()->PushKey(DIK_RIGHT))
		{
			camera->GetInstance()->SetTranslate({ camera->GetInstance()->GetTranslate().x + 0.2f, camera->GetInstance()->GetTranslate().y, camera->GetInstance()->GetTranslate().z });
		}
		if (Input::GetInstance()->PushKey(DIK_LEFT))
		{
			camera->GetInstance()->SetTranslate({ camera->GetInstance()->GetTranslate().x - 0.2f, camera->GetInstance()->GetTranslate().y, camera->GetInstance()->GetTranslate().z});
		}
#endif // DEBUG

		camera->GetInstance()->Update();
		prevSceneNo_ = currentSceneNo_;
		currentSceneNo_ = sceneArr_[currentSceneNo_]->GetSceneNo();
		if (prevSceneNo_ != currentSceneNo_)
		{
			sceneArr_[prevSceneNo_]->Finalize();
			audioManager->Initialize();
			sceneArr_[currentSceneNo_]->Init();
		}
		imgui->Update();
		skyDome->Update();
		sceneArr_[currentSceneNo_]->Update();
#ifdef _DEBUG
		ImGui::Begin("camera");
		//ImGui::DragFloat3("object.rotate", (float*)&object3d->GetRotate(), 0.01f);
		//ImGui::DragFloat3("object.translate", (float*)&object3d->GetTranslate(), 0.01f);
		ImGui::DragFloat3("camera.rotate", (float*)&camera->GetInstance()->GetRotate(), 0.01f);
		ImGui::DragFloat3("camera.translate", (float*)&camera->GetInstance()->GetTranslate(), 0.01f);
		ImGui::DragFloat4("light.color", (float*)&object3d->GetDirectionalLightData()->color, 0.01f);
		ImGui::DragFloat("light.intensity", (float*)&object3d->GetDirectionalLightData()->intensity, 0.01f);
		ImGui::DragFloat3("light.direction", (float*)&object3d->GetDirectionalLight().direction, 0.01f, -1.0f, 1.0f);
		object3d->GetDirectionalLightData()->direction = Normalize(object3d->GetDirectionalLightData()->direction);
		ImGui::End();
		ImGui::Begin("scene");
		switch (int(sceneArr_[currentSceneNo_]->GetSceneNo()))
		{
		case 0:
			ImGui::Text("TITLE");
			break;
		case 1:
			ImGui::Text("GAMESCENE");
			break;
		case 2:
			ImGui::Text("GAMEOVER");
			break;
		case 3:
			ImGui::Text("CLEAR");
			break;
		}
		ImGui::End();

#endif
		if (winAPP->ProcessMessage())
		{
			ImGui::Render();
			break;
		}
		srvManager->PreDraw();
		skyDome->Draw();
		sceneArr_[currentSceneNo_]->Draw();

		imgui->Endframe(dx12Common->GetCommandList().Get());

		srvManager->PostDraw();
	}

	CloseHandle(srvManager->GetFenceEvent());
	sceneArr_[currentSceneNo_]->Finalize();
	delete skyDome;
	skyDome = NULL;
	particleCommon->DeleteInstance();
	for (Model* model : models)
	{
		delete model;
		model = NULL;
	}
	SPCommon->DeleteInstance();
	camera->DeleteInstance();
	delete object3d;
	ModelManager::GetInstance()->Finalize();
	object3dCommon->DeleteInstance();
	TextureManager::GetInstance()->Finalize();
	imgui->Finalize();
	delete input;
	input = NULL;
	audioManager->Finalize();
	srvManager->Finalize();
	dx12Common->DeleteInstance();
	winAPP->Finalize();
	CoUninitialize();
	delete leakCheck;
	return 0;
}
