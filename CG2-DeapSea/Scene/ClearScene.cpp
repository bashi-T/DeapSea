#include "ClearScene.h"

//ClearScene::~ClearScene()
//{
//	delete sprite;
//}

void ClearScene::Init()
{
	sprite = new Sprite;
	sprite->Initialize(SpriteCommon::GetInstance(), SRVManager::GetInstance(), "Resource/clearExample.png");
	sprite->SetPositoin({ float(WinAPP::clientWidth_ / 2) - (sprite->GetSize().x / 2),0.0f });
	Camera::GetInstance()->SetTranslate({ 0.0f,7.0f,-20.0f });
	i = 0;
}

void ClearScene::Update()
{
	XINPUT_STATE joyState;
	sprite->Update();
	i++;
	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
	}
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER || Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		sceneNo = TITLE;
	}
	if (i == 5)
	{
		sceneNo = INGAME;
	}
}

void ClearScene::Draw()
{
	sprite->Draw();
}

void ClearScene::Finalize()
{
	delete sprite;
}
