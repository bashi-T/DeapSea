#pragma once
#include<string>
#include<DirectXTex.h>
#include<WRL.h>
#include "DX12Common.h"

class TextureManager
{
private:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	static TextureManager* instance;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = delete;
	TextureManager& operator=(TextureManager&) = delete;
	struct TextureData
	{
		std::string filePath;
		DirectX::TexMetadata metadata;
		ComPtr<ID3D12Resource> resource;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

	std::vector<TextureData> textureDatas;
	DX12Common* dx12Common_;
	Debug* debug_;
	static uint32_t kSRVIndexTop;

public:
	static TextureManager* GetInstance();
	void Finalize();
	void Initialize();
	void LoadTexture(DX12Common* dxcommon,const std::string& filePath);
	ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);
	uint32_t GetTextureIndexByFilePath(const std::string& filePath);
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandleGPU(uint32_t textureIndex);
	void UploadTextureData(
		ID3D12Resource* texture,
		const DirectX::ScratchImage& mipImages,
		const DirectX::TexMetadata& metadata);
	const DirectX::TexMetadata& GetMetaData(uint32_t textureIndex);

};

