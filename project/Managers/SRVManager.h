#pragma once
#include"DX12Common.h"

class SRVManager
{
private:
	DX12Common* dxCommon_ = nullptr;
	uint32_t descriptorSize;
	ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;
	uint32_t useIndex = 0;
	static inline SRVManager* instance;
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc;
public:
	static const uint32_t kMaxSRVCount;
	static const uint32_t kSRVIndexTop;
	void Initialize(DX12Common* dxCommon/*, uint32_t srvIndex*/);
	//uint32_t Allocate();
	D3D12_CPU_DESCRIPTOR_HANDLE GetCPUDescriptorHandle(uint32_t index);
	D3D12_GPU_DESCRIPTOR_HANDLE GetGPUDescriptorHandle(uint32_t index);
	void CreateSRVforTexture2D(
		uint32_t srvIndex,
		ID3D12Resource* pResource,
		DXGI_FORMAT Format,
		UINT MipLevels);
	void CreateSRVforStructuredBuffer(
		uint32_t srvIndex,
		ID3D12Resource* pResource,
		UINT numElements,
		UINT structureByteStride);
	//void CreateSRVforRenderTexture(
	//	uint32_t srvIndex,
	//	ID3D12Resource* pResource,
	//	UINT numElements,
	//	UINT structureByteStride);
	void SetGraphicsRootDescriptorTable(UINT RootParamaterIndex, uint32_t srvIndex);
	bool CheckNumTexture(uint32_t textureIndex);
	uint32_t GetDescriptorSize(){ return descriptorSize; }

	ComPtr<ID3D12DescriptorHeap> GetSrvDescriptorHeap() { return descriptorHeap; }
	static SRVManager* GetInstance();

};

