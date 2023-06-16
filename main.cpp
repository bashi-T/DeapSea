#include"WindowApp.h"

int WINAPI WinMain(HINSTANCE, HINSTANCE, LPSTR, int) {
	OutputDebugStringA("HelloDirectX!\n");
	WindowApp* window_ = new WindowApp;
	window_->CreateWindowView();


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