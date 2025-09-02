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
			}

			if ((float)joyState.Gamepad.sThumbLX != 0.0f || (float)joyState.Gamepad.sThumbLY != 0.0f)
			{
				object3d_->SetIsAnimation(true);
			}
			else
			{
				object3d_->SetIsAnimation(false);
			}


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
	}
	else if (isHit == true)
	{
		object3d_->SetIsAnimation(false);
		object3d_->SetColor({ 1.0f,0.3f,0.3f,0.15f });
		hitTimer++;
		if (hitTimer == 60)
		{
			isHit = false;
			object3d_->SetIsAnimation(true);
			isMovable = true;
		}
	}

	object3d_->SkeltonUpdate(camera_);
	
	collision =
	{
		{object3d_->GetTranslate().x - collisionwidth,object3d_->GetTranslate().y - collisionwidth,object3d_->GetTranslate().z - collisionwidth},
		{object3d_->GetTranslate().x + collisionwidth,object3d_->GetTranslate().y + collisionwidth,object3d_->GetTranslate().z + collisionwidth}
	};
	for (const auto& bullet : pBullets)
	{
		bullet->Update();
	}

}

void Player::Draw()
{
	object3d_->SkeltonDraw(modelManager_->GetModelCommon());
}

void Player::Shot()
{
}

void Player::Move()
{
	object3d_->SetTranslate(Add(object3d_->GetTranslate(), moveVector));
}

void Player::Guard(Vector3 whalePos)
{
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
