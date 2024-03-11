#pragma once
#include<string>
#include "externals/DirectXTex/DirectXTex.h"
#include<WRL.h>
#include "Commons/DX12Common.h"
#include"SRVManager.h"
#include<unordered_map>

class TextureManager
{
public:
	struct TextureData
	{
		DirectX::TexMetadata metadata;
		ComPtr<ID3D12Resource> resource;
		uint32_t srvIndex;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};

	static TextureManager* GetInstance();
	void Finalize();
	void Initialize(SRVManager* srvManager_);
	void LoadTexture(DX12Common* dxcommon,const std::string& filePath);
	ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);
	uint32_t GetTextureIndexByFilePath(const std::string& filePath);
	uint32_t GetSrvIndex(const std::string& filePath);
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandleGPU(const std::string& filePath);
	void UploadTextureData(
		ID3D12Resource* texture,
		const DirectX::ScratchImage& mipImages,
		const DirectX::TexMetadata& metadata);
	const DirectX::TexMetadata& GetMetaData(const std::string& filePath);

private:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	static TextureManager* instance;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = delete;
	TextureManager& operator=(TextureManager&) = delete;
	DX12Common* dx12Common_;
	Debug* debug_;
	SRVManager* srvManager = nullptr;
	static uint32_t kSRVIndexTop;
	std::unordered_map<std::string, TextureData>textureDatas;

};

