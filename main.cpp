#include"WindowApp.h"
#include"Debug.h"
#include"Mesh.h"

const int32_t kWindowWidth = 1280;
const int32_t kWindowHeight = 720;
const int32_t kNumTriangle = 10;
Vector4* vertexData = nullptr;
int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//OutputDebugStringA("HelloDirectX!\n");
	Mesh* mesh[kNumTriangle];
	for (int i = 0; i < kNumTriangle; i++)
	{
		mesh[i] = new Mesh;
	}

	DX12Common* dx12Common = DX12Common::GetInstance();
	Debug* debug = new Debug;
	WinAPP* winAPP = WinAPP::GetInstance();
	MSG NewMSG = winAPP->GetMSG();
	MyImGui* imgui = new MyImGui;

	winAPP->Initialize(kWindowWidth, kWindowHeight);
	winAPP->CreateWindowView(L"CG2");

#ifdef _DEBUG
	debug->DebugLayer();
#endif
	//ImGui::Begin("color");
	dx12Common->Init();
	imgui->Initialize(
	    winAPP->GetHWND(),
		dx12Common->GetInstance()->GetDevice(),
	    dx12Common->GetInstance()->GetSwapChainDesc(),
	    dx12Common->GetInstance()->GetRtvDesc(),
	    dx12Common->GetInstance()->GetSrvDescriptorHeap());

	Vector4 Top[kNumTriangle];
	Vector4 Left[kNumTriangle];
	Vector4 Right[kNumTriangle];
	Vector4 Color[kNumTriangle];

	Top[0] = {-0.75f, -0.6f, 0.0f, 1.0f};
	Left[0] = {-1.0f, -1.0f, 0.0f, 1.0f};
	Right[0] = {-0.5f, -1.0f, 0.0f, 1.0f};
	Color[0] = {1.0f, 0.0f, 0.0f, 1.0f};

	Top[1] = {-0.25f, -0.7f, 0.0f, 1.0f};
	Left[1] = {-0.5f, -1.0f, 0.0f, 1.0f};
	Right[1] = {0.0f, -1.0f, 0.0f, 1.0f};
	Color[1] = {1.0f, 0.0f, 0.0f, 1.0f};

	Top[2] = {0.25f, -0.8f, 0.0f, 1.0f};
	Left[2] = {0.0f, -1.0f, 0.0f, 1.0f};
	Right[2] = {0.5f, -1.0f, 0.0f, 1.0f};
	Color[2] = {1.0f, 1.0f, 1.0f, 1.0f};

	Top[3] = {0.75f, -0.9f, 0.0f, 1.0f};
	Left[3] = {0.5f, -1.0f, 0.0f, 1.0f};
	Right[3] = {1.0f, -1.0f, 0.0f, 1.0f};
	Color[3] = {1.0f, 1.0f, 1.0f, 1.0f};

	Top[4] = {-0.45f, 0.0f, 0.0f, 1.0f};
	Left[4] = {-0.6f, -0.5f, 0.0f, 1.0f};
	Right[4] = {-0.3f, -0.5f, 0.0f, 1.0f};
	Color[4] = {1.0f, 0.0f, 0.0f, 1.0f};

	Top[5] = {0.25f, 0.0f, 0.0f, 1.0f};
	Left[5] = {0.05f, -0.5f, 0.0f, 1.0f};
	Right[5] = {0.55f, -0.5f, 0.0f, 1.0f};
	Color[5] = {1.0f, 1.0f, 1.0f, 1.0f};

	Top[6] = {-0.45f, 0.5f, 0.0f, 1.0f};
	Left[6] = {-0.9f, 0.0f, 0.0f, 1.0f};
	Right[6] = {0.0f, 0.0f, 0.0f, 1.0f};
	Color[6] = {1.0f, 0.0f, 0.0f, 1.0f};

	Top[7] = {0.25f, 0.5f, 0.0f, 1.0f};
	Left[7] = {-0.0f, 0.0f, 0.0f, 1.0f};
	Right[7] = {0.5f, 0.0f, 0.0f, 1.0f};
	Color[7] = {1.0f, 1.0f, 1.0f, 1.0f};

	Top[8] = {-0.25f, 1.0f, 0.0f, 1.0f};
	Left[8] = {-0.25f, 0.5f, 0.0f, 1.0f};
	Right[8] = {0.0f, 0.5f, 0.0f, 1.0f};
	Color[8] = {1.0f, 0.0f, 0.0f, 1.0f};

	Top[9] = {0.25f, 1.0f, 0.0f, 1.0f};
	Left[9] = {0.0f, 0.5f, 0.0f, 1.0f};
	Right[9] = {0.4f, 0.5f, 0.0f, 1.0f};
	Color[9] = {1.0f, 1.0f, 1.0f, 1.0f};

	 

	for (int i = 0; i < kNumTriangle; i++)
	{
		mesh[i]->Initialize(kWindowWidth, kWindowHeight);
	}

	while (NewMSG.message != WM_QUIT) {
		
		imgui->Update();

		for (int i = 0; i < kNumTriangle; i++) {
			mesh[i]->Update();
		}
		ImGui::Begin("TriangleColor");
		ImGui::ColorEdit3("Triangle1", (float*)&Color[0]);
		ImGui::ColorEdit3("Triangle2", (float*)&Color[1]);
		ImGui::ColorEdit3("Triangle3", (float*)&Color[2]);
		ImGui::ColorEdit3("Triangle4", (float*)&Color[3]);
		ImGui::ColorEdit3("Triangle5", (float*)&Color[4]);
		ImGui::ColorEdit3("Triangle6", (float*)&Color[5]);
		ImGui::ColorEdit3("Triangle7", (float*)&Color[6]);
		ImGui::ColorEdit3("Triangle8", (float*)&Color[7]);
		ImGui::ColorEdit3("Triangle9", (float*)&Color[8]);
		ImGui::ColorEdit3("Triangle10", (float*)&Color[9]);
		ImGui::End();

		if (PeekMessage(&NewMSG, NULL, 0, 0, PM_REMOVE)) {
			winAPP->ProcessMessage(NewMSG);
			ImGui::Render();
		} else{
			dx12Common->DrawScreen();

			for (int i = 0; i < kNumTriangle; i++) {
				mesh[i]->DrawTriangle();
			}
			ID3D12DescriptorHeap* descriptorHeaps[] =
			{
			    dx12Common->GetInstance()->GetSrvDescriptorHeap()
			};
			dx12Common->GetInstance()->GetCommandList()->
				SetDescriptorHeaps(1, descriptorHeaps);

			for (int i = 0; i < kNumTriangle; i++) {
				mesh[i]->Draw(Top[i], Right[i], Left[i], Color[i]);
			}
			imgui->Endframe(dx12Common->GetInstance()->GetCommandList());
			dx12Common->ClearScreen();
		}
	}

	CloseHandle(dx12Common->GetFenceEvent());
	for (int i = 0; i < kNumTriangle; i++)
	{
		mesh[i]->MeshRelease();
	}
	dx12Common->DX12Release(debug->GetDebugController());
	debug->ReportLiveObject();
	return 0;
}