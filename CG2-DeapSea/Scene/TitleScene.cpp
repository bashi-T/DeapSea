#include "titleScene.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

namespace MyEngine
{
	void TitleScene::Init()
	{
		std::string PNGs[6] =
		{
			"Resource/sinkaiTitle4.png",
			"Resource/practice.png",
			"Resource/Stage1.png",
			"Resource/Stage2.png",
			"Resource/Stage3.png",
			"Resource/black.png",
		};

		Object3dCommon::GetInstance()->SetDefaultCamera(Camera::GetInstance());
		player_ = std::make_unique<Player>();
		whale_ = std::make_unique<Whale>();
		cursor_ = std::make_unique<Cursor>();

		player_->Initialize();
		player_->SetIsHit(true);
		whale_->Initialize(player_.get());
		whale_->SetTranslate({ 0.0f,0.15f,5.0f });
		floatingTime = 0;

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

		for (uint32_t i = 0; i < 6; i++)
		{
			std::unique_ptr<UIPlane> uiPlane = std::make_unique<UIPlane>();
			uiPlane->Initialize(Planes[i], PNGs[i]);
			uiPlanes.push_back(std::move(uiPlane));
		}
		uiPlanes[0]->SetTranslate({ 0.0f,1.6f,-16.0f });
		uiPlanes[1]->SetTranslate({ 0.0f,2.1f,0.0f });
		uiPlanes[2]->SetTranslate({ 0.0f,0.7f,0.0f });
		uiPlanes[3]->SetTranslate({ 0.0f,-0.7f,0.0f });
		uiPlanes[4]->SetTranslate({ 0.0f,-2.1f,0.0f });

		uiPlanes[1]->SetScale({ 1.5f,1.0f,0.0f });
		uiPlanes[2]->SetScale({ 1.5f,1.0f,0.0f });
		uiPlanes[3]->SetScale({ 1.5f,1.0f,0.0f });
		uiPlanes[4]->SetScale({ 1.5f,1.0f,0.0f });

		//uiPlanes[1]->SetColor({ 1.0f,0.0f,0.0f,0.1f });

		uiPlanes[5]->SetTranslate({ Camera::GetInstance()->GetTranslate().x, Camera::GetInstance()->GetTranslate().y, Camera::GetInstance()->GetTranslate().z + 2.0f });
		uiPlanes[5]->SetScale({ 10.0f,10.0f,0.0f });
		uiPlanes[5]->SetColor({ 0.0f,0.0f,0.0f,0.0f });

		cursor_->Initialize();
		cursor_->SetTranslate({ uiPlanes[1]->GetTranslate().x - 2.5f,uiPlanes[1]->GetTranslate().y,uiPlanes[1]->GetTranslate().z });
		Camera::GetInstance()->SetTranslate({ 0.0f,2.0f,-20.0f });
		Camera::GetInstance()->SetRotate({ 0.1f,0.0f,0.0f });

	}

