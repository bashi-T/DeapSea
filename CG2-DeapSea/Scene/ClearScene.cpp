#include "ClearScene.h"

namespace MyEngine
{
	void ClearScene::Initialize()
	{
		camera_ = Camera::GetInstance();
		sprite_ = std::make_unique<Sprite>();
		sprite_->Initialize("Resource/clearExample.png");
		sprite_->SetPosition({ float(WinAPP::clientWidth_ / 2) - (sprite_->GetSize().x / 2),0.0f });
		camera_->SetTranslate({ 0.0f,-270.0f,-20.0f });
		camera_->SetRotate({ -1.0f,0.0f,0.0f });

		player_ = std::make_unique<Player>();
		whale_ = std::make_unique<Whale>();
		sand_ = std::make_unique<Sand>();

		sand_->Initialize();
		sand_->SetTranslate({ 0.0f,-280.0f,0.0f });
	}

	std::unique_ptr<BaseScene> ClearScene::Update()
	{
		XINPUT_STATE joyState;
		sprite_->Update();
		sand_->Update();
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
		sand_->Draw();
		sprite_->Draw();
	}
}
