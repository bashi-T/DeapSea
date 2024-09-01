#include "titleScene.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

//TitleScene::~TitleScene()
//{
//	delete sprite;
//	particles.clear();
//}

void TitleScene::Init()
{
	//for(uint32_t i = 0; i < 1; i++)
	//{
		//Particle* particle = new Particle;
		//particle->Initialize("Resource/colorbabble.png", ParticleCommon::GetInstance(), SRVManager::GetInstance(), Object3dCommon::GetInstance());
		//particles.push_back(particle);
	//}

	sprite = new Sprite;
	sprite->Initialize(SpriteCommon::GetInstance(), SRVManager::GetInstance(), "Resource/Titleexample.png");
	sprite->SetPositoin({ float(WinAPP::clientWidth_ / 2) - (sprite->GetSize().x / 2),0.0f });
	Object3dCommon::GetInstance()->SetDefaultCamera(Camera::GetInstance());
}

void TitleScene::Update()
{
	XINPUT_STATE joyState;
	//for (Particle* particle : particles)
	//{
	//	particle->Update();
	//}
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
	//for (Particle* particle : particles)
	//{
	//	particle->Draw();
	//}
	sprite->Draw();
}

void TitleScene::Finalize()
{
	//for (Object3d* object3d : objects3d)
	//{
	//	delete object3d;
	//	object3d = NULL;
	//}
	//for (Particle* particle : particles)
	//{
	//	delete particle;
	//  particle = NULL;
	//}
	//particles.clear();
	sprite->Finalize();
	delete sprite;
	sprite = NULL;
}
