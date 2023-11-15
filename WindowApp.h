#pragma once
#include<windows.h>
#include<cstdint>
#include"MyImGui.h"

class WinAPP
 {
public:
	static void Initialize(int32_t width, int32_t height);

	static LRESULT CALLBACK WindowProc(HWND hwnd,
		UINT msg, WPARAM wparam, LPARAM lparam);
	const wchar_t* title = L"CG2";
	static void CreateWindowView(const wchar_t* title = L"CG2");
	static int32_t GetClientWidth() { return clientWidth_; }
	static int32_t GetClientHeight() { return clientHeight_; }
	static HWND GetHWND() { return hwnd_; }
	static inline MSG GetMSG() { return msg; }
	static void ProcessMessage(MSG NewMSG);
	static inline MSG msg{};
	static WinAPP* GetInstance();

private:
	WinAPP() = default;
	~WinAPP() = default;
	WinAPP(const WinAPP& obj) = delete;
	WinAPP& oparator(const WinAPP& obj) = delete;

	static int32_t clientWidth_;
	static int32_t clientHeight_;
	static inline WNDCLASS wc_{};
	static HWND hwnd_;

};



