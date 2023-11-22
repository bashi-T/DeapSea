#pragma once
#define DIRECTINPUT_VERSION 0x0800
#include<dinput.h>
#include<WRL.h>
#include<DirectXTex.h>
#include"WindowApp.h"

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")

class Input
{
public:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	void Initialize(HINSTANCE hInstance, HWND hwnd);
	void Update();
	bool PushKey(BYTE keyNumber);
	bool TriggerKey(BYTE keyNumber);

private:
	WinAPP* winApp = nullptr;
	HRESULT hr;
	HINSTANCE hInstance;
	ComPtr<IDirectInput8>directInput = nullptr;
	ComPtr<IDirectInputDevice8> keyboard;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};

};

