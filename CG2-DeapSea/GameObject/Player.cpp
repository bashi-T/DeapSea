#include "Player.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

Player::~Player()
{
}

void Player::Initialize()
{
	object3d = std::make_unique<Object3d>();
	object3d->Initialize(Object3dCommon::GetInstance(), SRVManager::GetInstance());
	ModelManager::GetInstance()->LoadSkeltonAnimation(playerModel, playerSkin, SRVManager::GetInstance(), true);
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
	pCollision =
	{
		.min{-2.0f,-2.0f,-2.0f},
		.max{2.0f,2.0f,2.0f}
	};
	//pCollision.radius = 2.0f;
	angle_ = 0.0f;
}

void Player::Update()
{
	pBullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet)
		{
			if (bullet->IsDead())
			{
				bullet.reset();
				return true;
			}
			return false;
		});
	XINPUT_STATE joyState;
	if (isHit == false && isMovable == true)
	{
		if (Input::GetInstance()->GetJoystickState(0, joyState))//joystick操作
		{

			if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER)
			{
			}
			else
			{
				moveVector =
				{
					(float)joyState.Gamepad.sThumbLX / (SHRT_MAX),
					0.0f,
					(float)joyState.Gamepad.sThumbLY / (SHRT_MAX)
				};
				float inputMagnitude = Length(moveVector);
				// スティックの入力に応じて速度を調整する
				float adjustedSpeed = 1.0f * inputMagnitude;

				// 最大速度を超えないようにする
				if (adjustedSpeed > 1.0f)
				{
					adjustedSpeed = 1.0f;
				}

				// 実際の移動量を計算
				moveVector.x *= adjustedSpeed;
				moveVector.z *= adjustedSpeed;

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
			object3d->SetRotate({ 0.0f,/*object3d->GetRotate().y+*/LerpShortAngle(object3d->GetRotate().y, angle_,0.1f),0.0f });


			//if (Input::GetInstance()->PushKey(DIK_D))
			//{
			//	object3d->SetTranslate({ object3d->GetTranslate().x + 0.05f, object3d->GetTranslate().y, object3d->GetTranslate().z });
			//}
			//if (Input::GetInstance()->PushKey(DIK_A))
			//{
			//	object3d->SetTranslate({ object3d->GetTranslate().x - 0.05f, object3d->GetTranslate().y, object3d->GetTranslate().z });
			//}

			if (joyState.Gamepad.bRightTrigger)
			{
				isShot = true;
			}
			else
			{
				isShot = false;
			}
		}
		else//keyboard操作
		{
			moveVector = { 0.0f,0.0f,0.0f };
			if (Input::GetInstance()->PushKey(DIK_RIGHT))
			{
				moveVector = { 0.075f, 0.0f,0.0f };
			}
			if (Input::GetInstance()->PushKey(DIK_LEFT))
			{
				moveVector = { -0.075f, 0.0f,0.0f };
			}
			if (Input::GetInstance()->PushKey(DIK_UP))
			{
				moveVector = { 0.0f, 0.0f,0.075f };
			}
			if (Input::GetInstance()->PushKey(DIK_DOWN))
			{
				moveVector = { 0.0f, 0.0f,-0.075f };
			}

			if (Input::GetInstance()->PushKey(DIK_A))
			{
			}
			else
			{
				object3d->SetTranslate(Add(object3d->GetTranslate(), moveVector));
			}
			if (moveVector.x != 0.0f || moveVector.z != 0.0f)
			{
				object3d->SetIsAnimation(true);
			}
			else
			{
				object3d->SetIsAnimation(false);
			}
			if (moveVector.z > 0)
			{
				object3d->SetRotate({ 0.0f,0.0f,0.0f });
			}
			if (moveVector.z < 0)
			{
				object3d->SetRotate({ 0.0f,3.0f,0.0f });
			}
			if (moveVector.x > 0)
			{
				object3d->SetRotate({ 0.0f,1.5f,0.0f });
			}
			if (moveVector.x < 0)
			{
				object3d->SetRotate({ 0.0f,4.5f,0.0f });
			}

			if (Input::GetInstance()->PushKey(DIK_SPACE))
			{
				isShot = true;
			}
			else
			{
				isShot = false;
			}

		}
		Shot();

		//pCollision.center = object3d->GetTranslate();
		if (hitTimer >= 60)
		{
			hitTimer++;
			if (hitTimer == 120)
			{
				hitTimer = 0;
			}
		}
	}
	else if (isHit == true)
	{
		hitTimer++;
		if (hitTimer == 60)
		{
			isHit = false;
			isMovable = true;
		}
	}

	object3d->SkeltonUpdate(Camera::GetInstance());
	//pCollision.center = object3d->GetTranslate();
	for (const auto& bullet : pBullets)
	{
		bullet->Update();
	}
}

void Player::Draw()
{
	object3d->SkeltonDraw(ModelManager::GetInstance()->GetModelCommon());
	for (const auto& bullet : pBullets)
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
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique< PlayerBullet>();
			newBullet->Initialize(object3d->GetTranslate(), object3d->GetRotate());
			pBullets.push_back(std::move(newBullet));
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
	isMovable = false;
}
