#include"WindowApp.h"
#include"DX12.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	OutputDebugStringA("HelloDirectX!\n");
	WindowApp* window_ = new WindowApp;
	DX12* dx12_ = new DX12;
	window_->CreateWindowView();

	dx12_->MakeDXGIFactory();
	dx12_->ChoseUseAdapter();
	dx12_->MakeD3D12Device();


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