#include "Particle.h"
#include"Commons/ParticleCommon.h"

Particle::~Particle() {
}

void Particle::Initialize(const std::string& textureFilePath, ParticleCommon* particleCommon, SRVManager* srvManager, Object3dCommon* object3dCommon)
{
	this->particleCommon_ = particleCommon;
	this->object3dCommon_ = object3dCommon;
	this->srvManager = srvManager;
	this->camera_ = object3dCommon_->GetDefaultCamera();
	kNumMaxInstance = 10;
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());

	LeftTop = { -0.5f, 0.5f, 0.0f, 1.0f };
	RightTop = { 0.5f, 0.5f, 0.0f, 1.0f };
	RightBottom = { 0.5f, -0.5f, 0.0f, 1.0f };
	LeftBottom = { -0.5f, -0.5f, 0.0f, 1.0f };
	Color = { 1.0f, 1.0f, 1.0f, 1.0f };

	texcoordLeftTop = { 0.0f,0.0f };
	texcoordRightTop = { 0.0f,1.0f };
	texcoordRightBottom = { 1.0f,1.0f };
	texcoordLeftBottom = { 1.0f,0.0f };

	for (uint32_t index = 0; index < kNumMaxInstance; ++index)
	{
		particles[index] = MakeNewParticle(randomEngine);
	}

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

	InputData(true);

	for (uint32_t index = 0; index < kNumMaxInstance; ++index)
	{
		instancingData[index].WVP = MakeIdentity4x4();
		instancingData[index].World = MakeIdentity4x4();
		instancingData[index].color = particles[index].color;
	}
	MakeShaderResourceViewInstance();

	materialData.textureFilePath = textureFilePath;
	TextureManager::GetInstance()->LoadTexture(textureFilePath);
	materialData.textureIndex = TextureManager::GetInstance()->GetSrvIndex(textureFilePath);
}


void Particle::Update(bool isRevive)
{
	InputData(isRevive);
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
	srvManager->SetGraphicsRootDescriptorTable(
		2, materialData.textureIndex);
	particleCommon_->GetDx12Common()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(
		3, cameraResource->GetGPUVirtualAddress());

	D3D12_CPU_DESCRIPTOR_HANDLE rtv =
		particleCommon_->GetDx12Common()->GetRtvHandles(srvManager->GetBackBufferIndex());
	D3D12_CPU_DESCRIPTOR_HANDLE dsv = particleCommon_->GetDx12Common()->GetDsvHandle();
	particleCommon_->GetDx12Common()->GetCommandList().Get()->OMSetRenderTargets(1, &rtv, false, &dsv);

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

void Particle::InputData(bool isRevive)
{
	uint32_t* indexData = nullptr;
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));

	colorData[0].color = { 1.0f,1.0f,1.0f,1.0f };

	kNumInstance = 0;
	for (uint32_t index = 0; index <kNumMaxInstance; ++index)
	{
		float alpha = 1.0f - (particles[index].currentTime / particles[index].lifeTime);
		Matrix4x4 worldMatrix =
			MakeAffineMatrix(
				particles[index].transform.scale,
				particles[index].transform.rotate,
				particles[index].transform.translate);
		if (particles[index].lifeTime <= particles[index].currentTime && isRevive == true)
		{
			std::random_device seedGenerator;
			std::mt19937 randomEngine(seedGenerator());
			particles[index] = MakeNewParticle(randomEngine);
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
	uint32_t index = srvManager->Allocate();
	instancingSrvHandleCPU = srvManager->GetCPUDescriptorHandle(index);
	instancingSrvHandleGPU = srvManager->GetGPUDescriptorHandle(index);
	srvManager->CreateSRVforStructuredBuffer(index, instancingResource.Get(), kNumInstance, sizeof(ParticleForGPU));
}

Particle::Particles Particle::MakeNewParticle(std::mt19937& randomEngine)
{
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
	std::uniform_real_distribution<float> distTime(1.0f, 3.0f);
	Particles particle;
	particle.transform.scale = { 1.0f,1.0f,1.0f };
	particle.transform.rotate = { 0.0f,0.0f,0.0f };
	particle.transform.translate = { distribution(randomEngine),distribution(randomEngine),distribution(randomEngine) };
	particle.velocity = { distribution(randomEngine) ,distribution(randomEngine) ,distribution(randomEngine) };
	particle.color = { distColor(randomEngine) ,distColor(randomEngine) ,distColor(randomEngine) ,1.0f };
	particle.lifeTime = distTime(randomEngine);
	particle.currentTime = 0;
	return particle;
}