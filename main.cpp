#include"WindowApp.h"
#include"Debug.h"
#include"DX12Common.h"
const int32_t kWindowWidth = 1280;
const int32_t kWindowHeight = 720;
const wchar_t* title = L"CG2";

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	//OutputDebugStringA("HelloDirectX!\n");
	WinAPP::Initilize(kWindowWidth, kWindowHeight);
	DX12Common* dx12Common_ = new DX12Common;
	Debug* debug_ = new Debug;
	MSG NewMSG = WinAPP::GetMSG();

	WinAPP::CreateWindowView(L"CG2");
#ifdef _DEBUG
	debug_->DebugLayer();
#endif
	dx12Common_->Init();

	while(title != 0)
	{
		if (PeekMessage(&NewMSG, NULL, 0, 0, PM_REMOVE))
		{
			WinAPP::ProcessMessage(NewMSG);
		}
		else
		{

        }
	}
	dx12Common_->DX12Release(debug_->GetDebugController());
	debug_->ReportLiveObject();
	return 0;
}