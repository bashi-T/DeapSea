#include"WindowApp.h"
#include"Debug.h"
#include"Mesh.h"
#include"Input.h"
#include"SpriteCommon.h"
#include"Sprite.h"

const int32_t kWindowWidth = 1280;
const int32_t kWindowHeight = 720;
const int32_t kNumTriangle = 1;
Vector4* vertexData = nullptr;
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int)
{
	Debug::D3DResourceLeakChecker* leakCheck = new Debug::D3DResourceLeakChecker;
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	WinAPP* winAPP = WinAPP::GetInstance();
	DX12Common* dx12Common = DX12Common::GetInstance();
	MSG NewMSG = winAPP->GetMSG();
	MyImGui* imgui = new MyImGui;
	Mesh* mesh[kNumTriangle];
	SpriteCommon* SPCommon = new SpriteCommon;
	Sprite* sprite = new Sprite;

	for (int i = 0; i < kNumTriangle; i++)
	{
		mesh[i] = new Mesh;
	}
	bool useWorldMap = true;

	winAPP->Initialize(kWindowWidth, kWindowHeight, L"GE3");

	dx12Common->Init(kWindowWidth, kWindowHeight, winAPP);
	imgui->Initialize(
	    winAPP->GetHWND(),
		dx12Common->GetDevice().Get(),
	    dx12Common->GetSwapChainDesc(),
	    dx12Common->GetRtvDesc(),
	    dx12Common->GetSrvDescriptorHeap().Get());

	//for (int i = 0; i < kNumTriangle; i++)
	//{
	//}
	SPCommon->Initialize(kWindowWidth, kWindowHeight, dx12Common);
	sprite->Initialize(kWindowWidth, kWindowHeight, SPCommon);

	while (NewMSG.message != WM_QUIT)
	{
		dx12Common->update();
		imgui->Update();

		for (int i = 0; i < kNumTriangle; i++)
		{
		}
		sprite->Update();
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

		sprite->Draw();

		imgui->Endframe(dx12Common->GetCommandList().Get());

		dx12Common->PostDraw();

	}

	CloseHandle(dx12Common->GetFenceEvent());
	dx12Common->DX12Release();

	delete imgui;
	dx12Common->DeleteInstance();
	for (int i = 0; i < kNumTriangle; i++)
	{
	}
	delete sprite;
	winAPP->Finalize();

	delete leakCheck;
	return 0;
}