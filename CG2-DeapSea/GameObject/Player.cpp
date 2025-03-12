#include "Player.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

Player::~Player()
{
}

void Player::Initialize(Camera* camera)
{
	object3d_ = std::make_unique<Object3d>();
	object3d_->Initialize();
	camera_ = camera;
	modelManager_ = ModelManager::GetInstance();
	modelManager_->LoadSkeltonAnimation(playerModel, playerSkin, true);
	object3d_->SetModel(playerModel);
	Model* model = modelManager_->FindModel(playerModel);
	Model::ModelData* modelData = model->GetModelData();
	for (Model::VertexData& vertex : modelData->vertices)
	{
		vertex.normal.x = vertex.position.x;
		vertex.normal.y = vertex.position.y;
		vertex.normal.z = vertex.position.z;
	}
	model->Memcpy();
	collision =
	{
		.min{-collisionwidth,-collisionwidth,-collisionwidth},
		.max{collisionwidth,collisionwidth,collisionwidth}
	};
	//collision.radius = 2.0f;
	angle_ = 0.0f;
	object3d_->SetScale({2.0f, 2.0f, 2.0f});
}

void Player::Update(Vector3 whalePos)
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
				object3d_->SetIsAnimation(true);
			}
			else
			{
				object3d_->SetIsAnimation(false);
			}

			//if ((float)joyState.Gamepad.sThumbLY > 0)
			//{
			//	object3d_->SetRotate({ 0.0f,0.0f,0.0f });
			//}
			//if ((float)joyState.Gamepad.sThumbLY < 0)
			//{
			//	object3d_->SetRotate({ 0.0f,3.0f,0.0f });
			//}
			//if ((float)joyState.Gamepad.sThumbLX > 0)
			//{
			//	object3d_->SetRotate({ 0.0f,1.5f,0.0f });
			//}
			//if ((float)joyState.Gamepad.sThumbLX < 0)
			//{
			//	object3d_->SetRotate({ 0.0f,4.5f,0.0f });
			//}

			angle_ = std::atan2(object3d_->GetTranslate().x + (float)joyState.Gamepad.sThumbLX / (SHRT_MAX * 10.0f), object3d_->GetTranslate().z + (float)joyState.Gamepad.sThumbLY / (SHRT_MAX * 10.0f));
			object3d_->SetRotate({ 0.0f,/*object3d_->GetRotate().y+*/LerpShortAngle(object3d_->GetRotate().y, angle_,0.1f),0.0f });
			Move();

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
				moveVector = {moveSpeed, 0.0f, 0.0f};
			}
			if (Input::GetInstance()->PushKey(DIK_LEFT))
			{
				moveVector = {-moveSpeed, 0.0f, 0.0f};
			}
			if (Input::GetInstance()->PushKey(DIK_UP))
			{
				moveVector = {0.0f, 0.0f, moveSpeed};
			}
			if (Input::GetInstance()->PushKey(DIK_DOWN))
			{
				moveVector = {0.0f, 0.0f, -moveSpeed};
			}

			Move();

			if (moveVector.x != 0.0f || moveVector.z != 0.0f)
			{
				object3d_->SetIsAnimation(true);
			}
			else
			{
				object3d_->SetIsAnimation(false);
			}
			if (moveVector.z > 0)
			{
				object3d_->SetRotate({ 0.0f,0.0f,0.0f });
			}
			if (moveVector.z < 0)
			{
				object3d_->SetRotate({ 0.0f,3.14f,0.0f });
			}
			if (moveVector.x > 0)
			{
				object3d_->SetRotate({ 0.0f,1.57f,0.0f });
			}
			if (moveVector.x < 0)
			{
				object3d_->SetRotate({ 0.0f,4.71f,0.0f });
			}

			if (Input::GetInstance()->PushKey(DIK_SPACE))
			{
				isShot = true;
			}
			else
			{
				isShot = false;
			}

			if (Input::GetInstance()->PushKey(DIK_V) && isGuard==false)
			{
				isGuard = true;
			}
		}
		Shot();
		Guard(whalePos);
		if (hitTimer >= 60)
		{
			object3d_->SetColor({ 1.0f,1.0f,1.0f,0.25f });
			hitTimer++;
			if (hitTimer == 120)
			{
				hitTimer = 0;
			}
		}
	}
	else if (isHit == true)
	{
		object3d_->SetIsAnimation(false);
		object3d_->SetColor({ 1.0f,1.0f,1.0f,0.15f });
		hitTimer++;
		if (hitTimer == 60)
		{
			isHit = false;
			object3d_->SetIsAnimation(true);
			isMovable = true;
		}
	}

	object3d_->SkeltonUpdate(camera_);
	
	//collision.center = object3d_->GetTranslate();
	collision =
	{
		{object3d_->GetTranslate().x - collisionwidth,object3d_->GetTranslate().y - collisionwidth,object3d_->GetTranslate().z - collisionwidth},
		{object3d_->GetTranslate().x + collisionwidth,object3d_->GetTranslate().y + collisionwidth,object3d_->GetTranslate().z + collisionwidth}
	};
	for (const auto& bullet : pBullets)
	{
		bullet->Update();
	}

	#ifdef _DEBUG
	if (shieldTimer < 0)
	{
		object3d_->SetColor({0.0f, 0.0f, 1.0f, 1.0f});
	}
	else
	{
		object3d_->SetColor({1.0f, 1.0f, 1.0f, 1.0f});
	}
	#endif
}

void Player::Draw()
{
	object3d_->SkeltonDraw(modelManager_->GetModelCommon());
	for (const auto& bullet : pBullets)
	{
		bullet->Draw();
	}

	if (shieldTimer > 0)
	{
		shield_->Draw();
	}
}

void Player::Shot()
{
	if (isShot == true)
	{
		shotInterval++;
		if (shotInterval == 1)
		{
			std::unique_ptr<PlayerBullet> newBullet = std::make_unique<PlayerBullet>();
			newBullet->Initialize(object3d_->GetTranslate(), object3d_->GetObjectMatrix());
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

void Player::Move()
{
	object3d_->SetTranslate(Add(object3d_->GetTranslate(), moveVector));
}

void Player::Guard(Vector3 whalePos)
{
	if (isGuard && shieldTimer == 0)
	{
		shield_ = std::make_unique<Shield>();
		shield_->Initialize();
		shield_->SetIsDead(false);
	} 
	else if (shield_ == nullptr)
	{
		if (shieldTimer<0)
		{
			shieldTimer++;
		}
		return;
	}
	shieldTimer++;
	shield_->Update(object3d_->GetTranslate(), whalePos);
	if (shieldTimer >= 300)
	{
		shield_->SetIsDead(true);
	}

	if (shield_->IsDead() == true&&shield_->GetRadius()<=0.0f)
	{
		shieldTimer *= -10;
		isGuard = false;
		shield_.reset();
	};
}

void Player::OnCollision()
{
	isHit = true;
	isMovable = false;
}

void Player::SetMaxPosition(float pos, float maxPos)
{
	if (pos >= maxPos)
	{
		pos = maxPos;
	}
	if (pos <= -maxPos)
	{
		pos = -maxPos;
	}
}
