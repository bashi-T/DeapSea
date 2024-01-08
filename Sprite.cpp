#include "Sprite.h"
#include"SpriteCommon.h"


Sprite::~Sprite()
{
}

void Sprite::Initialize(int32_t width, int32_t height, SpriteCommon* spriteCommon)
{
	this->spriteCommon_ = spriteCommon;
	vertexResource = spriteCommon_->CreateBufferResource(sizeof(VertexData) * 6, spriteCommon_->GetDx12Common());
	indexResource = spriteCommon_->CreateBufferResource(sizeof(uint32_t) * 6, spriteCommon_->GetDx12Common());
	materialResource = spriteCommon_->CreateBufferResource(sizeof(Material), spriteCommon_->GetDx12Common());
	transformationMatrixResource = spriteCommon_->CreateBufferResource(sizeof(TransformationMatrix), spriteCommon_->GetDx12Common());

	MakeBufferView();
	
	LeftTop[0] = { 0.0f, 0.0f, 0.0f, 1.0f };
	RightTop[0] = { float(width) / 3, 0.0f, 0.0f, 1.0f };
	RightBottom[0] = { float(width) / 3, float(height) / 3, 0.0f, 1.0f };
	LeftBottom[0] = { 0.0f, float(height) / 3, 0.0f, 1.0f };
	Color[0] = { 1.0f, 1.0f, 1.0f, 1.0f };
	coordLeftTop[0] = { 0.0f, 0.0f };
	coordRightTop[0] = { 1.0f, 0.0f };
	coordRightBottom[0] = { 1.0f, 1.0f };
	coordLeftBottom[0] = { 0.0f, 1.0f };
	
	InputData(
		LeftTop[0], RightTop[0], RightBottom[0], LeftBottom[0], Color[0], coordLeftTop[0], coordRightTop[0],
		coordRightBottom[0], coordLeftBottom[0], width, height);

}

void Sprite::Update(int32_t width, int32_t height)
{
	cameraMatrix =
		MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	viewMatrix = Inverse(cameraMatrix);
	ViewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
	projectionMatrix =
		MakeOrthographicMatrix(0.0f, 0.0f, float(width), float(height), 0.0f, 100.0f);

	Matrix4x4 worldMatrix = MakeAffineMatrix(
		transformMatrix.scale, transformMatrix.rotate, transformMatrix.translate);
	viewMatrix = MakeIdentity4x4();
	worldViewProjectionMatrix =
		Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	transformationMatrixData->WVP = worldViewProjectionMatrix;
	transformationMatrixData->World = worldMatrix;

}

void Sprite::MakeBufferView()
{
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 6;
	vertexBufferView.StrideInBytes = sizeof(VertexData);

	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
}

void Sprite::InputData(
	Vector4 LeftTop, Vector4 RightTop, Vector4 RightBottom, Vector4 LeftBottom, Vector4 color,
	Vector2 coordLeftTop, Vector2 coordRightTop, Vector2 coordRightBottom, Vector2 coordLeftBottom,
	int32_t width, int32_t height)
{
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	transformationMatrixResource->Map(
		0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));

	materialData[0].color = color;
	materialData[0].enableLighting = false;
	materialData[0].uvTransform = MakeIdentity4x4();
	Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTransform.scale);
	uvTransformMatrix = Multiply(uvTransformMatrix, MakerotateZMatrix(uvTransform.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransform.translate));
	materialData[0].uvTransform = uvTransformMatrix;

	vertexData[0].position = LeftTop;
	vertexData[1].position = RightTop;
	vertexData[2].position = RightBottom;
	vertexData[3].position = LeftBottom;
	vertexData[0].texcoord = coordLeftTop;
	vertexData[1].texcoord = coordRightTop;
	vertexData[2].texcoord = coordRightBottom;
	vertexData[3].texcoord = coordLeftBottom;

	vertexData[0].normal = { 0.0f, 0.0f, -1.0f };

	indexData[0] = 0;
	indexData[1] = 1;
	indexData[2] = 2;

	indexData[3] = 0;
	indexData[4] = 2;
	indexData[5] = 3;

	transformationMatrixData->WVP = MakeIdentity4x4();
	transformationMatrixData->World = MakeIdentity4x4();
}

