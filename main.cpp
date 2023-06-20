#include"WindowApp.h"
#include"DX12Common.h"

const int32_t kWindowWidth = 1280;
const int32_t kWindowHeight = 720;

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	OutputDebugStringA("HelloDirectX!\n");
	//WindowApp* window_ = new WindowApp;
	DX12* dx12_ = new DX12;

	WinAPP::Initilize(kWindowWidth, kWindowHeight);

	WinAPP::CreateWindowView(L"CG2");

	dx12_->MakeDXGIFactory();
	dx12_->ChoseUseAdapter();
	dx12_->MakeD3D12Device();
	dx12_->MakeCommandQueue();
	dx12_->MakeCommandList();
	dx12_->MakeSwapchain(WinAPP::GetClientWidth(), WinAPP::GetClientHeight(),WinAPP::GetHWND());
	dx12_->MakeRTV();
	dx12_->MakeScreen();


	MSG msg{};
	while(msg.message!=WM_QUIT)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{

        }
	}

	return 0;
}