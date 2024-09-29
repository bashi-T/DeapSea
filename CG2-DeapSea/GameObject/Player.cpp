#include "Player.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

Player::~Player()
{
	delete object3d;
	object3d = NULL;
	for (PlayerBullet* bullet : pBullets)
	{
		delete bullet;
		bullet = NULL;
	}
}

void Player::Initialize()
{
	object3d = new Object3d; 
	object3d->Initialize(Object3dCommon::GetInstance(), SRVManager::GetInstance());
	const std::string playerModel = "human/walk.gltf";
	const std::string playerSkin = "Resource/monsterBall.png";
	ModelManager::GetInstance()->LoadSkeltonAnimation(playerModel, playerSkin, SRVManager::GetInstance());
	object3d->SetModel(playerModel);
	Model* model = ModelManager::GetInstance()->FindModel(playerModel);
	Model::ModelData* modelData = model->GetModelData();
	for (Model::VertexData& vertex : modelData->vertices)
	{
		vertex.normal.x = vertex.position.x;
		vertex.normal.y = vertex.position.y;
		vertex.normal.z = vertex.position.z;
	}
	model->Memcpy();
	pCollision.center = object3d->GetTranslate();
	pCollision.orientations[0] = { 1.0f,0.0f,0.0f };
	pCollision.orientations[1] = { 0.0f,1.0f,0.0f };
	pCollision.orientations[2] = { 0.0f,0.0f,1.0f };
	pCollision.size = { 0.5f,1.0f,0.5f };
	angle_ = 0.0f;
}

void Player::Update()
{
	pBullets.remove_if([](PlayerBullet* bullet)
		{
			if (bullet->IsDead())
			{
				delete bullet;
				return true;
			}
			return false;
		});
	XINPUT_STATE joyState;
	Vector3 move = { 0.0f,0.0f,0.0f };
	if (isHit == false)
	{
		if (Input::GetInstance()->GetJoystickState(0, joyState))
		{
			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
			{
			}
			else
			{
				move =
				{ 
					(float)joyState.Gamepad.sThumbLX / (SHRT_MAX),
					0.0f,
					(float)joyState.Gamepad.sThumbLY / (SHRT_MAX)
				};
				float inputMagnitude = Length(move);
				// スティックの入力に応じて速度を調整する
				float adjustedSpeed = 1.0f * inputMagnitude;

				// 最大速度を超えないようにする
				if (adjustedSpeed > 1.0f)
				{
					adjustedSpeed = 1.0f;
				}

				// 実際の移動量を計算
				move.x *= adjustedSpeed;
				move.z *= adjustedSpeed;


			}
		}

		if ((float)joyState.Gamepad.sThumbLX != 0.0f || (float)joyState.Gamepad.sThumbLY != 0.0f)
		{
			object3d->SetIsAnimation(true);
		}
		else
		{
			object3d->SetIsAnimation(false);
		}

		//if ((float)joyState.Gamepad.sThumbLY > 0)
		//{
		//	object3d->SetRotate({ 0.0f,0.0f,0.0f });
		//}
		//if ((float)joyState.Gamepad.sThumbLY < 0)
		//{
		//	object3d->SetRotate({ 0.0f,3.0f,0.0f });
		//}
		//if ((float)joyState.Gamepad.sThumbLX > 0)
		//{
		//	object3d->SetRotate({ 0.0f,1.5f,0.0f });
		//}
		//if ((float)joyState.Gamepad.sThumbLX < 0)
		//{
		//	object3d->SetRotate({ 0.0f,4.5f,0.0f });
		//}

		angle_ = std::atan2(object3d->GetTranslate().x + (float)joyState.Gamepad.sThumbLX / (SHRT_MAX * 10.0f), object3d->GetTranslate().z + (float)joyState.Gamepad.sThumbLY / (SHRT_MAX * 10.0f));
		object3d->SetRotate({ 0.0f,/*object3d->GetRotate().y+*/LerpShortAngle(object3d->GetRotate().y, angle_,0.1f),0.0f});


		//if (Input::GetInstance()->PushKey(DIK_D))
		//{
		//	object3d->SetTranslate({ object3d->GetTranslate().x + 0.05f, object3d->GetTranslate().y, object3d->GetTranslate().z });
		//}
		//if (Input::GetInstance()->PushKey(DIK_A))
		//{
		//	object3d->SetTranslate({ object3d->GetTranslate().x - 0.05f, object3d->GetTranslate().y, object3d->GetTranslate().z });
		//}

		if (joyState.Gamepad.bRightTrigger || Input::GetInstance()->PushKey(DIK_C))
		{
			isShot = true;
		}
		else
		{
			isShot = false;
		}
		Shot();

		object3d->SkeltonUpdate(Camera::GetInstance());
		pCollision.center = object3d->GetTranslate();
		if(hitTimer>=60)
		{
			hitTimer++;
			if (hitTimer == 120)
			{
				hitTimer = 0;
			}
		}
	}
	else
	{
		hitTimer++;
		if (hitTimer == 60)
		{
			isHit = false;
		}
	}
	Camera::GetInstance()->SetTranslate({ object3d->GetTranslate().x,object3d->GetTranslate().y + 3.0f,object3d->GetTranslate().z - 20.0f });
	for (PlayerBullet* bullet : pBullets)
	{
		bullet->Update();
	}
}

void Player::Draw()
{
	object3d->SkeltonDraw(ModelManager::GetInstance()->GetModelCommon());
	for (PlayerBullet* bullet : pBullets)
	{
		bullet->Draw();
	}
}

void Player::Shot()
{
	if (isShot == true)
	{
		shotInterval++;
		if (shotInterval == 1)
		{
			PlayerBullet* newBullet = new PlayerBullet;
			newBullet->Initialize(object3d->GetTranslate(), object3d->GetRotate());
			pBullets.push_back(newBullet);
		}
		if (shotInterval == 15)
		{
			shotInterval = 0;

		}
	}
	else
	{
		shotInterval = 0;
	}
}

void Player::OnCollision()
{
	isHit = true;
}

void Player::Finalize()
{
}
