#include "titleScene.h"

void TitleScene::Init()
{
	Particle* particle = new Particle;
	particle->Initialize("Resource/colorbabble.png", SRVManager::GetInstance(), Object3dCommon::GetInstance());
	particles.push_back(particle);
	sprite = new Sprite;
	sprite->Initialize(SpriteCommon::GetInstance(), SRVManager::GetInstance(), "Resource/Titleexample.png");
	sprite->SetPositoin({ float(WinAPP::clientWidth_ / 2) - (sprite->GetSize().x / 2),0.0f });
	Object3dCommon::GetInstance()->SetDefaultCamera(Camera::GetInstance());
}

void TitleScene::Update()
{
	XINPUT_STATE joyState;
	for (Particle* particle : particles)
	{
		particle->Update();
	}
	sprite->Update();
	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
		{
			sceneNo = INGAME;
			GameManager::stageNumber = 0;
		}
	}

}

void TitleScene::Draw()
{
	for (Particle* particle : particles)
	{
		particle->Draw();
	}
	sprite->Draw();
}

void TitleScene::Finalize()
{
	for (Object3d* object3d : objects3d)
	{
		delete object3d;
	}
	delete sprite;
}
