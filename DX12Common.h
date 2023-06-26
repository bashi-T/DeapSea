#pragma once
#include"WindowApp.h"
#include"Mesh.h"

class DX12Common
{
public:
	DX12Common();
	~DX12Common();
	static void Init();
	static void MakeDXGIFactory();
	static void ChoseUseAdapter();
	static void MakeD3D12Device();

	static void MakeCommandQueue();
	static void MakeCommandList();

	static void MakeSwapchain(int32_t width, int32_t height, HWND hwnd_);
	static void MakeDescriptorHeap();
	static void BringResources();
	static void MakeRTV();

	static void MakeScreen();
	static void ClearScreen();
	static void MakeFence();
	static void DX12Release(ID3D12Debug1* debugController);

	static ID3D12Device* GetDevice() { return device; }
	static ID3D12GraphicsCommandList* GetcommandList() { return commandList; }

private:
	static inline Debug* debug_ = nullptr;
	static inline WinAPP* window_ = nullptr;
	static inline ID3D12Device* device = nullptr;

	static inline IDXGIFactory7* dxgiFactory = nullptr;
	static inline HRESULT hr = NULL;
	static inline IDXGIAdapter4* useAdapter = nullptr;

	static inline ID3D12CommandQueue* commandQueue = nullptr;
	static inline ID3D12CommandAllocator* commandAllocator = nullptr;
	static inline ID3D12GraphicsCommandList* commandList = nullptr;
	static inline IDXGISwapChain4* swapChain = nullptr;
	static inline DXGI_SWAP_CHAIN_DESC1 swapChainDesc{};
	static inline ID3D12DescriptorHeap* rtvDescriptorHeap = nullptr;
	static inline D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
	static inline ID3D12Resource* swapChainResources[2] = { nullptr };
	static inline D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[2];
	static inline ID3D12Fence* fence = nullptr;
	static inline uint64_t fenceValue = 0;
	static inline HANDLE fenceEvent;
};

