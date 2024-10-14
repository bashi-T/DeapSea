#include "titleScene.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

void TitleScene::Init()
{
	std::string PNGs[5] =
	{
		"Resource/sinkaiTitle4.png",
		"Resource/practice.png",
		"Resource/Stage1.png",
		"Resource/Stage2.png",
		"Resource/Stage3.png",
	};
	for(uint32_t i = 0; i < 1; i++)
	{
		Particle* particle = new Particle;
		particle->SetElements(1.0f, 1.0f, 1.0f, 6.0f,
			-7.0f, 7.0f, -6.0f, -6.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 1.0f, 6.0f, 0.0f, 0.0f);
		particle->Initialize("Resource/clearbabble.png", ParticleCommon::GetInstance(), SRVManager::GetInstance(), Object3dCommon::GetInstance(), particle->GetElements());
		particles.push_back(particle);
	}
	for (uint32_t i = 0; i < 5; i++)
	{
		UIPlane* uiPlane = new UIPlane;
		uiPlane->Initialize("plane/plane.obj", PNGs[i]);
		uiPlanes.push_back(uiPlane);
	}
	uiPlanes[0]->SetTranslate({ 0.0f,1.6f,-16.0f });
	uiPlanes[1]->SetTranslate({ 0.0f,1.5f,0.0f });
	uiPlanes[2]->SetTranslate({ 0.0f,0.5f,0.0f });
	uiPlanes[3]->SetTranslate({ 0.0f,-0.5f,0.0f });
	uiPlanes[4]->SetTranslate({ 0.0f,-1.5f,0.0f });

	uiPlanes[1]->SetScale({ 1.5f,0.8f,1.0f});
	uiPlanes[2]->SetScale({ 1.5f,0.8f,1.0f});
	uiPlanes[3]->SetScale({ 1.5f,0.8f,1.0f});
	uiPlanes[4]->SetScale({ 1.5f,0.8f,1.0f});

	Object3dCommon::GetInstance()->SetDefaultCamera(Camera::GetInstance());
	cursor = new Cursor;
	cursor->Initialize();
	cursor->SetTranslate({ -2.5f,1.5f,0.0f });
	Camera::GetInstance()->SetTranslate({0.0f,2.0f,-20.0f});

	player = new Player;
	player->Initialize();
	whale = new Whale;
	whale->Initialize(player);
	whale->SetTranslate({ 0.0f,-13.15f,5.0f });
	floatTime = 0;

	isSceneTransition = false;
	isStageSelect = false;
	numStage = 1;
	nowStage = 1;
	cooltime = 0;
	sceneTransitionTime = 0;

	bgm = AudioManager::GetInstance()->SoundLoadWave("Resource/Sounds/titlebgm.wav");
	//AudioManager::GetInstance()->SoundPlayWave(AudioManager::GetInstance()->GetxAudio2().Get(), bgm);
	enterSound = AudioManager::GetInstance()->SoundLoadWave("Resource/Sounds/kettei.wav");
	moveSound = AudioManager::GetInstance()->SoundLoadWave("Resource/Sounds/kettei2.wav");
}