void Sprite::Draw(SpriteCommon* spriteCommon)
{
	this->spriteCommon_ = spriteCommon;
	spriteCommon_->GetDx12Common()->GetCommandList()->SetPipelineState(spriteCommon_->GetGraphicsPipelineState().Get());
	spriteCommon_->GetDx12Common()->GetCommandList()->SetGraphicsRootSignature(rootSignature.Get());
	spriteCommon_->GetDx12Common()->GetCommandList()->IASetPrimitiveTopology(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	spriteCommon_->GetDx12Common()->GetCommandList()->
		IASetVertexBuffers(0, 1, &vertexBufferView);
	spriteCommon_->GetDx12Common()->GetCommandList()->
		IASetIndexBuffer(&indexBufferView);
	spriteCommon_->GetDx12Common()->GetCommandList()->SetGraphicsRootConstantBufferView(
		0, materialResource->GetGPUVirtualAddress());
	spriteCommon_->GetDx12Common()->GetCommandList()->SetGraphicsRootConstantBufferView(
		1, transformationMatrixResource->GetGPUVirtualAddress());

	//D3D12_CPU_DESCRIPTOR_HANDLE rtv =
	//	spriteCommon_->GetDx12Common()->GetRtvHandles(spriteCommon_->GetDx12Common()->GetBackBufferIndex());
	//D3D12_CPU_DESCRIPTOR_HANDLE dsv = spriteCommon_->GetDx12Common()->GetDsvHandle();
	//spriteCommon_->GetDx12Common()->GetCommandList()->OMSetRenderTargets(1, &rtv, false, &dsv);
	spriteCommon_->GetDx12Common()->GetCommandList()->SetGraphicsRootDescriptorTable(
		2, GetTextureSrvHandleGPU());

	spriteCommon_->GetDx12Common()->GetCommandList()->
		DrawIndexedInstanced(6, 1, 0, 0, 0);
}

ComPtr<ID3D12Resource> Sprite::CreateBufferResource(size_t sizeInBytes)
{
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

	hr = spriteCommon_->GetDx12Common()->GetDevice()->CreateCommittedResource(
		&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&Resource));
	assert(SUCCEEDED(hr));
	return Resource;
}

DirectX::ScratchImage Sprite::LoadTexture(const std::string& filePath)
{
	DirectX::ScratchImage image{};
	std::wstring filePathW = debug_->ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(
		filePathW.c_str(),
		DirectX::WIC_FLAGS_FORCE_SRGB,
		nullptr,
		image);
	assert(SUCCEEDED(hr));

	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(
		image.GetImages(),
		image.GetImageCount(),
		image.GetMetadata(),
		DirectX::TEX_FILTER_SRGB,
		0,
		mipImages);

	return mipImages;
}

ComPtr<ID3D12Resource> Sprite::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata)
{
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);
	resourceDesc.Height = UINT(metadata.height);
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);
	resourceDesc.Format = metadata.format;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);

	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&resource));

	assert(SUCCEEDED(hr));

	return resource;
}

void Sprite::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages, const DirectX::TexMetadata& metadata)
{
	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; mipLevel++)
	{
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
		HRESULT hr = texture->WriteToSubresource(
			UINT(mipLevel),
			nullptr,
			img->pixels,
			UINT(img->rowPitch),
			UINT(img->slicePitch)
		);
		assert(SUCCEEDED(hr));
	}
}


ComPtr<IDxcBlob> Sprite::CompileShader(
	const std::wstring& filePath,
	const wchar_t* profile,
	IDxcUtils* dxcUtils_,
	IDxcCompiler3* dxcCompiler_,
	IDxcIncludeHandler* includeHandler_)
{
	debug_->Log(
		debug_->ConvertString(std::format(L"Begin CompileShader,path{},\n", filePath, profile)));
	ComPtr<IDxcBlobEncoding> shaderSource = nullptr;
	hr = dxcUtils_->LoadFile(filePath.c_str(), nullptr, shaderSource.GetAddressOf());
	assert(SUCCEEDED(hr));

	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	LPCWSTR arguments[]{
		filePath.c_str(), L"-E", L"main", L"-T", profile, L"-Zi", L"-Qembed_debug", L"-Od", L"-Zpr",
	};
	ComPtr<IDxcResult> shaderResult = nullptr;
	hr = dxcCompiler_->Compile(
		&shaderSourceBuffer, arguments, _countof(arguments), includeHandler_,
		IID_PPV_ARGS(&shaderResult));
	assert(SUCCEEDED(hr));

	ComPtr<IDxcBlobUtf8> shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		debug_->Log(shaderError->GetStringPointer());
		assert(SUCCEEDED(hr));
	}
	ComPtr<IDxcBlob> shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	debug_->Log(
		debug_->ConvertString(std::format(L"Compile Succeded,path:{}\n", filePath, profile)));
	shaderSource->Release();
	shaderResult->Release();
	return shaderBlob;
}
