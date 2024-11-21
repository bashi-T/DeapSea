#include "Whale.h"

	Whale::~Whale()
	{
	}

	void Whale::Initialize(Player* players)
	{
		player = players;
		object3d = std::make_unique<Object3d>();
		object3d->Initialize(Object3dCommon::GetInstance(), SRVManager::GetInstance());
		ModelManager::GetInstance()->LoadModel(whaleModel, whaleSkin, true);
		object3d->SetModel(whaleModel);
		Model* model = ModelManager::GetInstance()->FindModel(whaleModel);
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

		particle = std::make_unique<Particle>();
		particle->SetElements(1.0f, 1.0f, 1.0f, 2.0f,
			-7.0f, 7.0f, -6.0f, -4.0f, 0.0f, 0.0f,
			0.0f, 0.0f, 6.0f, 12.0f, 0.0f, 0.0f);
		particle->Initialize("Resource/bloodParticle.png", ParticleCommon::GetInstance(), SRVManager::GetInstance(), Object3dCommon::GetInstance(), particle->GetElements());

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
				switch (life)//ダメージ表現パーティクル
				{
				case 0:
					ChangeModel("whale/BoneWhale.obj", "Resource/boneColor.png");
					particle->SetElements(1.0f, 1.0f, 0.5f, 1.5f,
						object3d->GetTranslate().x - 4.0f, object3d->GetTranslate().x + 4.0f,
						object3d->GetTranslate().y - 2.0f, object3d->GetTranslate().y + 2.0f,
						object3d->GetTranslate().z - 4.0f, object3d->GetTranslate().z - 2.0f,
						-1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);
					particle->RandomInitialize(particle->GetElements());
					break;
				case 1:
					ChangeModel("whale/3DamagedWhale.obj", "Resource/3DamageBone.png");
					particle->SetElements(1.0f, 1.0f, 0.5f, 1.5f,
						object3d->GetTranslate().x - 4.0f, object3d->GetTranslate().x + 4.0f,
						object3d->GetTranslate().y - 2.0f, object3d->GetTranslate().y + 2.0f,
						object3d->GetTranslate().z - 4.0f, object3d->GetTranslate().z - 2.0f,
						-1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);
					particle->RandomInitialize(particle->GetElements());

					break;
				case 2:
					ChangeModel("whale/2DamagedWhale.obj", "Resource/2DamageBone.png");
					particle->SetElements(1.0f, 1.0f, 0.5f, 1.5f,
						object3d->GetTranslate().x - 4.0f, object3d->GetTranslate().x + 4.0f,
						object3d->GetTranslate().y - 2.0f, object3d->GetTranslate().y + 2.0f,
						object3d->GetTranslate().z - 4.0f, object3d->GetTranslate().z - 2.0f,
						-1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);
					particle->RandomInitialize(particle->GetElements());

					break;

				case 3:
					ChangeModel("whale/1DamagedWhale.obj", "Resource/1DamagedBone.png");
					particle->SetElements(1.0f, 1.0f, 0.5f, 1.5f,
						object3d->GetTranslate().x - 4.0f, object3d->GetTranslate().x + 4.0f,
						object3d->GetTranslate().y - 2.0f, object3d->GetTranslate().y + 2.0f,
						object3d->GetTranslate().z - 4.0f, object3d->GetTranslate().z - 2.0f,
						-1.0f, 1.0f, 0.0f, 1.0f, -1.0f, 1.0f);
					particle->RandomInitialize(particle->GetElements());

					break;
				}
			}

			//object3d->SetRotate({ object3d->GetRotate().x,object3d->GetRotate().y + 0.3f,object3d->GetRotate().z });
			coolTimer++;
			if (coolTimer == 120)
			{
				isHit = false;
				coolTimer = 0;
				object3d->SetRotate({ 0.0f,0.0f,0.0f });
			}
		}

		nowWhaleSpeed = { (whaleSpeed.x * accSpeed.x) ,0.0f,(whaleSpeed.z * accSpeed.z) };
		object3d->SetTranslate(Add(object3d->GetTranslate(), nowWhaleSpeed));
		object3d->Update(Camera::GetInstance());
		particle->Update(false, particle->GetElements());

#ifdef _DEBUG
		ImGui::Begin("whale");
		ImGui::DragFloat3("whale.translate", (float*)&object3d->GetTranslate(), 0.01f);
		ImGui::Text("life:%d", life);
		ImGui::End();
#endif
	}

	void Whale::Draw()
	{
		object3d->Draw(ModelManager::GetInstance()->GetModelCommon());
		if (life != 4)
		{
			particle->Draw();
		}
	}

	void Whale::OnCollision()
	{
		life -= 1;
		isHit = true;
	}

	void Whale::SetTranslate(Vector3 translate)
	{
		object3d->SetTranslate(translate);
	}

	void Whale::SetRotate(Vector3 rotate)
	{
		object3d->SetRotate(rotate);
	}

	void Whale::ChangeModel(std::string shape, std::string skin)
	{
		ModelManager::GetInstance()->EraseModel(whaleModel, whaleSkin);
		whaleModel = shape;
		whaleSkin = skin;
		ModelManager::GetInstance()->LoadModel(whaleModel, whaleSkin, true);
		object3d->SetModel(whaleModel);
		Model* model = ModelManager::GetInstance()->FindModel(whaleModel);
		Model::ModelData* modelData = model->GetModelData();
		for (Model::VertexData& vertex : modelData->vertices)
		{
			vertex.normal.x = vertex.position.x;
			vertex.normal.y = vertex.position.y;
			vertex.normal.z = vertex.position.z;
		}
		model->Memcpy();
	}