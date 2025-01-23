#include "titleScene.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

namespace MyEngine
{
	void TitleScene::Initialize()
	{
		std::string PNGs[NumArgument] =
		{
			"Resource/sinkaiTitle4.png",
			"Resource/practice.png",
			"Resource/Stage1.png",
			"Resource/Stage2.png",
			"Resource/Stage3.png",
			"Resource/black.png",
		};

		camera_ = Camera::GetInstance();
		player_ = std::make_unique<Player>();
		whale_ = std::make_unique<Whale>();
		cursor_ = std::make_unique<Cursor>();

		player_->Initialize(camera_);
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

		for (uint32_t i = 0; i < NumArgument; i++)
		{
			std::unique_ptr<UIPlane> uiPlane = std::make_unique<UIPlane>();
			uiPlane->Initialize(Planes[i], PNGs[i]);
			uiPlanes_.push_back(std::move(uiPlane));
			uiPlanes_[i]->SetTranslate(UIPos[i]);
			uiPlanes_[i]->SetScale(UIScale[i]);
		}

		uiPlanes_[Blackout]->SetTranslate({ camera_->GetTranslate().x, camera_->GetTranslate().y, camera_->GetTranslate().z + zfar });
		uiPlanes_[Blackout]->SetColor({ 0.0f,0.0f,0.0f,0.0f });

		cursor_->Initialize();
		cursor_->SetTranslate({ uiPlanes_[Practice]->GetTranslate().x + cursorX,uiPlanes_[Practice]->GetTranslate().y,uiPlanes_[Practice]->GetTranslate().z });
		camera_->SetTranslate({ 0.0f,2.0f,-20.0f });
		camera_->SetRotate({ 0.1f,0.0f,0.0f });
	}

	void TitleScene::Update()
	{
		XINPUT_STATE joyState;
		//for (Particle* particle : particles)
		//{
		//	particle->Update(true, particle->GetElements());
		//}
		for (int i = 0; i < uiPlanes_.size(); i++)
		{
			uiPlanes_[i]->Update();
		}
		cursor_->Update();

		if (floatingTime < floatingTimes[0])//title上昇
		{
			uiPlanes_[Title]->SetTranslate({ uiPlanes_[Title]->GetTranslate().x,uiPlanes_[Title]->GetTranslate().y - uiFloating,uiPlanes_[Title]->GetTranslate().z });
			whale_->SetTranslate({ whale_->GetTranslate().x,whale_->GetTranslate().y - whaleFloating,whale_->GetTranslate().z });
			floatingTime++;
		}
		else if (floatingTime >= floatingTimes[0] && floatingTime < floatingTimes[1])//title下降
		{
			uiPlanes_[Title]->SetTranslate({ uiPlanes_[Title]->GetTranslate().x,uiPlanes_[Title]->GetTranslate().y + uiFloating,uiPlanes_[Title]->GetTranslate().z });
			whale_->SetTranslate({ whale_->GetTranslate().x,whale_->GetTranslate().y + whaleFloating,whale_->GetTranslate().z });
			floatingTime++;
		}
		else if (floatingTimes[1])
		{
			floatingTime = 0;
		}
		whale_->SetRotate({ whale_->GetRotate().x, whale_->GetRotate().y + whaleRotateY, whale_->GetRotate().z });
		whale_->Update();
		Input::GetInstance()->GetJoystickState(0, joyState);
		if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER && isStageSelect == false || Input::GetInstance()->TriggerKey(DIK_SPACE) && isStageSelect == false)
		{
			AudioManager::GetInstance()->SoundPlayWave(AudioManager::GetInstance()->GetxAudio2().Get(), enterSound);
			isStageSelect = true;
		}
		if (isStageSelect == true && isSceneTransition == false)//stageselect
		{
			if (cooltime <= maxCoolTime)
			{
				cooltime++;
			}
			else
			{
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN || Input::GetInstance()->TriggerKey(DIK_DOWN))
				{
					AudioManager::GetInstance()->SoundPlayWave(AudioManager::GetInstance()->GetxAudio2().Get(), moveSound);
					nowStage++;
					if (nowStage > Stage3)//一番下に移動
					{
						nowStage = Practice;
					}
					cursor_->SetTranslate({ uiPlanes_[nowStage]->GetTranslate().x + cursorX,uiPlanes_[nowStage]->GetTranslate().y,uiPlanes_[nowStage]->GetTranslate().z });
					cooltime = 0;
				}
				else if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP || Input::GetInstance()->TriggerKey(DIK_UP))
				{
					AudioManager::GetInstance()->SoundPlayWave(AudioManager::GetInstance()->GetxAudio2().Get(), moveSound);
					nowStage--;
					if (nowStage < Practice)//一番上に移動
					{
						nowStage = Stage3;
					}
					cursor_->SetTranslate({ uiPlanes_[nowStage]->GetTranslate().x + cursorX,uiPlanes_[nowStage]->GetTranslate().y,uiPlanes_[nowStage]->GetTranslate().z });
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
		if (isSceneTransition == true)//gameSceneへ遷移
		{
			sceneTransitionTime++;
			if (sceneTransitionTime >= sceneTransitionTimes[0])//潜行開始
			{
				camera_->SetTranslate({ camera_->GetTranslate().x, camera_->GetTranslate().y - 3.0f, camera_->GetTranslate().z });
				uiPlanes_[nowStage]->SetTranslate({ uiPlanes_[nowStage]->GetTranslate().x,uiPlanes_[nowStage]->GetTranslate().y - 2.75f,uiPlanes_[nowStage]->GetTranslate().z });
				whale_->SetTranslate({ whale_->GetTranslate().x,whale_->GetTranslate().y - 2.75f,whale_->GetTranslate().z });
			}
			if (sceneTransitionTime >= sceneTransitionTimes[1])//暗転開始
			{
				uiPlanes_[Blackout]->SetTranslate({ camera_->GetTranslate().x, camera_->GetTranslate().y, camera_->GetTranslate().z + 2.0f });
				uiPlanes_[Blackout]->SetColor({ uiPlanes_[Blackout]->GetColor().x,uiPlanes_[Blackout]->GetColor().y,uiPlanes_[Blackout]->GetColor().z,uiPlanes_[Blackout]->GetColor().a - 0.02f });
				uiPlanes_[Blackout]->Update();
			}
			if (sceneTransitionTime == sceneTransitionTimes[2])//シーン遷移
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
			uiPlanes_[Title]->Draw();
		}
		else if (isSceneTransition == false)
		{
			cursor_->Draw();
			uiPlanes_[Practice]->Draw();
			uiPlanes_[Stage1]->Draw();
			uiPlanes_[Stage2]->Draw();
			uiPlanes_[Stage3]->Draw();
		}
		else if (isSceneTransition == true)
		{
			uiPlanes_[nowStage]->Draw();
			uiPlanes_[Blackout]->Draw();
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
		for (uint32_t i = 0; i < NumArgument; i++)
		{
			uiPlanes_[i].reset();
		}
		uiPlanes_.clear();
		AudioManager::GetInstance()->SoundUnload(&bgm);
		AudioManager::GetInstance()->SoundUnload(&enterSound);
		AudioManager::GetInstance()->SoundUnload(&moveSound);
	};
}