void TitleScene::Update()
{
	XINPUT_STATE joyState;
	for (Particle* particle : particles)
	{
		particle->Update(true, particle->GetElements());
	}
	for (UIPlane* uiPlane : uiPlanes)
	{
		uiPlane->Update();
	}
	cursor->Update();

	if(floatTime<120)
	{
		uiPlanes[0]->SetTranslate({ uiPlanes[0]->GetTranslate().x,uiPlanes[0]->GetTranslate().y - 0.001f,uiPlanes[0]->GetTranslate().z});
		floatTime++;
	}
	else if (floatTime >= 120 && floatTime < 240)
	{
		uiPlanes[0]->SetTranslate({ uiPlanes[0]->GetTranslate().x,uiPlanes[0]->GetTranslate().y+0.001f,uiPlanes[0]->GetTranslate().z });
		floatTime++;
	}
	else if (floatTime <= 240)
	{
		floatTime = 0;
	}
	//whale->Update();
	Input::GetInstance()->GetJoystickState(0, joyState);
	if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && isStageSelect == false || Input::GetInstance()->TriggerKey(DIK_SPACE) && isStageSelect == false)
	{
		AudioManager::GetInstance()->SoundPlayWave(AudioManager::GetInstance()->GetxAudio2().Get(), enterSound);
		isStageSelect = true;
	}
	if (isStageSelect == true && isSceneTransition == false)
	{
		if (cooltime <= 12)
		{
			cooltime++;
		}
		else
		{
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN || Input::GetInstance()->TriggerKey(DIK_DOWN))
			{
				AudioManager::GetInstance()->SoundPlayWave(AudioManager::GetInstance()->GetxAudio2().Get(), moveSound);
				nowStage++;
				if (nowStage > 4)
				{
					nowStage = 1;
				}
				cursor->SetTranslate({ uiPlanes[nowStage]->GetTranslate().x - 2.5f,uiPlanes[nowStage]->GetTranslate().y,uiPlanes[nowStage]->GetTranslate().z });
				cooltime = 0;
			}
			else if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP || Input::GetInstance()->TriggerKey(DIK_UP))
			{
				AudioManager::GetInstance()->SoundPlayWave(AudioManager::GetInstance()->GetxAudio2().Get(), moveSound);
				nowStage--;
				if (nowStage < 1)
				{
					nowStage = 4;
				}
				cursor->SetTranslate({ uiPlanes[nowStage]->GetTranslate().x - 2.5f,uiPlanes[nowStage]->GetTranslate().y,uiPlanes[nowStage]->GetTranslate().z });
				cooltime = 0;
			}
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && isSceneTransition == false || Input::GetInstance()->TriggerKey(DIK_SPACE) && isSceneTransition == false)
			{
				AudioManager::GetInstance()->SoundPlayWave(AudioManager::GetInstance()->GetxAudio2().Get(), enterSound);
				isSceneTransition = true;
				cooltime = 0;
			}
		}
	}
	if (isSceneTransition == true)
	{
		sceneTransitionTime++;
		if (sceneTransitionTime >= 20)
		{
			Camera::GetInstance()->SetTranslate({ Camera::GetInstance()->GetTranslate().x, Camera::GetInstance()->GetTranslate().y - 3.0f, Camera::GetInstance()->GetTranslate().z });
			uiPlanes[nowStage]->SetTranslate({ uiPlanes[nowStage]->GetTranslate().x,uiPlanes[nowStage]->GetTranslate().y - 2.75f,uiPlanes[nowStage]->GetTranslate().z });
		}
		if (sceneTransitionTime == 90)
		{
			sceneNo = INGAME;
			GameManager::stageNumber = nowStage - 1;
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
		uiPlanes[0]->Draw();
	}
	else if(isSceneTransition==false)
	{
		cursor->Draw();
		uiPlanes[1]->Draw();
		uiPlanes[2]->Draw();
		uiPlanes[3]->Draw();
		uiPlanes[4]->Draw();
	}
	else if (isSceneTransition == true)
	{
		uiPlanes[nowStage]->Draw();
	}

	for (Particle* particle : particles)
	{
		particle->Draw();
	}

	whale->Draw();
}

void TitleScene::Finalize()
{
	for (Particle* particle : particles)
	{
		delete particle;
	  particle = NULL;
	}
	particles.clear();
	for (UIPlane* uiPlane : uiPlanes)
	{
		delete uiPlane;
		uiPlane = NULL;
	}
	uiPlanes.clear();
	delete cursor;
	cursor = NULL;
	delete whale;
	whale = NULL;
	AudioManager::GetInstance()->SoundUnload(&bgm);
	AudioManager::GetInstance()->SoundUnload(&enterSound);
	AudioManager::GetInstance()->SoundUnload(&moveSound);
};
