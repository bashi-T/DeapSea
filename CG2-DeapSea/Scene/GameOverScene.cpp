#include "GameOverScene.h"

namespace MyEngine
{
	void GameOverScene::Initialize()
	{
		camera_ = Camera::GetInstance();
		sprite_ = std::make_unique<Sprite>();
		sprite_->Initialize("Resource/GameOverExample.png");
		sprite_->SetPosition({ float(WinAPP::clientWidth_ / 2) - (sprite_->GetSize().x / 2),0.0f });

		player_ = std::make_unique<Player>();
	    whale_ = std::make_unique<Whale>();

		player_->Initialize(camera_);
	    player_->SetIsHit(true);
		whale_->Initialize(player_.get());
		whale_->ChangeModel(boneWhaleModel, boneWhaleSkin);
		whale_->SetTranslate({ 0.0f,-279.5f,0.0f });

		camera_->SetTranslate({ 0.0f,-277.0f,-20.0f });
		camera_->SetRotate({ 0.1f,0.0f,0.0f });

		sand_ = std::make_unique<Sand>();
		sand_->Initialize();
		sand_->SetTranslate({ 0.0f,-280.0f,0.0f });

	}

	std::unique_ptr<BaseScene> GameOverScene::Update()
	{
		XINPUT_STATE joyState;
		sprite_->Update();
		sand_->Update();
		whale_->Update();
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
		sprite_->Draw();
		whale_->Draw();
		sand_->Draw();
	}
}
