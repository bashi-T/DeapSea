#pragma once
#include"WindowApp.h"
#include "list"
#include<cassert>
#include"Debug.h"
#include<d3d12.h>
#include<dxgi1_6.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")
class DX12Common final
{
public:
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
	void DrawScreen();
	void ClearScreen();
	void MakeFence();
	void DX12Release(ID3D12Debug1* debugController);

	ID3D12Device* GetDevice() { return device; }
	ID3D12GraphicsCommandList* GetcommandList() { return commandList; }
	UINT GetBackBufferIndex() { return backBufferIndex; }
	D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
	HANDLE GetFenceEvent() { return fenceEvent; }
	uint64_t fenceValue = 0;
	DX12Common(const DX12Common& obj) = delete;
	DX12Common& oparator(const DX12Common&obj) = delete;
	static DX12Common* GetInstance();

private:
	DX12Common();
	~DX12Common();

	Debug* debug_ = nullptr;
	WinAPP* window_ = nullptr;
	ID3D12Device* device = nullptr;
	IDXGIFactory7* dxgiFactory = nullptr;
	HRESULT hr = NULL;
	IDXGIAdapter4* useAdapter = nullptr;

	ID3D12CommandQueue* commandQueue = nullptr;
	ID3D12CommandAllocator* commandAllocator = nullptr;
	ID3D12GraphicsCommandList* commandList = nullptr;
	IDXGISwapChain4* swapChain = nullptr;
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	ID3D12DescriptorHeap* rtvDescriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
	ID3D12Resource* swapChainResources[10] = { nullptr };
	ID3D12Fence* fence = nullptr;
	HANDLE fenceEvent;
	D3D12_RESOURCE_BARRIER barrier{};
	UINT backBufferIndex;
};

