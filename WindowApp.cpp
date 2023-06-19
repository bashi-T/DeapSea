#include "WindowApp.h"

LRESULT CALLBACK WindowApp::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}

void WindowApp::CreateWindowView(const wchar_t* title)
{
	wc.lpfnWndProc = WindowProc;
	wc.lpszClassName = L"CG2WindowClass";
	wc.hInstance = GetModuleHandle(nullptr);
	wc.hCursor = LoadCursor(nullptr, IDC_ARROW);
	RegisterClass(&wc);

	RECT wtc = {
        0,
        0,
        kClientWidth,
        kColientHeight
	};

	AdjustWindowRect(&wtc, WS_OVERLAPPEDWINDOW, false);

	hwnd = CreateWindow(
		wc.lpszClassName,
		title,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		wtc.right - wtc.left,
		wtc.bottom - wtc.top,
		nullptr,
		nullptr,
		wc.hInstance,
		nullptr
	);

	ShowWindow(hwnd, SW_SHOW);

}
HWND WindowApp::hwnd;

