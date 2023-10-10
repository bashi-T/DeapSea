#pragma once
#include"WindowApp.h"
#include "list"
#include<cassert>
#include"Debug.h"
#include<d3d12.h>
#include<dxgi1_6.h>
#include<DirectXTex.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class DX12Common final
{
public:
	void Init(const std::string& filePath, int32_t width, int32_t height);
	void MakeDXGIFactory();
	void ChoseUseAdapter();
	void MakeD3D12Device();

	void MakeCommandQueue();
	void MakeCommandList();

	void MakeSwapchain(int32_t width, int32_t height, HWND hwnd_);
	void MakeDescriptorHeap();
	void BringResources();
	void MakeRTV();
	void MakeDSV();

	void MakeScreen();
	void DrawScreen();
	void ClearScreen();
	void MakeFence();
	void DX12Release(ID3D12Debug1* debugController);
	ID3D12DescriptorHeap* CreateDescriptorHeap(
	    ID3D12Device* device,
		D3D12_DESCRIPTOR_HEAP_TYPE heapType,
		UINT numDesctiptors,
	    bool shaderVisible);
	DirectX::ScratchImage LoadTexture(const std::string& filePath);
	ID3D12Resource* CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata);
	void UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages, const DirectX::TexMetadata& metadata);

	void MakeShaderResourceView(const DirectX::TexMetadata& metadata);
	ID3D12Resource* CreatedepthstencilTextureResource(ID3D12Device* device, int32_t width, int32_t height);


	ID3D12Device* GetDevice() { return device; }
	ID3D12GraphicsCommandList* GetCommandList() { return commandList; }
	UINT GetBackBufferIndex() { return backBufferIndex; }
	HANDLE GetFenceEvent() { return fenceEvent; }
	uint64_t fenceValue = 0;
	static DX12Common* GetInstance();
	D3D12_CPU_DESCRIPTOR_HANDLE GetRtvHandles(int32_t i) { return rtvHandles[i]; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetDsvHandle() { return dsvHandle; }
	ID3D12DescriptorHeap* GetRtvDescriptorHeap() { return rtvDescriptorHeap; }
	ID3D12DescriptorHeap* GetSrvDescriptorHeap() { return srvDescriptorHeap; }
	DXGI_SWAP_CHAIN_DESC1 GetSwapChainDesc() { return swapChainDesc; }
	D3D12_RENDER_TARGET_VIEW_DESC GetRtvDesc() { return rtvDesc; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetTextureSrvHandleCPU() {return textureSrvHandleCPU; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureSrvHandleGPU() {return textureSrvHandleGPU; }
	ID3D12DescriptorHeap* GetDsvDescriptorHeap() { return dsvDescriptorHeap; }


		

private:
	DX12Common()=default;
	~DX12Common()=default;
	DX12Common(const DX12Common& obj) = delete;
	DX12Common& oparator(const DX12Common&obj) = delete;

	Debug* debug_ = nullptr;
	ID3D12Device* device = nullptr;
	IDXGIFactory7* dxgiFactory = nullptr;
	HRESULT hr = NULL;
	IDXGIAdapter4* useAdapter = nullptr;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
	D3D12_CPU_DESCRIPTOR_HANDLE dsvHandle;
	float clearColor[4] = { 0.1f, 0.25f, 0.5f, 1.0f };

	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;
	IDXGISwapChain4* swapChain = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	ID3D12DescriptorHeap* rtvDescriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	ID3D12DescriptorHeap* srvDescriptorHeap = nullptr;
	D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc{};
	ID3D12DescriptorHeap* dsvDescriptorHeap;

	ID3D12Resource* swapChainResources[10] = { nullptr };
	ID3D12Fence* fence = nullptr;
	HANDLE fenceEvent;
	D3D12_RESOURCE_BARRIER barrier{};
	UINT backBufferIndex;
	ID3D12Resource* textureResource;
	DirectX::ScratchImage mipImages;
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;
	ID3D12Resource* depthStencilResource;
};

