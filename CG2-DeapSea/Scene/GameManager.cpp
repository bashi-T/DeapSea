#include "GameManager.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

namespace MyEngine
{
	Vector4* vertexData = nullptr;
	int32_t GameManager::stageNumber = 0;

	GameManager::GameManager()
	{
		sceneArr_[TITLE] = std::make_unique<TitleScene>();
		sceneArr_[INGAME] = std::make_unique<GameScene>();
		sceneArr_[GAMEOVER] = std::make_unique<GameOverScene>();
		sceneArr_[CLEAR] = std::make_unique<ClearScene>();
	}

	GameManager::~GameManager() {}

	int32_t GameManager::Run()
	{
		hr = CoInitializeEx(0, COINIT_MULTITHREADED);
		std::unique_ptr<Debug::D3DResourceLeakChecker> leakCheck = std::make_unique<Debug::D3DResourceLeakChecker>();
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

		winAPP = WinAPP::GetInstance();
		MSG NewMSG = winAPP->GetMSG();
		input = Input::GetInstance();
		imgui = MyImGui::GetInstance();

		dx12Common = DX12Common::GetInstance();
		srvManager = SRVManager::GetInstance();
		audioManager = AudioManager::GetInstance();
		textureManager = TextureManager::GetInstance();
		modelManager = ModelManager::GetInstance();
		spriteCommon = SpriteCommon::GetInstance();
		object3dCommon = Object3dCommon::GetInstance();
		particleCommon = ParticleCommon::GetInstance();
		meshCommon = MeshCommon::GetInstance();
		camera = Camera::GetInstance();

		object3d = std::make_unique<Object3d>();
		skyDome = std::make_unique<SkyDome>();


		//ここから初期化
		winAPP->Initialize(WinAPP::clientWidth_, WinAPP::clientHeight_, L"深海デリバリー");
		input->Initialize(winAPP);
		dx12Common->Initialize(WinAPP::clientWidth_, WinAPP::clientHeight_, winAPP);
		srvManager->Initialize();
		audioManager->Initialize();
		imgui->Initialize(
			winAPP->GetHWND(),
			dx12Common->GetDevice().Get(),
			dx12Common->GetSwapChainDesc(),
			dx12Common->GetRtvDesc(),
			srvManager->GetSrvDescriptorHeap().Get());
		textureManager->Initialize();
		camera->Initialize();
		camera->SetRotate({ 0.1f,0.0f,0.0f });
		camera->SetTranslate({ 0.0f,3.0f,-20.0f });
		object3dCommon->Initialize();
		modelManager->Initialize();
		object3dCommon->SetDefaultCamera(camera);
		object3d->Initialize();
		spriteCommon->Initialize();
		particleCommon->Initialize();
		meshCommon->Initialize();
		skyDome->Initialize(camera);
		sceneArr_[TITLE]->Initialize();

		//ウィンドウのボタンが押されるまでループ
		while (NewMSG.message != WM_QUIT)
		{
			dx12Common->update();
			Input::GetInstance()->Update();

#ifdef _DEBUG
			if (Input::GetInstance()->PushKey(DIK_D))
			{
				camera->SetRotate({ camera->GetRotate().x , camera->GetRotate().y + 0.01f, camera->GetRotate().z });
			}
			if (Input::GetInstance()->PushKey(DIK_A))
			{
				camera->SetRotate({ camera->GetRotate().x, camera->GetRotate().y - 0.01f, camera->GetRotate().z });
			}
			if (Input::GetInstance()->PushKey(DIK_W))
			{
				camera->SetRotate({ camera->GetRotate().x + 0.01f, camera->GetRotate().y, camera->GetRotate().z });
			}
			if (Input::GetInstance()->PushKey(DIK_S))
			{
				camera->SetRotate({ camera->GetRotate().x - 0.01f, camera->GetRotate().y , camera->GetRotate().z });
			}
#endif // DEBUG

			prevSceneNo_ = currentSceneNo_;
			currentSceneNo_ = sceneArr_[currentSceneNo_]->GetSceneNo();
			if (prevSceneNo_ != currentSceneNo_)
			{
				sceneArr_[prevSceneNo_]->Finalize();
				audioManager->Initialize();
				sceneArr_[currentSceneNo_]->Initialize();
			}
			imgui->Update();
			skyDome->Update();
			sceneArr_[currentSceneNo_]->Update();
			camera->Update();

#ifdef _DEBUG
			ImGui::Begin("camera");
			//ImGui::DragFloat3("object.rotate", (float*)&object3d->GetRotate(), 0.01f);
			//ImGui::DragFloat3("object.translate", (float*)&object3d->GetTranslate(), 0.01f);
			ImGui::DragFloat3("camera.rotate", (float*)&camera->GetRotate(), 0.01f);
			ImGui::DragFloat3("camera.translate", (float*)&camera->GetTranslate(), 0.01f);
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
		//particleCommon->DeleteInstance();
		//spriteCommon->DeleteInstance();
		//camera->DeleteInstance();
		//modelManager->Finalize();
		//object3dCommon->DeleteInstance();
		//textureManager->Finalize();
		//audioManager->Finalize();
		//srvManager->Finalize();
		//dx12Common->DeleteInstance();

		imgui->Finalize();
		input->Finalize();
		winAPP->Finalize();
		CoUninitialize();
		return 0;
    }
}
