#include "Whale.h"

	Whale::~Whale()
	{
	}

	void Whale::Initialize(Player* players)
	{
		player = players;
		object3d_ = std::make_unique<Object3d>();
		object3d_->Initialize();
		modelManager_ = ModelManager::GetInstance();
		modelManager_->LoadModel(whaleModel, whaleSkin, true);
		object3d_->SetModel(whaleModel);
		Model* model = modelManager_->FindModel(whaleModel);
		Model::ModelData* modelData = model->GetModelData();
		for (Model::VertexData& vertex : modelData->vertices)
		{
			vertex.normal.x = vertex.position.x;
			vertex.normal.y = vertex.position.y;
			vertex.normal.z = vertex.position.z;
		}
		model->Memcpy();
		life = 4;
		coolTimer = 0;
		wCollision =
		{
			.min{-3.0f,-1.0f,-1.0f},
			.max{3.0f,1.0f,1.0f}
		};

		particle_ = std::make_unique<Particle>();
		particle_->SetElements(0.0f, 1.0f, 1.0f, 2.0f,
			-7.0f, 7.0f, -6.0f, -4.0f, 0.0f, 0.0f,
			-0.005f, 0.005f, 0.005f, 0.01f, 0.0f, 0.0f);
		particle_->Initialize("Resource/bloodParticle.png", particle_->GetElements(), 300);
	}

	void Whale::Update()
	{
		XINPUT_STATE joyState;
		if (player->GetIsHit() == false)
		{
			if (Input::GetInstance()->GetJoystickState(0, joyState))
			{
			}
			if (player->GetMoveVector().x > 0.0f)
			{
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER || Input::GetInstance()->PushKey(DIK_S))
				{
					accSpeed.x -= 0.01f;
					if (accSpeed.x <= 0.0f)
					{
						accSpeed.x = 0.0f;
					}
				}
				else
				{
					accSpeed.x += 0.01f;
					if (accSpeed.x >= 1.0f)
					{
						accSpeed.x = 1.0f;
					}
				}
			}
			else if (player->GetMoveVector().x < 0.0f)
			{
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER || Input::GetInstance()->PushKey(DIK_S))
				{
					accSpeed.x += 0.01f;
					if (accSpeed.x >= 0.0f)
					{
						accSpeed.x = 0.0f;
					}
				}
				else
				{
					accSpeed.x -= 0.01f;
					if (accSpeed.x <= -1.0f)
					{
						accSpeed.x = -1.0f;
					}
				}
			}
			else
			{
				if (accSpeed.x > 0.0f)
				{
					accSpeed.x -= 0.01f;
				}
				else if (accSpeed.x < 0.0f)
				{
					accSpeed.x += 0.01f;
				}
				else if (accSpeed.x == 0.0f)
				{
					accSpeed.x = 0.0f;
				}
			}

			if (player->GetMoveVector().z > 0.0f)
			{
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER || Input::GetInstance()->PushKey(DIK_S))
				{
					accSpeed.z -= 0.01f;
					if (accSpeed.z <= 0.0f)
					{
						accSpeed.z = 0.0f;
					}
				}
				else
				{
					accSpeed.z += 0.01f;
					if (accSpeed.z >= 1.0f)
					{
						accSpeed.z = 1.0f;
					}
				}
			}
			else if (player->GetMoveVector().z < 0.0f)
			{
				if (joyState.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER || Input::GetInstance()->PushKey(DIK_S))
				{
					accSpeed.z += 0.01f;
					if (accSpeed.z >= 0.0f)
					{
						accSpeed.z = 0.0f;
					}
				}
				else
				{
					accSpeed.z -= 0.01f;
					if (accSpeed.z <= -1.0f)
					{
						accSpeed.z = -1.0f;
					}
				}
			}
			else
			{
				if (accSpeed.z > 0.0f)
				{
					accSpeed.z -= 0.01f;
				}
				else if (accSpeed.z < 0.0f)
				{
					accSpeed.z += 0.01f;
				}
				else if (accSpeed.z == 0.0f)
				{
					accSpeed.z = 0.0f;
				}
			}
		}

		if (isHit == true)
		{
			if (coolTimer == 0)
			{
				ChangeModel(life);
			}

			//object3d_->SetRotate({ object3d_->GetRotate().x,object3d_->GetRotate().y + 0.3f,object3d_->GetRotate().z });
			coolTimer++;
			if (coolTimer == 120)
			{
				isHit = false;
				coolTimer = 0;
				SetColor({ 1.0f,1.0f,1.0f,1.0f });
			}
		}

		nowWhaleSpeed = { (whaleSpeed.x * accSpeed.x) ,0.0f,(whaleSpeed.z * accSpeed.z) };
		object3d_->SetTranslate(Add(object3d_->GetTranslate(), nowWhaleSpeed));
		object3d_->Update(Camera::GetInstance());
		
		wCollision =
		{
			.min{object3d_->GetTranslate().x - 3.0f,object3d_->GetTranslate().y - 1.0f,object3d_->GetTranslate().z - 1.0f},
			.max{object3d_->GetTranslate().x + 3.0f,object3d_->GetTranslate().y + 1.0f,object3d_->GetTranslate().z + 1.0f}
		};

		particle_->Update(false, particle_->GetElements());

