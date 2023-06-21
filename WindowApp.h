#pragma once
#include<windows.h>
#include<cstdint>

class WinAPP {
public:
	static void Initilize(int32_t width, int32_t height);

	static LRESULT CALLBACK WindowProc(HWND hwnd,
		UINT msg, WPARAM wparam, LPARAM lparam);
	const wchar_t* title = L"CG2";
	static void CreateWindowView(const wchar_t* title = L"CG2");
	static int32_t GetClientWidth() { return clientWidth_; }
	static int32_t GetClientHeight() { return clientHeight_; }
	static HWND GetHWND() { return hwnd_; }
	static inline MSG GetMSG() { return msg; }
	static void ProcessMessage(MSG NewMSG);
private:
	static int32_t clientWidth_;
	static int32_t clientHeight_;
	static inline WNDCLASS wc_{};
	static HWND hwnd_;
	static inline MSG msg{};

};



