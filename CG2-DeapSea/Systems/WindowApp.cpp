#include "WindowApp.h"

namespace MyEngine
{
	//クライアント領域のサイズ
	int32_t WinAPP::clientWidth_ = 1280;
	int32_t WinAPP::clientHeight_ = 720;

	void WinAPP::Initialize(int32_t width, int32_t height, const wchar_t* title)
	{
		[[maybe_unused]] HRESULT hr = CoInitializeEx(0, COINIT_MULTITHREADED);
		assert(SUCCEEDED(hr));

		wc_.lpfnWndProc = WindowProc;//ウィンドウプロシージャ
		wc_.lpszClassName = L"深海デリバリー";//ウィンドウクラス名
		wc_.hInstance = GetModuleHandle(nullptr);//インスタンスハンドル
		wc_.hCursor = LoadCursor(nullptr, IDC_ARROW);//カーソル
		//ウィンドウクラス登録
		RegisterClass(&wc_);

		clientWidth_ = width;
		clientHeight_ = height;
		//ウィンドウサイズを表す構造体にクライアント領域を入れる
		RECT wtc = {
			0,
			0,
			clientWidth_,
			clientHeight_
		};
		//クライアント領域をもとに実際のサイズにwrcを変更してもらう
		AdjustWindowRect(&wtc, WS_OVERLAPPEDWINDOW, false);

		hwnd_ = CreateWindow(
			wc_.lpszClassName,//利用するクラス名
			title,//タイトルバーの文字
			WS_OVERLAPPEDWINDOW,//ウィンドウスタイル
			CW_USEDEFAULT,//表示X座標
			CW_USEDEFAULT,//表示Y座標
			wtc.right - wtc.left,//ウィンドウ横幅
			wtc.bottom - wtc.top,//ウィンドウ縦幅
			nullptr,//親ウィンドウハンドル
			nullptr,//メニューハンドル
			wc_.hInstance,//インスタンスハンドル
			nullptr//オプション
		);
		//ウィンドウ表示
		ShowWindow(hwnd_, SW_SHOW);
	}
	//ウィンドウプロシージャ
	LRESULT CALLBACK WinAPP::WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
	{
		if (ImGui_ImplWin32_WndProcHandler(hwnd, msg, wparam, lparam))
		{
			return true;
		}
		switch (msg)
		{//ウィンドウ破棄
		case WM_DESTROY:
			//OSにたいしてアプリの終了を伝える
			PostQuitMessage(0);
			return 0;
		}
		//標準のメッセージ処理
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
