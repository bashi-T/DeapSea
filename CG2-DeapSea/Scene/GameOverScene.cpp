#include "GameOverScene.h"

//GameOverScene::~GameOverScene()
//{
//	delete sprite;
//}

void GameOverScene::Init()
{
	//object3d = new Object3d;
	//object3d->Initialize(Object3dCommon::GetInstance(), SRVManager::GetInstance());
	//object3d
	sprite = new Sprite;
	sprite->Initialize(SpriteCommon::GetInstance(), SRVManager::GetInstance(), "Resource/GameOverExample.png");
	sprite->SetPositoin({ float(WinAPP::clientWidth_ / 2) - (sprite->GetSize().x / 2),0.0f });
	player = new Player;
	player->Initialize();
	whale = new Whale;
	whale->Initialize(player);
	whale->ChangeModel(boneWhaleModel, boneWhaleSkin);
	whale->SetTranslate({ 0.0f,-174.0f,0.0f });
	Camera::GetInstance()->SetTranslate({ 0.0f,-170.0f,-20.0f });
}

void GameOverScene::Update()
{
	XINPUT_STATE joyState;
	sprite->Update();
	if (floatingTime < 120)//title上下動
	{
		whale->SetTranslate({ whale->GetTranslate().x,whale->GetTranslate().y - 0.002f,whale->GetTranslate().z });
		floatingTime++;
	}
	else if (floatingTime >= 120 && floatingTime < 240)
	{
		whale->SetTranslate({ whale->GetTranslate().x,whale->GetTranslate().y + 0.002f,whale->GetTranslate().z });
		floatingTime++;
	}
	else if (floatingTime <= 240)
	{
		floatingTime = 0;
	}
	whale->SetRotate({ whale->GetRotate().x, whale->GetRotate().y + 0.01f, whale->GetRotate().z });
	whale->Update();
	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
	}
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER || Input::GetInstance()->TriggerKey(DIK_SPACE))
	{
		sceneNo = TITLE;
	}
}

void GameOverScene::Draw()
{
	sprite->Draw();
	whale->Draw();
}

void GameOverScene::Finalize()
{
	delete sprite;
}
