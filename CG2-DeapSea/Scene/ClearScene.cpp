#include "ClearScene.h"

namespace MyEngine
{
	void ClearScene::Initialize()
	{
	}

	std::unique_ptr<BaseScene> ClearScene::Update()
	{
		XINPUT_STATE joyState;
		if (Input::GetInstance()->GetJoystickState(0, joyState))
		{
		}
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER || Input::GetInstance()->TriggerKey(DIK_SPACE))
		{
		    return std::make_unique<TitleScene>();
		}
		else
		{
		    return nullptr;
		}
	}

	void ClearScene::Draw()
	{
	}
}
