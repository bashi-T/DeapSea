#include"WindowApp.h"
#include"Debug.h"
#include"Mesh.h"
#include"Input.h"
#include"SpriteCommon.h"
#include"Sprite.h"
#include"TextureManager.h"

const int32_t kWindowWidth = 1280;
const int32_t kWindowHeight = 720;
const int32_t kNumTriangle = 1;
Vector4* vertexData = nullptr;
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	CoInitializeEx(0, COINIT_MULTITHREADED);
	Debug::D3DResourceLeakChecker* leakCheck = new Debug::D3DResourceLeakChecker;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	WinAPP* winAPP = WinAPP::GetInstance();
	DX12Common* dx12Common = DX12Common::GetInstance();
	MSG NewMSG = winAPP->GetMSG();
	MyImGui* imgui = new MyImGui;
	SpriteCommon* SPCommon = new SpriteCommon;
	std::vector<Sprite*> sprites;
	Vector2 posSprite = { 0.0f,0.0f };
	//for (int i = 0; i < kNumTriangle; i++)
	//{
	//	mesh[i] = new Mesh;
	//}
	//bool useWorldMap = true;

		std::string textureFilePath[10] =
		{
			"Resource/civ6.png",
			"Resource/uvChecker.png",
			"Resource/monsterBall.png",
			"Resource/worldMap.png",
			"Resource/uvChecker.png",
			"Resource/uvChecker.png",
			"Resource/uvChecker.png",
			"Resource/uvChecker.png",
			"Resource/uvChecker.png",
			"Resource/cursor.png"
		};

	winAPP->Initialize(kWindowWidth, kWindowHeight, L"GE3");
	dx12Common->Initialize(kWindowWidth, kWindowHeight, winAPP);
	imgui->Initialize(
	    winAPP->GetHWND(),
		dx12Common->GetDevice().Get(),
	    dx12Common->GetSwapChainDesc(),
	    dx12Common->GetRtvDesc(),
	    dx12Common->GetSrvDescriptorHeap().Get());
	TextureManager::GetInstance()->Initialize();
	for (uint32_t i = 0; i < 10; i++)
	{
	}

	SPCommon->Initialize(dx12Common);
	for (uint32_t i = 0; i < 10; i++)
	{
		Sprite* sprite = new Sprite();
		TextureManager::GetInstance()->LoadTexture(dx12Common, textureFilePath[i]);
		sprite->Initialize(kWindowWidth, kWindowHeight, SPCommon, textureFilePath[i]);
		if (i == 0) {
			posSprite.x = -50.0f;
			posSprite.y = -50.0f;

		}else
		{
			posSprite.x = 100.0f * i;
			posSprite.y = 50.0f * i;
		}
		sprite->SetPositoin(posSprite);
		sprites.push_back(sprite);
	}

	while (NewMSG.message != WM_QUIT)
	{
		dx12Common->update();
		imgui->Update();
		int i = 0;
		for (Sprite* sprite : sprites)
		{
			sprite->Update(kWindowWidth, kWindowHeight);
		}

		//ImGui::Begin("sphereEdit");
		//ImGui::ColorEdit3("Sphere", (float*)&ColorSphere[0]);
		//ImGui::DragFloat3("sphere.center", (float*)&sphere.center, 0.01f);
		//ImGui::DragFloat("sphere.radius", (float*)&sphere.radius, 0.01f);
		//ImGui::Checkbox("useWorldMap", &useWorldMap);
		//ImGui::End();

		if (winAPP->ProcessMessage())
		{
			//winAPP->ProcessMessage(NewMSG);
			ImGui::Render();
			break;
		}
		dx12Common->PreDraw();

		//mesh[0]->DrawOBJ({ 1.0f,1.0f,1.0f,1.0f }, true, kWindowWidth, kWindowHeight);
		for (Sprite* sprite : sprites)
		{
			sprite->Draw(SPCommon);
		}
		imgui->Endframe(dx12Common->GetCommandList().Get());

		dx12Common->PostDraw();

	}

	CloseHandle(dx12Common->GetFenceEvent());
	//dx12Common->DX12Release();
	//delete imgui;
	for (Sprite* sprite : sprites)
	{
		delete sprite;
	}
	TextureManager::GetInstance()->Finalize();
	imgui->Finalize();
	dx12Common->DeleteInstance();
	winAPP->Finalize();
	CoUninitialize();
	delete leakCheck;
	return 0;
}