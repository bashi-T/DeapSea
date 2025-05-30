#pragma once
#define DIRECTINPUT_VERSION 0x0800	//DirectInputのバージョン指定
#include<dinput.h>
#include<Xinput.h>
#include<WRL.h>
#include"Systems/WindowApp.h"
#include<thread>

#pragma comment(lib,"dinput8.lib")
#pragma comment(lib,"dxguid.lib")
#pragma comment(lib,"XInput.lib")

namespace MyEngine
{
	class Input
	{
	public:
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		void Initialize(WinAPP* winApp);
		void Update();
		void Finalize();
		bool PushKey(BYTE keyNumber);
		bool TriggerKey(BYTE keyNumber);
		// ジョイスティックのデッドゾーンを適用する関数
		SHORT ApplyDeadzone(SHORT value, SHORT deadzone);
		bool GetJoystickState(int32_t stickNo, XINPUT_STATE& state);

		static Input* GetInstance();

		Input() = default;
		~Input() = default;
	private:
		Input(Input&) = delete;
		Input& operator=(Input&) = delete;

		WinAPP* winApp_ = nullptr;
		HRESULT hr = NULL;
		ComPtr<IDirectInput8>directInput = nullptr;
		ComPtr<IDirectInputDevice8> keyboard;
		BYTE key[256] = {};
		BYTE keyPre[256] = {};
		static inline std::unique_ptr<Input> instance;
		// デッドゾーンの設定
		const int DEADZONE_THRESHOLD = 8000;

	};
}

