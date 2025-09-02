#include "GameManager.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

namespace MyEngine
{
	Vector4* vertexData = nullptr;

	GameManager::GameManager()
	{
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
	    sceneManager_ = SceneManager::GetInstance();
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
	    camera->SetRotate(cameraFirstRotate);
	    camera->SetTranslate(cameraFirsttranslate);
		object3dCommon->Initialize();
		modelManager->Initialize();
		object3dCommon->SetDefaultCamera(camera);
		object3d->Initialize();
		spriteCommon->Initialize();
		particleCommon->Initialize();
		meshCommon->Initialize();
		skyDome->Initialize(camera);
		sceneManager_->Initialize();

		//ウィンドウのボタンが押されるまでループ
		while (NewMSG.message != WM_QUIT)
		{
			dx12Common->update();
			input->Update();

			imgui->Update();
			skyDome->Update();
			sceneManager_->Update();
			camera->Update();

#ifdef _DEBUG
			ImGui::Begin("camera");
		    ImGui::DragFloat3("camera.rotate", (float*)&camera->GetRotate(), kRotateSpeed);
		    ImGui::DragFloat3("camera.translate", (float*)&camera->GetTranslate(), kRotateSpeed);
		    ImGui::DragFloat4("light.color", (float*)&object3d->GetDirectionalLightData()->color, kRotateSpeed);
		    ImGui::DragFloat("light.intensity", (float*)&object3d->GetDirectionalLightData()->intensity, kRotateSpeed);
		    ImGui::DragFloat3("light.direction", (float*)&object3d->GetDirectionalLight().direction, kRotateSpeed, -1.0f, 1.0f);
			object3d->GetDirectionalLightData()->direction = Normalize(object3d->GetDirectionalLightData()->direction);
			ImGui::End();
#endif // DEBUG

			if (winAPP->ProcessMessage())
			{
				ImGui::Render();
				break;
			}
			srvManager->PreDraw();
			skyDome->Draw();
			sceneManager_->Draw();

			imgui->Endframe(dx12Common->GetCommandList().Get());

			srvManager->PostDraw();
	    }

		CloseHandle(srvManager->GetFenceEvent());

		imgui->Finalize();
		input->Finalize();
		winAPP->Finalize();
		CoUninitialize();
		return 0;
    }

} // namespace MyEngine
