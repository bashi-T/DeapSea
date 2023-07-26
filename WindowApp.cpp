#include "WindowApp.h"

int32_t WinAPP::clientWidth_ = 0;
int32_t WinAPP::clientHeight_ = 0;

void WinAPP::Initialize(int32_t width, int32_t height)
{
	clientWidth_ = width;
	clientHeight_ = height;
}

LRESULT CALLBACK WinAPP::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WinAPP::CreateWindowView(const wchar_t* title)
{
	wc_.lpfnWndProc = WindowProc;
	wc_.lpszClassName = L"CG2WindowClass";
	wc_.hInstance = GetModuleHandle(nullptr);
	wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);
	RegisterClass(&wc_);

	RECT wtc = {
        0,
        0,
        clientWidth_,
        clientHeight_
	};

	AdjustWindowRect(&wtc, WS_OVERLAPPEDWINDOW, false);

	hwnd_ = CreateWindow(
		wc_.lpszClassName,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wtc.right - wtc.left,
		wtc.bottom - wtc.top,
		nullptr,
		nullptr,
		wc_.hInstance,
		nullptr
	);
	ShowWindow(hwnd_, SW_SHOW);

}
void WinAPP::ProcessMessage(MSG NewMSG)
{
	TranslateMessage(&NewMSG);
	DispatchMessage(&NewMSG);

}
WinAPP* WinAPP::GetInstance() {
	static WinAPP instance;
	return &instance;
}
HWND WinAPP::hwnd_;

