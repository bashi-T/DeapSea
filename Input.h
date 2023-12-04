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
	void Initialize(WinAPP* winApp);
	void Update();
	bool PushKey(BYTE keyNumber);
	bool TriggerKey(BYTE keyNumber);

private:
	WinAPP* winApp_ = nullptr;
	HRESULT hr;
	ComPtr<IDirectInput8>directInput = nullptr;
	ComPtr<IDirectInputDevice8> keyboard;
	BYTE key[256] = {};
	BYTE keyPre[256] = {};

};

