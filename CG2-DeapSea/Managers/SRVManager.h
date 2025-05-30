#pragma once
#include"DX12Common.h"

namespace MyEngine
{
	class SRVManager
	{
	public:
		static const uint32_t kMaxSRVCount;
		static const uint32_t kSRVIndexTop;
		void Initialize();
		uint32_t Allocate();
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
		void SetGraphicsRootDescriptorTable(UINT RootParamaterIndex, uint32_t srvIndex);
		void MakeFence();
		uint64_t fenceValue = 0;
		bool CheckNumTexture(uint32_t textureIndex);
		void PreDraw();
		void PostDraw();
		void Finalize();
		UINT GetBackBufferIndex() { return backBufferIndex; }
		ComPtr<ID3D12DescriptorHeap> GetSrvDescriptorHeap() { return descriptorHeap; }
		HANDLE GetFenceEvent() { return fenceEvent; }
		static SRVManager* GetInstance();

		SRVManager() = default;
		~SRVManager() = default;
	private:
		SRVManager(SRVManager&) = delete;
		SRVManager& operator=(SRVManager&) = delete;

		DX12Common* dx12Common_ = nullptr;

		static inline std::unique_ptr<SRVManager> instance;

		uint32_t descriptorSize;
		ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;
		uint32_t useIndex = 0;
		UINT backBufferIndex;
		D3D12_RESOURCE_BARRIER barrier{};
		float clearColor[4] = { 0.1f, 0.25f, 0.5f, 1.0f };
		D3D12_VIEWPORT viewport{};
		D3D12_RECT scissorRect{};
		ComPtr<ID3D12Fence> fence = nullptr;
		HANDLE fenceEvent;
		HRESULT hr = NULL;


	};
}