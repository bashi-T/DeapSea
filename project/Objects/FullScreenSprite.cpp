#include "FullScreenSprite.h"
#include"Commons/FullScreenSpriteCommon.h"

FullScreenSprite::~FullScreenSprite()
{
}

void FullScreenSprite::MeshInitialize(FullScreenSpriteCommon* spriteCommon, SRVManager* srvManager)
{
	this->spriteCommon_ = spriteCommon;
	this->srvManager = srvManager;
	vertexResource = CreateBufferResource(spriteCommon_, sizeof(VertexData) * 3);
	materialResource = CreateBufferResource(spriteCommon_, sizeof(Material));
	transformationMatrixResource = CreateBufferResource(spriteCommon_, sizeof(TransformationMatrix));

	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	transformationMatrixResource->Map(
		0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));
	materialData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
}

void FullScreenSprite::SpriteInitialize(FullScreenSpriteCommon* spriteCommon, SRVManager* srvManager, std::string texturefilePath)
{
	this->spriteCommon_ = spriteCommon;
	this->srvManager = srvManager;
	vertexResource = CreateBufferResource(spriteCommon_, sizeof(VertexData) * 3);
	indexResource = CreateBufferResource(spriteCommon_, sizeof(uint32_t) * 3);
	materialResource = CreateBufferResource(spriteCommon_, sizeof(Material));
	transformationMatrixResource = CreateBufferResource(spriteCommon_, sizeof(TransformationMatrix));

	//uvTransform =
	//{
	//	{1.0f,1.0f,1.0f},
	//	{0.0f,0.0f,0.0f},
	//	{0.0f,0.0f,0.0f},
	//};
	//transformMatrix =
	//{
	//	{1.0f,1.0f,1.0f},
	//	{0.0f,0.0f,0.0f},
	//	{0.0f,0.0f,0.0f},
	//};
	//cameraTransform = {
	//	{1.0f, 1.0f, 1.0f},
	//	{0.0f, 0.0f, 0.0f},
	//	{0.0f, 0.0f, -15.0f}
	//};
	//cameraMatrix =
	//	MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	//MakeBufferView();
	//vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	//indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	//materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	//transformationMatrixResource->Map(
	//	0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));
	//LeftTop = { 0.0f, 0.0f, 0.0f, 1.0f };
	//RightTop = { float(WinAPP::clientWidth_) / 3, 0.0f, 0.0f, 1.0f };
	//RightBottom = { float(WinAPP::clientWidth_) / 3, float(WinAPP::clientHeight_) / 3, 0.0f, 1.0f };
	//LeftBottom = { 0.0f, float(WinAPP::clientHeight_) / 3, 0.0f, 1.0f };
	//coordLeftTop = { 0.0f, 0.0f };
	//coordRightTop = { 1.0f, 0.0f };
	//coordRightBottom = { 1.0f, 1.0f };
	//coordLeftBottom = { 0.0f, 1.0f };

	Color = { 1.0f, 1.0f, 1.0f, 1.0f };
	InputData(Color);
	materialData->material.textureFilePath = texturefilePath;
	TextureManager::GetInstance()->LoadTexture(materialData->material.textureFilePath);
	materialData->material.textureIndex = TextureManager::GetInstance()->GetSrvIndex(materialData->material.textureFilePath);
}

void FullScreenSprite::Update()
{
	cameraMatrix =
		MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	projectionMatrix =
		MakeOrthographicMatrix(0.0f, 0.0f, float(WinAPP::clientWidth_), float(WinAPP::clientHeight_), 0.0f, 100.0f);
	transformMatrix.translate = { position.x,position.y,0.0f };
	transformMatrix.rotate = { 0.0f,0.0f,rotation };
	transformMatrix.scale = { size.x,size.y,1.0f };
	InputData(Color);
}

void FullScreenSprite::MeshDraw(FullScreenSpriteCommon* spriteCommon)
{
	this->spriteCommon_ = spriteCommon;
	//spriteCommon_->GetDx12Common()->GetCommandList().Get()->
	//	SetPipelineState(spriteCommon_->GetGraphicsPipelineState().Get());
	//spriteCommon_->GetDx12Common()->GetCommandList().Get()->
	//	SetGraphicsRootSignature(spriteCommon_->GetRootSignature().Get());
	//spriteCommon_->GetDx12Common()->GetCommandList().Get()->
	//	IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	//
	//spriteCommon_->GetDx12Common()->GetCommandList().Get()->
	//	IASetVertexBuffers(0, 1, &vertexBufferView);
	//spriteCommon_->GetDx12Common()->GetCommandList().Get()->
	//	IASetIndexBuffer(&indexBufferView);
	//spriteCommon_->GetDx12Common()->GetCommandList().Get()->
	//	SetGraphicsRootConstantBufferView(
	//		0, materialResource->GetGPUVirtualAddress());
	//spriteCommon_->GetDx12Common()->GetCommandList().Get()->
	//	SetGraphicsRootConstantBufferView(
	//		1, transformationMatrixResource->GetGPUVirtualAddress());
	//
	//
	//spriteCommon_->GetDx12Common()->GetCommandList().Get()->
	//	DrawInstanced(3, 1, 0, 0);
	spriteCommon_->GetDx12Common()->GetCommandList().Get()->
		SetGraphicsRootSignature(spriteCommon_->GetRootSignature().Get());
	spriteCommon_->GetDx12Common()->GetCommandList().Get()->
		SetPipelineState(spriteCommon_->GetGraphicsPipelineState().Get());
	spriteCommon_->GetDx12Common()->GetCommandList().Get()->
		IASetVertexBuffers(0, 1, &vertexBufferView);
	//spriteCommon_->GetDx12Common()->GetCommandList().Get()->
	//	IASetIndexBuffer(&indexBufferView);
	spriteCommon_->GetDx12Common()->GetCommandList().Get()->
		SetGraphicsRootDescriptorTable(2, spriteCommon_->GetDx12Common()->GetSrvHandleGPU());
	spriteCommon_->GetDx12Common()->GetCommandList().Get()->
		IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	spriteCommon_->GetDx12Common()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(
		0, materialResource->GetGPUVirtualAddress());
	spriteCommon_->GetDx12Common()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(
		1, transformationMatrixResource->GetGPUVirtualAddress());

	spriteCommon_->GetDx12Common()->GetCommandList().Get()->
		DrawInstanced(3, 1, 0, 0);
}

