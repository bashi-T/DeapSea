#pragma once
#include<windows.h>
#include<cstdint>

class WindowApp {
public:
	static const int32_t kClientWidth = 1280;
	static const int32_t kColientHeight = 720;
	static HWND hwnd;

	static LRESULT CALLBACK WindowProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	const wchar_t* title = L"CG2";
	static void CreateWindowView(const wchar_t* title = L"CG2");
private:
	static inline WNDCLASS wc{};

};

typedef struct Vector4 {
	float x;
	float y;
	float z;
	float w;
} Vector4;

