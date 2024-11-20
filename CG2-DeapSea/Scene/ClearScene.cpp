#include "ClearScene.h"

//ClearScene::~ClearScene()
//{
//	delete sprite;
//}

void ClearScene::Init()
{
	sprite = std::make_unique<Sprite>();
	sprite->Initialize(SpriteCommon::GetInstance(), SRVManager::GetInstance(), "Resource/clearExample.png");
	sprite->SetPositoin({ float(WinAPP::clientWidth_ / 2) - (sprite->GetSize().x / 2),0.0f });
	Camera::GetInstance()->SetTranslate({ 0.0f,-270.0f,-20.0f });
	Camera::GetInstance()->SetRotate({ -1.0f,0.0f,0.0f });

	player_ = std::make_unique<Player>();
	whale_ = std::make_unique<Whale>();
	sand_ = std::make_unique<Sand>();

	sand_->Initialize();
	sand_->SetTranslate({0.0f,-280.0f,0.0f});
}

void ClearScene::Update()
{
	XINPUT_STATE joyState;
	sprite->Update();
	sand_->Update();
	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
	}
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER || Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		sceneNo = TITLE;
	}
}

void ClearScene::Draw()
{
	sand_->Draw();
	sprite->Draw();
}

void ClearScene::Finalize()
{
	sand_.reset();
}
