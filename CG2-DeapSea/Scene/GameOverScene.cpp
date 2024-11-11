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
	player_ = std::make_unique<Player>();
	whale_ = std::make_unique<Whale>();
	player_->Initialize();
	whale_->Initialize(player_.get());
	whale_->ChangeModel(boneWhaleModel, boneWhaleSkin);
	whale_->SetTranslate({ 0.0f,-174.0f,0.0f });
	Camera::GetInstance()->SetTranslate({ 0.0f,-170.0f,-20.0f });
}

void GameOverScene::Update()
{
	XINPUT_STATE joyState;
	sprite->Update();
	if (floatingTime < 120)//whale上下動
	{
		whale_->SetTranslate({ whale_->GetTranslate().x,whale_->GetTranslate().y - 0.002f,whale_->GetTranslate().z });
		floatingTime++;
	}
	else if (floatingTime >= 120 && floatingTime < 240)
	{
		whale_->SetTranslate({ whale_->GetTranslate().x,whale_->GetTranslate().y + 0.002f,whale_->GetTranslate().z });
		floatingTime++;
	}
	else if (floatingTime <= 240)
	{
		floatingTime = 0;
	}
	whale_->SetRotate({ whale_->GetRotate().x, whale_->GetRotate().y + 0.01f, whale_->GetRotate().z });
	whale_->Update();
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
	whale_->Draw();
}

void GameOverScene::Finalize()
{
	delete sprite;
	whale_.release();
	player_.release();
}