	void TitleScene::Update()
	{
		XINPUT_STATE joyState;
		//for (Particle* particle : particles)
		//{
		//	particle->Update(true, particle->GetElements());
		//}
		for (int i = 0; i < uiPlanes.size(); i++)
		{
			uiPlanes[i]->Update();
		}
		cursor_->Update();

		if (floatingTime < 120)//title上下動
		{
			uiPlanes[0]->SetTranslate({ uiPlanes[0]->GetTranslate().x,uiPlanes[0]->GetTranslate().y - 0.001f,uiPlanes[0]->GetTranslate().z });
			whale_->SetTranslate({ whale_->GetTranslate().x,whale_->GetTranslate().y - 0.003f,whale_->GetTranslate().z });
			floatingTime++;
		}
		else if (floatingTime >= 120 && floatingTime < 240)
		{
			uiPlanes[0]->SetTranslate({ uiPlanes[0]->GetTranslate().x,uiPlanes[0]->GetTranslate().y + 0.001f,uiPlanes[0]->GetTranslate().z });
			whale_->SetTranslate({ whale_->GetTranslate().x,whale_->GetTranslate().y + 0.003f,whale_->GetTranslate().z });
			floatingTime++;
		}
		else if (floatingTime <= 240)
		{
			floatingTime = 0;
		}
		whale_->SetRotate({ whale_->GetRotate().x, whale_->GetRotate().y + 0.01f, whale_->GetRotate().z });
		whale_->Update();
		Input::GetInstance()->GetJoystickState(0, joyState);
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && isStageSelect == false || Input::GetInstance()->TriggerKey(DIK_SPACE) && isStageSelect == false)
		{
			AudioManager::GetInstance()->SoundPlayWave(AudioManager::GetInstance()->GetxAudio2().Get(), enterSound);
			isStageSelect = true;
		}
		if (isStageSelect == true && isSceneTransition == false)//stageselect
		{
			if (cooltime <= 10)
			{
				cooltime++;
			}
			else
			{
				//if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN || Input::GetInstance()->TriggerKey(DIK_DOWN))
				//{
				//	AudioManager::GetInstance()->SoundPlayWave(AudioManager::GetInstance()->GetxAudio2().Get(), moveSound);
				//	nowStage++;
				//	if (nowStage > 4)
				//	{
				//		nowStage = 1;
				//	}
				//	cursor_->SetTranslate({ uiPlanes[nowStage]->GetTranslate().x - 2.5f,uiPlanes[nowStage]->GetTranslate().y,uiPlanes[nowStage]->GetTranslate().z });
				//	cooltime = 0;
				//}
				//else if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP || Input::GetInstance()->TriggerKey(DIK_UP))
				//{
				//	AudioManager::GetInstance()->SoundPlayWave(AudioManager::GetInstance()->GetxAudio2().Get(), moveSound);
				//	nowStage--;
				//	if (nowStage < 1)
				//	{
				//		nowStage = 4;
				//	}
				//	cursor_->SetTranslate({ uiPlanes[nowStage]->GetTranslate().x - 2.5f,uiPlanes[nowStage]->GetTranslate().y,uiPlanes[nowStage]->GetTranslate().z });
				//	cooltime = 0;
				//}
				cursor_->SetTranslate({ uiPlanes[1]->GetTranslate().x - 2.5f,uiPlanes[1]->GetTranslate().y,uiPlanes[1]->GetTranslate().z });
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && isSceneTransition == false || Input::GetInstance()->TriggerKey(DIK_SPACE) && isSceneTransition == false)
				{
					AudioManager::GetInstance()->SoundPlayWave(AudioManager::GetInstance()->GetxAudio2().Get(), enterSound);
					isSceneTransition = true;
					cooltime = 0;
				}

			}
		}
		if (isSceneTransition == true)//gameSceneへ遷移
		{
			sceneTransitionTime++;
			if (sceneTransitionTime >= 20)
			{
				Camera::GetInstance()->SetTranslate({ Camera::GetInstance()->GetTranslate().x, Camera::GetInstance()->GetTranslate().y - 3.0f, Camera::GetInstance()->GetTranslate().z });
				uiPlanes[nowStage]->SetTranslate({ uiPlanes[nowStage]->GetTranslate().x,uiPlanes[nowStage]->GetTranslate().y - 2.75f,uiPlanes[nowStage]->GetTranslate().z });
				whale_->SetTranslate({ whale_->GetTranslate().x,whale_->GetTranslate().y - 2.75f,whale_->GetTranslate().z });
			}
			if (sceneTransitionTime >= 40)
			{
				uiPlanes[5]->SetTranslate({ Camera::GetInstance()->GetTranslate().x, Camera::GetInstance()->GetTranslate().y, Camera::GetInstance()->GetTranslate().z + 2.0f });
				uiPlanes[5]->SetColor({ uiPlanes[5]->GetColor().x,uiPlanes[5]->GetColor().y,uiPlanes[5]->GetColor().z,uiPlanes[5]->GetColor().a - 0.02f });
				uiPlanes[5]->Update();
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
		whale_->Draw();
		if (isStageSelect == false)
		{
			uiPlanes[0]->Draw();
		}
		else if (isSceneTransition == false)
		{
			cursor_->Draw();
			uiPlanes[1]->Draw();
			uiPlanes[2]->Draw();
			uiPlanes[3]->Draw();
			uiPlanes[4]->Draw();
		}
		else if (isSceneTransition == true)
		{
			uiPlanes[nowStage]->Draw();
			uiPlanes[5]->Draw();
		}
		if (isSceneTransition == false)
		{
			//for (Particle* particle : particles)
			//{
			//	particle->Draw();
			//}
		}
	}

	void TitleScene::Finalize()
	{
		whale_.reset();
		//player_.reset();
		uiPlanes.clear();
		AudioManager::GetInstance()->SoundUnload(&bgm);
		AudioManager::GetInstance()->SoundUnload(&enterSound);
		AudioManager::GetInstance()->SoundUnload(&moveSound);
	};
}
