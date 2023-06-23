#include "DX12Common.h"

DX12Common::DX12Common()
{
}

DX12Common::~DX12Common()
{
}

void DX12Common::Init()
{
	MakeDXGIFactory();
	ChoseUseAdapter();
	MakeD3D12Device();
#ifdef _DEBUG
	debug_->InfoQueue(GetDevice());
#endif
	MakeScreen();
	mesh_->ResetDXC();
	mesh_->MakePSO(GetDevice());
}

void DX12Common::MakeDXGIFactory()
{
	hr = CreateDXGIFactory(IID_PPV_ARGS(&dxgiFactory));
	assert(SUCCEEDED(hr));
}

void DX12Common::ChoseUseAdapter()
{
	for (UINT i = 0; dxgiFactory->EnumAdapterByGpuPreference(
		i, DXGI_GPU_PREFERENCE_HIGH_PERFORMANCE,
		IID_PPV_ARGS(&useAdapter)) != DXGI_ERROR_NOT_FOUND; i++)
	{
		DXGI_ADAPTER_DESC3 adapterDesc{};
		hr = useAdapter->GetDesc3(&adapterDesc);
		assert(SUCCEEDED(hr));

		if (!(adapterDesc.Flags & DXGI_ADAPTER_FLAG3_SOFTWARE))
		{
			debug_->Log(debug_->ConvertString(
				std::format(L"Use Adapter:{}\n",
					adapterDesc.Description)));
			break;
		}
		useAdapter = nullptr;
	}
}

void DX12Common::MakeD3D12Device()
{
	D3D_FEATURE_LEVEL featureLevels[] =
	{
	D3D_FEATURE_LEVEL_12_2,
	D3D_FEATURE_LEVEL_12_1,
	D3D_FEATURE_LEVEL_12_0
	};
	const char* featureLevelStrings[] =
	{
		"12.2",
		"12.1",
		"12.0"
	};
	for (size_t i = 0; i < _countof(featureLevels); ++i)
	{
		hr = D3D12CreateDevice(useAdapter, featureLevels[i],
			IID_PPV_ARGS(&device));
		if (SUCCEEDED(hr))
		{
			debug_->Log(std::format("featureLevel {}\n",
				featureLevelStrings[i]));
			break;
	    }
	}
	assert(device != nullptr);
	debug_->Log("Complete create D3D12Device\n");
}

void DX12Common::MakeCommandQueue()
{
	D3D12_COMMAND_QUEUE_DESC commandQueueDesc{};
	hr = device->CreateCommandQueue(
		&commandQueueDesc, IID_PPV_ARGS(&commandQueue));
	assert(SUCCEEDED(hr));
}

void DX12Common::MakeCommandList()
{
	hr = device->CreateCommandAllocator(
		D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&commandAllocator));
	assert(SUCCEEDED(hr));
	hr = device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT,
		commandAllocator, nullptr, IID_PPV_ARGS(&commandList));
	assert(SUCCEEDED(hr));
}

void DX12Common::MakeSwapchain(int32_t width, int32_t height, HWND hwnd_)
{
	swapChainDesc.Width = width;
	swapChainDesc.Height = height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	hr = dxgiFactory->CreateSwapChainForHwnd
	(
		commandQueue,
		hwnd_,
		&swapChainDesc,
		nullptr,
		nullptr,
		reinterpret_cast<IDXGISwapChain1**>(&swapChain)
	);
	assert(SUCCEEDED(hr));
}

void DX12Common::MakeDescriptorHeap()
{
	rtvDescriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvDescriptorHeapDesc.NumDescriptors = 2;
	hr = device->CreateDescriptorHeap(&rtvDescriptorHeapDesc,
		IID_PPV_ARGS(&rtvDescriptorHeap));
	assert(SUCCEEDED(hr));
}

void DX12Common::BringResources()
{
	hr = swapChain->GetBuffer(0, IID_PPV_ARGS(&swapChainResources[0]));
	assert(SUCCEEDED(hr));
	hr = swapChain->GetBuffer(1, IID_PPV_ARGS(&swapChainResources[1]));
	assert(SUCCEEDED(hr));
}

void DX12Common::MakeRTV()
{
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc{};
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle =
		rtvDescriptorHeap->GetCPUDescriptorHandleForHeapStart();

	rtvHandles[0] = rtvStartHandle;
	device->CreateRenderTargetView(
		swapChainResources[0], &rtvDesc, rtvHandles[0]);

	rtvHandles[1].ptr = rtvHandles[0].ptr + device->
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	device->CreateRenderTargetView(
		swapChainResources[1], &rtvDesc, rtvHandles[1]);
}

void DX12Common::MakeScreen()
{
	//
	MakeCommandQueue();
	//
	MakeCommandList();
	//
	MakeSwapchain(
		WinAPP::GetClientWidth(),
		WinAPP::GetClientHeight(),
		WinAPP::GetHWND()
	);
	//
	MakeDescriptorHeap();
	//
	BringResources();
	MakeRTV();

	UINT backBufferIndex = swapChain->GetCurrentBackBufferIndex();
	D3D12_RESOURCE_BARRIER barrier{};
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = swapChainResources[backBufferIndex];
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	commandList->ResourceBarrier(1, &barrier);
	commandList->OMSetRenderTargets(1, &rtvHandles[backBufferIndex],
		false, nullptr);
	
	float clearColor[] =
	{
		0.1f, 0.25f,0.5f,1.0f
	};
	commandList->ClearRenderTargetView(
		rtvHandles[backBufferIndex], clearColor, 0, nullptr);
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	commandList->ResourceBarrier(1, &barrier);
	hr = commandList->Close();
	assert(SUCCEEDED(hr));

	ID3D12CommandList* commandLists[] =
	{
		commandList
	};
	commandQueue->ExecuteCommandLists(1, commandLists);
	swapChain->Present(1, 0);

	MakeFence();

	mesh_->ResetDXC();

	fenceValue++;
	commandQueue->Signal(fence, fenceValue);

	if (fence->GetCompletedValue() < fenceValue)
	{
		fence->SetEventOnCompletion(fenceValue, fenceEvent);
		WaitForSingleObject(fenceEvent, INFINITE);
	}
	hr = commandAllocator->Reset();
	assert(SUCCEEDED(hr));
	hr = commandList->Reset(commandAllocator,nullptr);
	assert(SUCCEEDED(hr));

}

void DX12Common::MakeFence()
{
	hr = device->CreateFence(fenceValue,
		D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(hr));
	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);
}

void DX12Common::DX12Release(ID3D12Debug1* debugController)
{
	CloseHandle(fenceEvent);
	fence->Release();
	rtvDescriptorHeap->Release();
	swapChainResources[0]->Release();
	swapChainResources[1]->Release();
	swapChain->Release();
	commandList->Release();
	commandAllocator->Release();
	commandQueue->Release();
	device->Release();
	useAdapter->Release();
	dxgiFactory->Release();
#ifdef _DEBUG
	debugController->Release();
#endif
	CloseWindow(window_->GetHWND());
}
