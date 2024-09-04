#include "titleScene.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

void TitleScene::Init()
{
	std::string PNGs[6] =
	{
		"Resource/Titleexample.png",
		"Resource/practice.png",
		"Resource/Stage1.png",
		"Resource/Stage2.png",
		"Resource/Stage3.png",
		"Resource/arrow.png",
	};
	//for(uint32_t i = 0; i < 1; i++)
	//{
		//Particle* particle = new Particle;
		//particle->Initialize("Resource/colorbabble.png", ParticleCommon::GetInstance(), SRVManager::GetInstance(), Object3dCommon::GetInstance());
		//particles.push_back(particle);
	//}
	for (uint32_t i = 0; i < 6; i++)
	{
		Sprite* sprite = new Sprite;
		sprite->Initialize(SpriteCommon::GetInstance(), SRVManager::GetInstance(), PNGs[i]);
		sprites.push_back(sprite);
	}
	sprites[0]->SetPositoin({ float(WinAPP::clientWidth_ / 2) - (sprites[0]->GetSize().x / 2),0.0f });
	sprites[1]->SetPositoin({ float(WinAPP::clientWidth_ / 2) - (sprites[1]->GetSize().x / 2),100.0f });
	sprites[2]->SetPositoin({ float(WinAPP::clientWidth_ / 2) - (sprites[2]->GetSize().x / 2),sprites[1]->GetSize().y / 3 + sprites[1]->GetPosition().y });
	sprites[3]->SetPositoin({ float(WinAPP::clientWidth_ / 2) - (sprites[3]->GetSize().x / 2),sprites[2]->GetSize().y / 3 + sprites[2]->GetPosition().y });
	sprites[4]->SetPositoin({ float(WinAPP::clientWidth_ / 2) - (sprites[4]->GetSize().x / 2),sprites[3]->GetSize().y / 3 + sprites[3]->GetPosition().y });
	Object3dCommon::GetInstance()->SetDefaultCamera(Camera::GetInstance());
	cursor = new Cursor;
	cursor->Initialize();
	cursor->SetTranslate({ -2.7f,1.7f,0.0f });

	//whale = new Whale;
	//whale->Initialize();
	//whale->SetTranslate({ 0.0f,-13.15f,5.0f });
	//floatTime = 0;

	isSceneTransition = false;
	isStageSelect = false;
	numStage = 1;
	nowStage = 1;
	cooltime = 0;
}

void TitleScene::Update()
{
	XINPUT_STATE joyState;
	//for (Particle* particle : particles)
	//{
	//	particle->Update(true);
	//}
	for (Sprite* sprite : sprites)
	{
		sprite->Update();
	}
	cursor->Update();

	//if(floatTime<120)
	//{
	//	whale->SetTranslate({ whale->GetTranslate().x,whale->GetTranslate().y-0.005f,whale->GetTranslate().z });
	//	floatTime++;
	//}
	//else if (floatTime >= 120 && floatTime < 240)
	//{
	//	whale->SetTranslate({ whale->GetTranslate().x,whale->GetTranslate().y+0.005f,whale->GetTranslate().z });
	//	floatTime++;
	//}
	//else if (floatTime <= 240)
	//{
	//	floatTime = 0;
	//}
	//whale->Update();

	if (Input::GetInstance()->GetJoystickState(0, joyState))
	{
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && isStageSelect == false)
		{
			isStageSelect = true;
		}
	}
	if (isStageSelect == true && isSceneTransition == false)
	{
		if (cooltime <= 12)
		{
			cooltime++;
		}
		else
		{
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN)
			{
				/*sprites[5]->SetPositoin({ sprites[nowStage]->GetPosition().x - sprites[5]->GetSize().x,sprites[nowStage]->GetPosition().y });*/
				cursor->SetTranslate({ cursor->GetTranslate().x,cursor->GetTranslate().y - 1.1f,cursor->GetTranslate().z });
				cooltime = 0;
				nowStage++;
				if (nowStage > 4)
				{
					cursor->SetTranslate({ -2.7f,1.7f,0.0f });
					nowStage = 1;
				}
			}
			else if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP)
			{
				cursor->SetTranslate({ cursor->GetTranslate().x,cursor->GetTranslate().y + 1.1f,cursor->GetTranslate().z });
				cooltime = 0;
				nowStage--;
				if (nowStage < 1)
				{
					cursor->SetTranslate({ -2.7f,-1.55f,0.0f });
					nowStage = 4;
				}
			}
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && isSceneTransition == false)
			{
				isSceneTransition = true;
				cooltime = 0;
			}
		}
	}
	if (isSceneTransition == true)
	{
		sceneTransitionTime++;
		if(sceneTransitionTime>=20)
		{
			Camera::GetInstance()->SetTranslate({ Camera::GetInstance()->GetTranslate().x, Camera::GetInstance()->GetTranslate().y - 3.0f, Camera::GetInstance()->GetTranslate().z });
			sprites[nowStage]->SetPositoin({ sprites[nowStage]->GetPosition().x,sprites[nowStage]->GetPosition().y - 10.0f });
		}
		if(sceneTransitionTime==90)
		{
			sceneNo = INGAME;
			GameManager::stageNumber = 0;
		}
	}
#ifdef _DEBUG
	ImGui::Begin("num");
	ImGui::InputInt("num", &nowStage);
	ImGui::End();
#endif
}

void TitleScene::Draw()
{
	if (isStageSelect == false)
	{
		sprites[0]->Draw();
	}
	else if(isSceneTransition==false)
	{
		cursor->Draw();
		sprites[1]->Draw();
		sprites[2]->Draw();
		sprites[3]->Draw();
		sprites[4]->Draw();
		//sprites[5]->Draw();
	}
	else if (isSceneTransition == true)
	{
		sprites[nowStage]->Draw();
	}
	//whale->Draw();
}

void TitleScene::Finalize()
{
	//for (Particle* particle : particles)
	//{
	//	delete particle;
	//  particle = NULL;
	//}
	//particles.clear();
	for (Sprite* sprite : sprites)
	{
		sprite->Finalize();
		delete sprite;
		sprite = NULL;
	}
	sprites.clear();
	delete cursor;
	cursor = NULL;
	delete whale;
	whale = NULL;

};
