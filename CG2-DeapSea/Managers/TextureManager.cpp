#include "TextureManager.h"

namespace MyEngine
{
	TextureManager* TextureManager::GetInstance()
	{
		if (instance == NULL)
		{
			instance = std::make_unique<TextureManager>();
		}
		return instance.get();
	}

	void TextureManager::Finalize()
	{
		textureDatas.clear();
		instance.reset();
	}

	void TextureManager::Initialize()
	{
		textureDatas.reserve(SRVManager::kMaxSRVCount);
		dx12Common_ = DX12Common::GetInstance();
		srvManager_ = SRVManager::GetInstance();
	}

	void TextureManager::LoadTexture(const std::string& filePath)
	{
		if (textureDatas.contains(filePath))
		{
			return;
		}

		assert(srvManager_->CheckNumTexture((uint32_t)textureDatas.size()));

		DirectX::ScratchImage image{};//テクスチャファイルをプログラムで扱えるように
		std::wstring filePathW = debug_->ConvertString(filePath);
		hr = DirectX::LoadFromWICFile(
			filePathW.c_str(),
			DirectX::WIC_FLAGS_FORCE_SRGB,
			nullptr,
			image);
		assert(SUCCEEDED(hr));

		DirectX::ScratchImage mipImages{};//MipMapの作成
		hr = DirectX::GenerateMipMaps(
			image.GetImages(),
			image.GetImageCount(),
			image.GetMetadata(),
			DirectX::TEX_FILTER_SRGB,
			0,
			mipImages);
		assert(SUCCEEDED(hr));

		TextureData& textureData = textureDatas[filePath];
		textureData.metadata = mipImages.GetMetadata();
		textureData.resource = CreateTextureResource(textureData.metadata);
		UploadTextureData(textureData.resource.Get(), mipImages, textureData.metadata);

		textureData.srvIndex = srvManager_->Allocate();
		textureData.srvHandleCPU = srvManager_->
			GetCPUDescriptorHandle(textureData.srvIndex);

		textureData.srvHandleGPU = srvManager_->
			GetGPUDescriptorHandle(textureData.srvIndex);

		srvManager_->CreateSRVforTexture2D(
			textureData.srvIndex,
			textureData.resource.Get(),
			textureData.metadata.format,
			(UINT)textureData.metadata.mipLevels);

		textureDatas[filePath] = textureData;
	}

	void TextureManager::EraseTexture(const std::string& filePath)
	{
		textureDatas.erase(filePath);
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
		hr = dx12Common_->GetDevice()->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&resource));

		assert(SUCCEEDED(hr));

		return resource;
	}

	uint32_t TextureManager::GetSrvIndex(const std::string& filePath)
	{
		TextureData& textureData = textureDatas[filePath];
		assert(srvManager_->CheckNumTexture(textureData.srvIndex));
		return textureData.srvIndex;
	}

	D3D12_GPU_DESCRIPTOR_HANDLE TextureManager::GetSRVHandleGPU(const std::string& filePath)
	{
		TextureData& textureData = textureDatas[filePath];
		assert(srvManager_->CheckNumTexture(textureData.srvIndex));
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
			hr = texture->WriteToSubresource(
				UINT(mipLevel),
				nullptr,
				img->pixels,
				UINT(img->rowPitch),
				UINT(img->slicePitch)
			);
			assert(SUCCEEDED(hr));
		}
	}

	const DirectX::TexMetadata& TextureManager::GetMetaData(const std::string& filePath)
	{
		TextureData& textureData = textureDatas[filePath];
		assert(srvManager_->CheckNumTexture(textureData.srvIndex));
		return textureData.metadata;
	}
}
