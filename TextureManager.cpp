#include "TextureManager.h"
TextureManager* TextureManager::instance = nullptr;
uint32_t TextureManager::kSRVIndexTop = 1;

TextureManager* TextureManager::GetInstance()
{
	if (instance == nullptr)
	{
		instance = new TextureManager;
	}
	return instance;
}

void TextureManager::Finalize()
{
	delete instance;
	instance = nullptr;
}

void TextureManager::Initialize()
{
	textureDatas.reserve(DX12Common::kMaxSRVCount);
}

void TextureManager::LoadTexture(const std::string& filePath)
{
	auto it = std::find_if(
		textureDatas.begin(),
		textureDatas.end(),
		[&](TextureData& textureData) {return textureData.filePath == filePath; });
	if (it != textureDatas.end())
	{
		return;
	}
	assert(textureDatas.size() + kSRVIndexTop < DX12Common::kMaxSRVCount);

	DirectX::ScratchImage image{};
	std::wstring filePathW = debug_->ConvertString("Resource/" + filePath);
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
	assert(SUCCEEDED(hr));

	textureDatas.resize(textureDatas.size() + 1);
	TextureData& textureData = textureDatas.back();

	textureData.filePath = filePath;
	textureData.metadata = mipImages.GetMetadata();
	textureData.resource = CreateTextureResource(textureData.metadata);
	//UploadTextureData(textureData.resource.Get(), mipImages, textureData.metadata);

	uint32_t srvIndex = static_cast<uint32_t>(textureDatas.size() - 1) + kSRVIndexTop;

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = textureData.metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(textureData.metadata.mipLevels);
	
	textureData.srvHandleCPU = DX12Common::GetInstance()->
		GetCPUDescriptorHandle(DX12Common::GetInstance()->
			GetSrvDescriptorHeap().Get(), srvIndex, 1);
	textureData.srvHandleGPU = DX12Common::GetInstance()->
		GetGPUDescriptorHandle(DX12Common::GetInstance()->
			GetSrvDescriptorHeap().Get(), srvIndex, 1);
	textureData.srvHandleCPU.ptr = DX12Common::GetInstance()->GetDevice().Get()->
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	textureData.srvHandleGPU.ptr = DX12Common::GetInstance()->GetDevice().Get()->
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	DX12Common::GetInstance()->GetDevice().Get()->
		CreateShaderResourceView(
			textureData.resource.Get(),
			&srvDesc,
			textureData.srvHandleCPU);
}

ComPtr<ID3D12Resource> TextureManager::CreateTextureResource(const DirectX::TexMetadata& metadata)
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
	HRESULT hr = DX12Common::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&resource));

	assert(SUCCEEDED(hr));

	return resource;
}

uint32_t TextureManager::GetTextureIndexByFilePath(const std::string& filePath)
{
	auto it = std::find_if(
		textureDatas.begin(),
		textureDatas.end(),
		[&](TextureData& textureData) {return textureData.filePath == filePath; });
	if (it != textureDatas.end())
	{
		uint32_t textureIndex = static_cast<uint32_t>(std::distance(textureDatas.begin(), it));
		return textureIndex;
	}
	assert(0);
	return 0;
}

D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSRVHandleGPU(uint32_t textureIndex)
{
	assert(textureIndex);
	TextureData& textureData = textureDatas.back();
	return textureData.srvHandleGPU;
}

void TextureManager::UploadTextureData(
	ID3D12Resource* texture,
	const DirectX::ScratchImage& mipImages,
	const DirectX::TexMetadata& metadata)
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