#ifdef _DEBUG
		ImGui::Begin("whale");
		ImGui::DragFloat3("whale.translate", (float*)&object3d_->GetTranslate(), 0.01f);
		ImGui::Text("life:%d", life);
		ImGui::End();
#endif
	}

	void Whale::Draw()
	{
		object3d_->Draw(modelManager_->GetModelCommon());
		if (life != 4)
		{
			particle_->Draw();
		}
	}

	void Whale::SetMaxPosition(float pos, float originalPos, float maxPos)
	{
	    if (pos >= originalPos + maxPos)
		{
		    pos = originalPos + maxPos;
		}
	    if (pos <= originalPos - maxPos)
		{
		    pos = originalPos - maxPos;
		}
	}

	void Whale::ChangeModel(int32_t whaleLife)
	{
		switch (whaleLife)//ダメージ表現パーティクル
		{
		case 0:
			ChangeModel("whale/BoneWhale.obj", "Resource/boneColor.png");
			particle_->SetElements(0.0f, 1.0f, 0.5f, 1.5f,
				object3d_->GetTranslate().x - 4.0f, object3d_->GetTranslate().x + 4.0f,
				object3d_->GetTranslate().y - 2.0f, object3d_->GetTranslate().y + 2.0f,
				object3d_->GetTranslate().z - 4.0f, object3d_->GetTranslate().z - 2.0f,
				-0.005f, 0.005f, 0.005f, 0.01f, -0.005f, 0.005f);
			particle_->RandomInitialize(particle_->GetElements());
			particle_->SetColorRed(1.0f);

			break;
		case 1:
			ChangeModel("whale/3DamagedWhale.obj", "Resource/3DamageBone.png");
			particle_->SetElements(0.0f, 1.0f, 0.5f, 1.5f,
				object3d_->GetTranslate().x - 4.0f, object3d_->GetTranslate().x + 4.0f,
				object3d_->GetTranslate().y - 2.0f, object3d_->GetTranslate().y + 2.0f,
				object3d_->GetTranslate().z - 4.0f, object3d_->GetTranslate().z - 2.0f,
				-0.005f, 0.005f, 0.005f, 0.01f, -0.005f, 0.005f);
			particle_->RandomInitialize(particle_->GetElements());
			particle_->SetColorRed(1.0f);

			break;
		case 2:
			ChangeModel("whale/2DamagedWhale.obj", "Resource/2DamageBone.png");
			particle_->SetElements(0.0f, 1.0f, 0.5f, 1.5f,
				object3d_->GetTranslate().x - 4.0f, object3d_->GetTranslate().x + 4.0f,
				object3d_->GetTranslate().y - 2.0f, object3d_->GetTranslate().y + 2.0f,
				object3d_->GetTranslate().z - 4.0f, object3d_->GetTranslate().z - 2.0f,
				-0.005f, 0.005f, 0.005f, 0.01f, -0.005f, 0.005f);
			particle_->RandomInitialize(particle_->GetElements());
			particle_->SetColorRed(1.0f);

			break;

		case 3:
			ChangeModel("whale/1DamagedWhale.obj", "Resource/1DamagedBone.png");
			particle_->SetElements(0.0f, 1.0f, 0.5f, 1.5f,
				object3d_->GetTranslate().x - 4.0f, object3d_->GetTranslate().x + 4.0f,
				object3d_->GetTranslate().y - 2.0f, object3d_->GetTranslate().y + 2.0f,
				object3d_->GetTranslate().z - 4.0f, object3d_->GetTranslate().z - 2.0f,
				-0.005f, 0.005f, 0.005f, 0.01f, -0.005f, 0.005f);
			particle_->RandomInitialize(particle_->GetElements());
			particle_->SetColorRed(1.0f);

			break;
		}
	}

	void Whale::OnCollision()
	{
		life -= 1;
		isHit = true;
	}

	void Whale::OnTideCollision(Vector3 tideVector)
	{
		object3d_->SetTranslate(Add(object3d_->GetTranslate(),tideVector));
	}

	void Whale::SetTranslate(Vector3 translate)
	{
		object3d_->SetTranslate(translate);
	}

	void Whale::SetRotate(Vector3 rotate)
	{
		object3d_->SetRotate(rotate);
	}

	void Whale::ChangeModel(std::string shape, std::string skin)
	{
		modelManager_->EraseModel(whaleModel, whaleSkin);
		whaleModel = shape;
		whaleSkin = skin;
		modelManager_->LoadModel(whaleModel, whaleSkin, true);
		object3d_->SetModel(whaleModel);
		Model* model = modelManager_->FindModel(whaleModel);
		Model::ModelData* modelData = model->GetModelData();
		for (Model::VertexData& vertex : modelData->vertices)
		{
			vertex.normal.x = vertex.position.x;
			vertex.normal.y = vertex.position.y;
			vertex.normal.z = vertex.position.z;
		}
		model->Memcpy();
	}