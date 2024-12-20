#include "WindowApp.h"

namespace MyEngine
{
	int32_t WinAPP::clientWidth_ = 1280;
	int32_t WinAPP::clientHeight_ = 720;

	void WinAPP::Initialize(int32_t width, int32_t height, const wchar_t* title)
	{
		[[maybe_unused]] HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
		assert(SUCCEEDED(hr));

		wc_.lpfnWndProc = WindowProc;
		wc_.lpszClassName = L"深海デリバリー";
		wc_.hInstance = GetModuleHandle(nullptr);
		wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);
		RegisterClass(&wc_);

		clientWidth_ = width;
		clientHeight_ = height;

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

	LRESULT CALLBACK WinAPP::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		{
			return true;
		}
		switch (msg)
		{
		case WM_DESTROY:
			PostQuitMessage(0);
			return 0;
		}
		return DefWindowProc(hwnd, msg, wparam, lparam);
	}

	bool WinAPP::ProcessMessage()
	{
		MSG msg{};
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

		if (msg.message == WM_QUIT)
		{
			return true;
		}
		return false;
	}

	void WinAPP::Finalize()
	{
		CloseWindow(hwnd_);
		CoUninitialize();
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
}
