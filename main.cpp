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
	dx12Common->Init();

	for (int i = 0; i < kNumTriangle; i++)
	{
		mesh[i]->Initialize(kWindowWidth, kWindowHeight);
	}

	while (NewMSG.message != WM_QUIT)
	{
		imgui->Update();

		for (int i = 0; i < kNumTriangle; i++) {
			mesh[i]->Update(kNumTriangle);
		}
		ImGui::ShowDemoWindow();
		if (PeekMessage(&NewMSG, NULL, 0, 0, PM_REMOVE))
		{
			winAPP->ProcessMessage(NewMSG);
		}
		else
		{
			dx12Common->DrawScreen();
			ImGui::Render();
			for (int i = 0; i < kNumTriangle; i++) {
				mesh[i]->DrawTriangle(kNumTriangle);
				ID3D12DescriptorHeap* descriptorHeaps[] = {
				    dx12Common->GetInstance()->GetSrvDescriptorHeap()};
				dx12Common->GetInstance()->GetCommandList()->SetDescriptorHeaps(1, descriptorHeaps);

				mesh[i]->Draw(kNumTriangle);
			}

			ImGui_ImplDX12_RenderDrawData(
			    ImGui::GetDrawData(),
				dx12Common->GetInstance()->GetCommandList());

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