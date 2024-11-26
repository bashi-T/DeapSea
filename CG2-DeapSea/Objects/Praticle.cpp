#include "Particle.h"
#include"Commons/ParticleCommon.h"

namespace MyEngine
{
	Particle::~Particle()
	{
	}

	void Particle::Initialize(const std::string& textureFilePath, ParticleCommon* particleCommon, SRVManager* srvManager, Object3dCommon* object3dCommon, ElementsParticle element,int numMaxInstance)
	{
		this->particleCommon_ = particleCommon;
		this->object3dCommon_ = object3dCommon;
		this->srvManager_ = srvManager;
		this->camera_ = object3dCommon_->GetDefaultCamera();
		kNumMaxInstance = numMaxInstance;

		LeftTop = { -0.5f, 0.5f, 0.0f, 1.0f };
		RightTop = { 0.5f, 0.5f, 0.0f, 1.0f };
		RightBottom = { 0.5f, -0.5f, 0.0f, 1.0f };
		LeftBottom = { -0.5f, -0.5f, 0.0f, 1.0f };
		Color = { 1.0f, 1.0f, 1.0f, 1.0f };

		texcoordLeftTop = { 0.0f,0.0f };
		texcoordRightTop = { 0.0f,1.0f };
		texcoordRightBottom = { 1.0f,1.0f };
		texcoordLeftBottom = { 1.0f,0.0f };

		projectionMatrix = MakePerspectiveFovMatrix(0.65f, float(WinAPP::clientWidth_) / float(WinAPP::clientHeight_), 0.1f, 100.0f);
		vertexResource = CreateBufferResource(particleCommon_, sizeof(VertexData) * 6);
		colorResource = CreateBufferResource(particleCommon_, sizeof(Material));
		indexResource = CreateBufferResource(particleCommon_, sizeof(uint32_t) * 6);
		instancingResource = CreateBufferResource(particleCommon_, sizeof(ParticleForGPU) * kNumMaxInstance);
		cameraResource = CreateBufferResource(particleCommon_, sizeof(CameraTransform));

		MakeBufferView();

		vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
		colorResource->Map(0, nullptr, reinterpret_cast<void**>(&colorData));
		instancingResource->Map(0, nullptr, reinterpret_cast<void**>(&instancingData));
		cameraResource->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));

		RandomInitialize(element);
		InputData(true, element);

		MakeShaderResourceViewInstance();

		materialData.textureFilePath = textureFilePath;
		TextureManager::GetInstance()->LoadTexture(textureFilePath);
		materialData.textureIndex = TextureManager::GetInstance()->GetSrvIndex(textureFilePath);
	}

	void Particle::RandomInitialize(ElementsParticle element)
	{
		std::random_device seedGenerator;
		std::mt19937 randomEngine(seedGenerator());
		for (uint32_t index = 0; index < kNumMaxInstance; ++index)
		{
			particles[index] = MakeNewParticle(randomEngine, element.colorMin, element.colorMax, element.timeMin, element.timeMax);
			particles[index] = MakeNewParticlePosition(randomEngine,
				element.posxMin, element.posxMax, element.posyMin, element.posyMax, element.poszMin, element.poszMax,
				element.velxMin, element.velxMax, element.velyMin, element.velyMax, element.velzMin, element.velzMax);
			instancingData[index].WVP = MakeIdentity4x4();
			instancingData[index].World = MakeIdentity4x4();
			instancingData[index].color = particles[index].color;
		}
	}

	void Particle::Update(bool isRevive, ElementsParticle element)
	{
		InputData(isRevive, element);
	}

	void Particle::Draw()
	{
		particleCommon_->GetDx12Common()->GetCommandList().Get()->SetPipelineState(particleCommon_->GetGraphicsPipelineState().Get());
		particleCommon_->GetDx12Common()->GetCommandList().Get()->SetGraphicsRootSignature(particleCommon_->GetRootSignature().Get());
		particleCommon_->GetDx12Common()->GetCommandList().Get()->IASetPrimitiveTopology(
			D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		particleCommon_->GetDx12Common()->GetCommandList().Get()->IASetVertexBuffers(0, 1, &vertexBufferView);
		particleCommon_->GetDx12Common()->GetCommandList().Get()->
			IASetIndexBuffer(&indexBufferView);

		particleCommon_->GetDx12Common()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(
			0, colorResource->GetGPUVirtualAddress());
		particleCommon_->GetDx12Common()->GetCommandList().Get()->
			SetGraphicsRootDescriptorTable(
				1, instancingSrvHandleGPU);
		srvManager_->SetGraphicsRootDescriptorTable(
			2, materialData.textureIndex);
		particleCommon_->GetDx12Common()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(
			3, cameraResource->GetGPUVirtualAddress());

		particleCommon_->GetDx12Common()->GetCommandList().Get()->DrawIndexedInstanced(6, kNumInstance, 0, 0, 0);
	}

	ComPtr<ID3D12Resource> Particle::CreateBufferResource(ParticleCommon* particleCommon, size_t sizeInBytes)
	{
		this->particleCommon_ = particleCommon;
		D3D12_HEAP_PROPERTIES uploadHeapProperties{};

		uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
		D3D12_RESOURCE_DESC ResourceDesc{};

		ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;

		ResourceDesc.Width = sizeInBytes * 3;

		ResourceDesc.Height = 1;
		ResourceDesc.DepthOrArraySize = 1;
		ResourceDesc.MipLevels = 1;
		ResourceDesc.SampleDesc.Count = 1;

		ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		ComPtr<ID3D12Resource> Resource = nullptr;

		hr = particleCommon->GetDx12Common()->GetDevice().Get()->CreateCommittedResource(
			&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &ResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&Resource));
		assert(SUCCEEDED(hr));
		return Resource;
	}

	void Particle::MakeBufferView()
	{
		vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
		vertexBufferView.SizeInBytes = UINT(sizeof(VertexData)) * 4;
		vertexBufferView.StrideInBytes = sizeof(VertexData);

		indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
		indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
		indexBufferView.Format = DXGI_FORMAT_R32_UINT;
	}

	void Particle::InputData(bool isRevive, ElementsParticle element)
	{
		uint32_t* indexData = nullptr;
		indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

		colorData[0].color = { 1.0f,1.0f,1.0f,1.0f };

		kNumInstance = 0;
		for (uint32_t index = 0; index < kNumMaxInstance; ++index)
		{
			float alpha = 1.0f - (particles[index].currentTime / particles[index].lifeTime);
			Matrix4x4 backToFrontMatrix = MakerotateYMatrix(std::numbers::pi_v<float>);
			Matrix4x4 billboardMatrix = Multiply(backToFrontMatrix, Camera::GetInstance()->GetWorldMatrix());
			billboardMatrix.m[3][0] = 0.0f;
			billboardMatrix.m[3][1] = 0.0f;
			billboardMatrix.m[3][2] = 0.0f;
			Matrix4x4 scaleMatrix = MakeScaleMatrix(particles[index].transform.scale);
			Matrix4x4 translateMatrix = MakeTranslateMatrix(particles[index].transform.translate);
			Matrix4x4 worldMatrix = Multiply(scaleMatrix, Multiply(billboardMatrix, translateMatrix));

			if (particles[index].lifeTime <= particles[index].currentTime && isRevive == true)
			{
				std::random_device seedGenerator;
				std::mt19937 randomEngine(seedGenerator());
				particles[index] = MakeNewParticle(randomEngine, element.colorMin, element.colorMax, element.timeMin, element.timeMax);
				particles[index] = MakeNewParticlePosition(randomEngine,
					element.posxMin, element.posxMax, element.posyMin, element.posyMax, element.poszMin, element.poszMax,
					element.velxMin, element.velxMax, element.velyMin, element.velyMax, element.velzMin, element.velzMax);
				instancingData[index].WVP = MakeIdentity4x4();
				instancingData[index].World = MakeIdentity4x4();
				instancingData[index].color = particles[index].color;
			}

			particles[index].transform.translate.x += particles[index].velocity.x * kDeltaTime;
			particles[index].transform.translate.y += particles[index].velocity.y * kDeltaTime;
			particles[index].transform.translate.z += particles[index].velocity.z * kDeltaTime;
			particles[index].currentTime += kDeltaTime;
			if (camera_)
			{
				const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
				worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);
			}
			instancingData[kNumInstance].WVP = worldViewProjectionMatrix;
			instancingData[kNumInstance].World = worldMatrix;
			particles[index].color.a = alpha;
			instancingData[kNumInstance].color = particles[index].color;
			++kNumInstance;
		}
		cameraData->worldPosition =
		{
			camera_->GetWorldMatrix().m[3][0],
			camera_->GetWorldMatrix().m[3][1],
			camera_->GetWorldMatrix().m[3][2]
		};


		vertexData[0].position = LeftTop;
		vertexData[0].texcoord = texcoordLeftTop;
		vertexData[0].normal.x = vertexData[0].position.x;
		vertexData[0].normal.y = vertexData[0].position.y;
		vertexData[0].normal.z = vertexData[0].position.z;

		vertexData[1].position = RightTop;
		vertexData[1].texcoord = texcoordRightTop;
		vertexData[1].normal.x = vertexData[1].position.x;
		vertexData[1].normal.y = vertexData[1].position.y;
		vertexData[1].normal.z = vertexData[1].position.z;

		vertexData[2].position = RightBottom;
		vertexData[2].texcoord = texcoordRightBottom;
		vertexData[2].normal.x = vertexData[2].position.x;
		vertexData[2].normal.y = vertexData[2].position.y;
		vertexData[2].normal.z = vertexData[2].position.z;

		vertexData[3].position = LeftBottom;
		vertexData[3].texcoord = texcoordLeftBottom;
		vertexData[3].normal.x = vertexData[3].position.x;
		vertexData[3].normal.y = vertexData[3].position.y;
		vertexData[3].normal.z = vertexData[3].position.z;

		indexData[0] = 0;
		indexData[1] = 1;
		indexData[2] = 2;

		indexData[3] = 0;
		indexData[4] = 2;
		indexData[5] = 3;

	}

	void Particle::MakeShaderResourceViewInstance()
	{
		uint32_t index = srvManager_->Allocate();
		instancingSrvHandleCPU = srvManager_->GetCPUDescriptorHandle(index);
		instancingSrvHandleGPU = srvManager_->GetGPUDescriptorHandle(index);
		srvManager_->CreateSRVforStructuredBuffer(index, instancingResource.Get(), kNumInstance, sizeof(ParticleForGPU));
	}

	Particle::Particles Particle::MakeNewParticlePosition(std::mt19937& randomEngine,
		float posxMin, float posxMax, float posyMin, float posyMax, float poszMin, float poszMax,
		float velxMin, float velxMax, float velyMin, float velyMax, float velzMin, float velzMax)
	{
		std::uniform_real_distribution<float> distPosX(posxMin, posxMax);
		std::uniform_real_distribution<float> distPosY(posyMin, posyMax);
		std::uniform_real_distribution<float> distPosZ(poszMin, poszMax);
		std::uniform_real_distribution<float> distVelocityX(velxMin, velxMax);
		std::uniform_real_distribution<float> distVelocityY(velyMin, velyMax);
		std::uniform_real_distribution<float> distVelocityZ(velzMin, velzMax);
		Particles particle;
		particle.transform.translate = { distPosX(randomEngine),distPosY(randomEngine),distPosZ(randomEngine) };
		particle.velocity = { distVelocityX(randomEngine) ,distVelocityY(randomEngine) ,distVelocityZ(randomEngine) };
		return particle;
	}

	Particle::Particles Particle::MakeNewParticle(std::mt19937& randomEngine, float colorMin, float colorMax, float timeMin, float timeMax)
	{
		std::uniform_real_distribution<float> distColor(colorMin, colorMax);
		std::uniform_real_distribution<float> distTime(timeMin, timeMax);
		Particles particle;
		particle.transform.scale = { 1.0f,1.0f,1.0f };
		particle.transform.rotate = { 0.0f,0.0f,0.0f };
		particle.color = { distColor(randomEngine) ,distColor(randomEngine) ,distColor(randomEngine) ,1.0f };
		particle.lifeTime = distTime(randomEngine);
		particle.currentTime = 0;
		return particle;
	}

	void Particle::SetElements(float colorMin, float colorMax, float timeMin, float timeMax,
		float posxMin, float posxMax, float posyMin, float posyMax, float poszMin, float poszMax,
		float velxMin, float velxMax, float velyMin, float velyMax, float velzMin, float velzMax)
	{
		elements.colorMin = colorMin;
		elements.colorMax = colorMax;
		elements.timeMin = timeMin;
		elements.timeMax = timeMax;
		elements.posxMin = posxMin;
		elements.posxMax = posxMax;
		elements.posyMin = posyMin;
		elements.posyMax = posyMax;
		elements.poszMin = poszMin;
		elements.poszMax = poszMax;
		elements.velxMin = velxMin;
		elements.velxMax = velxMax;
		elements.velyMin = velyMin;
		elements.velyMax = velyMax;
		elements.velzMin = velzMin;
		elements.velzMax = velzMax;
	}

	void Particle::SetScale(Vector3 scale)
	{
		for (uint32_t index = 0; index < kNumMaxInstance; ++index)
		{
			particles[index].transform.scale = scale;
		}
	}

	void Particle::SetRotate(Vector3 rotate)
	{
		for (uint32_t index = 0; index < kNumMaxInstance; ++index)
		{
			particles[index].transform.rotate = rotate;
		}
	}
	void Particle::SetColorRed(float red)
	{
		for (uint32_t index = 0; index < kNumMaxInstance; ++index)
		{
			particles[index].color.x = red;
		}
	}
}
