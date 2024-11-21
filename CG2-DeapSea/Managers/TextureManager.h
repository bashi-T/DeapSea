#pragma once
#include<string>
#include "externals/DirectXTex/DirectXTex.h"
#include<WRL.h>
#include "Commons/DX12Common.h"
#include"SRVManager.h"
#include<unordered_map>


/// <summary>
/// textureの基礎設定
/// </summary>
class TextureManager
{
public:
	struct TextureData
	{
		DirectX::TexMetadata metadata;
		ComPtr<ID3D12Resource> resource;
		uint32_t srvIndex = 0;
		D3D12_CPU_DESCRIPTOR_HANDLE srvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE srvHandleGPU;
	};
	/// <summary>
	/// instance取得
	/// </summary>
	/// <returns></returns>
	static TextureManager* GetInstance();
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="dxcommon"></param>
	/// <param name="srvManager_"></param>
	void Initialize(DX12Common* dxcommon, SRVManager* srvManager_);
	/// <summary>
	/// instance解放
	/// </summary>
	void Finalize();
	/// <summary>
	/// textureの読み込み
	/// </summary>
	/// <param name="filePath"></param>
	void LoadTexture(const std::string& filePath);
	/// <summary>
	/// 
	/// </summary>
	/// <param name="filePath"></param>
	//void LoadTextureforSRV(const std::string& filePath);

	/// <summary>
	/// textureの消去
	/// </summary>
	/// <param name="filePath"></param>
	void EraseTexture(std::string& filePath);
	/// <summary>
	/// textureData.resourceの設定
	/// </summary>
	/// <param name="metadata"></param>
	/// <returns></returns>
	ComPtr<ID3D12Resource> CreateTextureResource(const DirectX::TexMetadata& metadata);
	/// <summary>
	/// metaDataの取得
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	const DirectX::TexMetadata& GetMetaData(const std::string& filePath);
	/// <summary>
	/// textureを収納して収納位置を返す
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	uint32_t GetSrvIndex(const std::string& filePath);
	/// <summary>
	/// GPU_DESCRIPTOR_HANDLEを取得
	/// </summary>
	/// <param name="filePath"></param>
	/// <returns></returns>
	D3D12_GPU_DESCRIPTOR_HANDLE GetSRVHandleGPU(const std::string& filePath);
	std::unordered_map<std::string, TextureData>GetTextureData() { return textureDatas; }
	/// <summary>
	/// textureData転送
	/// </summary>
	/// <param name="texture"></param>
	/// <param name="mipImages"></param>
	/// <param name="metadata"></param>
	void UploadTextureData(
		ID3D12Resource* texture,
		const DirectX::ScratchImage& mipImages,
		const DirectX::TexMetadata& metadata);

private:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	static TextureManager* instance;

	TextureManager() = default;
	~TextureManager() = default;
	TextureManager(TextureManager&) = delete;
	TextureManager& operator=(TextureManager&) = delete;
	DX12Common* dx12Common_ = nullptr;
	Debug* debug_ = nullptr;
	SRVManager* srvManager = nullptr;
	std::unordered_map<std::string, TextureData>textureDatas;
	HRESULT hr;
};

