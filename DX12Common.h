#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include"Debug.h"
#include"WindowApp.h"
#include"Mesh.h"
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class DX12Common
{
public:
	DX12Common();
	~DX12Common();
	void Init();
	void MakeDXGIFactory();
	void ChoseUseAdapter();
	void MakeD3D12Device();

	void MakeCommandQueue();
	void MakeCommandList();

	void MakeSwapchain(int32_t width, int32_t height, HWND hwnd_);
	void MakeDescriptorHeap();
	void BringResources();
	void MakeRTV();

	void MakeScreen();
	void MakeFence();
	void DX12Release(ID3D12Debug1* debugController);

	ID3D12Device* GetDevice() { return device; }

private:
	Debug* debug_ = nullptr;
	WinAPP* window_ = nullptr;
	ID3D12Device* device = nullptr;
	Mesh* mesh_ = nullptr;

	IDXGIFactory7 * dxgiFactory = nullptr;
	HRESULT hr = NULL;
	IDXGIAdapter4* useAdapter = nullptr;

	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;
	IDXGISwapChain4* swapChain = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	ID3D12DescriptorHeap* rtvDescriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
	ID3D12Resource* swapChainResources[2] = { nullptr };
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
	ID3D12Fence* fence = nullptr;
	uint64_t fenceValue = 0;
	HANDLE fenceEvent;
};