void FullScreenSprite::SpriteDraw(FullScreenSpriteCommon* spriteCommon)
{
	this->spriteCommon_ = spriteCommon;
	spriteCommon_->GetDx12Common()->GetCommandList().Get()->
		SetPipelineState(spriteCommon_->GetGraphicsPipelineState().Get());
	spriteCommon_->GetDx12Common()->GetCommandList().Get()->
		SetGraphicsRootSignature(spriteCommon_->GetRootSignature().Get());
	spriteCommon_->GetDx12Common()->GetCommandList().Get()->
		IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	spriteCommon_->GetDx12Common()->GetCommandList().Get()->
		IASetVertexBuffers(0, 1, &vertexBufferView);
	spriteCommon_->GetDx12Common()->GetCommandList().Get()->
		IASetIndexBuffer(&indexBufferView);
	spriteCommon_->GetDx12Common()->GetCommandList().Get()->
		SetGraphicsRootConstantBufferView(
			0, materialResource->GetGPUVirtualAddress());
	spriteCommon_->GetDx12Common()->GetCommandList().Get()->
		SetGraphicsRootConstantBufferView(
			1, transformationMatrixResource->GetGPUVirtualAddress());

	srvManager->SetGraphicsRootDescriptorTable(
		2, materialData->material.textureIndex);

	spriteCommon_->GetDx12Common()->GetCommandList().Get()->
		DrawInstanced(3, 1, 0, 0);
}

ComPtr<ID3D12Resource> FullScreenSprite::CreateBufferResource(FullScreenSpriteCommon* spriteCommon, size_t sizeInBytes)
{
	this->spriteCommon_ = spriteCommon;
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

	hr = spriteCommon_->GetDx12Common()->GetDevice().Get()->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&Resource));
	assert(SUCCEEDED(hr));
	return Resource;
}

void FullScreenSprite::MakeBufferView()
{
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 3;
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 3;
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
}

void FullScreenSprite::InputData(Vector4 color)
{
	//	Material* materialData = nullptr;
	//	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	//	materialData[0].color = color;
	//	materialData[0].enableLighting = true;
}

void FullScreenSprite::InputDataTexture(Vector4 color)
{
	float left = 0.0f - anchorPoint.x;
	float right = 1.0f - anchorPoint.x;
	float top = 0.0f - anchorPoint.y;
	float bottom = 1.0f - anchorPoint.y;

	if (isFlipX_)
	{
		left = -left;
		right = -right;
	}
	if (isFlipY_)
	{
		top = -top;
		bottom = -bottom;
	}

	const DirectX::TexMetadata& metadata =
		TextureManager::GetInstance()->GetMetaData(materialData->material.textureFilePath);
	float tex_left = textureLeftTop.x / metadata.width;
	float tex_right = (textureLeftTop.x + textureSize.x) / metadata.width;
	float tex_top = textureLeftTop.y / metadata.width;
	float tex_bottom = (textureLeftTop.y + textureSize.y) / metadata.width;

	vertexData[0].position = { left,top,0.0f,1.0f };
	vertexData[1].position = { right,top,0.0f,1.0f };
	vertexData[2].position = { right,bottom,0.0f,1.0f };
	vertexData[3].position = { left,bottom,0.0f,1.0f };
	vertexData[0].texcoord = { tex_left,tex_top };
	vertexData[1].texcoord = { tex_right,tex_top };
	vertexData[2].texcoord = { tex_right,tex_bottom };
	vertexData[3].texcoord = { tex_left,tex_bottom };

	vertexData[0].normal = { 0.0f, 0.0f, -1.0f };
	vertexData[1].normal = { 0.0f, 0.0f, -1.0f };
	vertexData[2].normal = { 0.0f, 0.0f, -1.0f };
	vertexData[3].normal = { 0.0f, 0.0f, -1.0f };

	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;

	indexData[3] = 0;
	indexData[4] = 2;
	indexData[5] = 3;

	materialData[0].color = color;
	materialData[0].enableLighting = false;
	materialData[0].uvTransform = MakeIdentity4x4();
	Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTransform.scale);
	uvTransformMatrix = Multiply(uvTransformMatrix, MakerotateZMatrix(uvTransform.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransform.translate));
	materialData[0].uvTransform = uvTransformMatrix;

	Matrix4x4 worldMatrix = MakeAffineMatrix(
		transformMatrix.scale, transformMatrix.rotate, transformMatrix.translate);
	viewMatrix = MakeIdentity4x4();
	worldViewProjectionMatrix =
		Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	transformationMatrixData->WVP = worldViewProjectionMatrix;
	transformationMatrixData->World = worldMatrix;
}
