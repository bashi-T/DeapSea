#include "GameOverScene.h"

namespace MyEngine
{
	void GameOverScene::Initialize()
	{
	}

	std::unique_ptr<BaseScene> GameOverScene::Update()
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

	void GameOverScene::Draw()
	{
	}
}
