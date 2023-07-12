#include"WindowApp.h"
#include"Debug.h"
#include"DX12Common.h"
const int32_t kWindowWidth = 1280;
const int32_t kWindowHeight = 720;
const int32_t kNumTriangle = 10;
Vector4* vertexData = nullptr;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//OutputDebugStringA("HelloDirectX!\n");
	Mesh* mesh[kNumTriangle] = {};
	DX12Common* dx12Common = new DX12Common;
	Debug* debug = new Debug;

	MSG NewMSG = WinAPP::GetMSG();

	WinAPP::Initialize(kWindowWidth, kWindowHeight);
	WinAPP::CreateWindowView(L"CG2");

	for (int i = 0; i < kNumTriangle; i++) {
	mesh[i]->Initialize(kWindowWidth, kWindowHeight, kNumTriangle);
	}

#ifdef _DEBUG
	debug->DebugLayer();
#endif
	dx12Common->Init();

	while (NewMSG.message != WM_QUIT)
	{
		if (PeekMessage(&NewMSG, NULL, 0, 0, PM_REMOVE))
		{
			WinAPP::ProcessMessage(NewMSG);
		}
		else
		{
			dx12Common->DrawScreen(kNumTriangle);
			for (int i = 0; i < kNumTriangle; i++) {
				dx12Common->DrawTriangle(kNumTriangle, i);
			}
			dx12Common->ClearScreen();
		}
	}
	dx12Common->DX12Release(debug->GetDebugController());
	debug->ReportLiveObject();
	return 0;
}