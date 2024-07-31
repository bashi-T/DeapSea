#include "DX12Common.h"
#include "Managers/SRVManager.h"

DX12Common* DX12Common::GetInstance()
{
	if (instance == NULL)
	{
		instance = new DX12Common;
	}
	return instance;
}

void DX12Common::DeleteInstance()
{
	if (instance != NULL)
	{
		delete instance;
	}
	instance = NULL;
}

bool DX12Common::CheckNumTexture(uint32_t textureIndex)
{
	if (SRVManager::kMaxSRVCount > textureIndex)
	{
		return true;
	}
	else {
		return false;
	}
}

void DX12Common::InitializefixFPS()
{
	reference_ = std::chrono::steady_clock::now();
}

void DX12Common::UpdateFixFPS()
{
	const std::chrono::microseconds kMinTime(uint64_t(1000000.0f / 60.0f));
	const std::chrono::microseconds kMinCheckTime(uint64_t(1000000.0f / 65.0f));
	std::chrono::steady_clock::time_point now = std::chrono::steady_clock::now();
	std::chrono::microseconds elapsed = std::chrono::duration_cast<std::chrono::microseconds>(now - reference_);
	if (elapsed < kMinTime)
	{
		while (std::chrono::steady_clock::now() - reference_ < kMinTime) {
			std::this_thread::sleep_for(std::chrono::microseconds(1));
		}

	}
	reference_ = std::chrono::steady_clock::now();
}

void DX12Common::Initialize(WinAPP* winApp)
{
	this->winApp_ = winApp;
	assert(winApp_);
#ifdef _DEBUG
	DebugLayer();
#endif
	InitializefixFPS();
	MakeDXGIFactory();
	ChoseUseAdapter();
	MakeD3D12Device();
	depthStencilResource = CreatedepthstencilTextureResource();

#ifdef _DEBUG
	InfoQueue(device.Get());
#endif
	MakeScreen(winApp_);
	MakeFence();

	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = float(WinAPP::clientWidth_);
	viewport.Height = float(WinAPP::clientHeight_);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	scissorRect.left = LONG(0.0f);
	scissorRect.right = LONG(WinAPP::clientWidth_);
	scissorRect.top = LONG(0.0f);
	scissorRect.bottom = LONG(WinAPP::clientHeight_);

	//↓initializeでいいかも
	renderTextureResource = CreateRenderTextureResource(
		DXGI_FORMAT_R8G8B8A8_UNORM_SRGB, kRenderTargetClearValue);

	rtvDescriptorHeap = CreateDescriptorHeap(
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		SRVManager::kMaxSRVCount,
		true);
	//descriptorSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	
	descriptorSizeRTV = device->
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	int index = Allocate();
	device->CreateRenderTargetView(
		renderTextureResource.Get(),
		&rtvDesc,
		//GetCPUDescriptorHandle(index)
		GetCPUDescriptorHandle(
			GetRtvDescriptorHeap().Get(),
			descriptorSizeRTV, index));
	//↑initializeでいいかも

}

void DX12Common::update()
{
	UpdateFixFPS();
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
			//debug_->Log(debug_->ConvertString(
			//	std::format(L"Use Adapter:{}\n",
			//		adapterDesc.Description)));
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
		hr = D3D12CreateDevice(useAdapter.Get(), featureLevels[i],
			IID_PPV_ARGS(&device));
		if (SUCCEEDED(hr))
		{
			//debug_->Log(std::format("featureLevel {}\n",
			//	featureLevelStrings[i]));
			break;
		}
	}
	assert(device != nullptr);
	//debug_->Log("Complete create D3D12Device\n");
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
		commandAllocator.Get(), nullptr, IID_PPV_ARGS(&commandList));
	assert(SUCCEEDED(hr));
}

void DX12Common::MakeSwapchain(HWND hwnd_)
{
	swapChainDesc.Width = WinAPP::clientWidth_;
	swapChainDesc.Height = WinAPP::clientHeight_;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.BufferCount = 2;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	hr = dxgiFactory->CreateSwapChainForHwnd
	(
		commandQueue.Get(),
		hwnd_,
		&swapChainDesc,
		nullptr,
		nullptr,
		reinterpret_cast<IDXGISwapChain1**>(swapChain.GetAddressOf())
	);
	assert(SUCCEEDED(hr));
}

void DX12Common::MakeDescriptorHeap()
{
	D3D12_DESCRIPTOR_HEAP_DESC rtvDescriptorHeapDesc{};
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
	rtvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	rtvDesc.ViewDimension = D3D12_RTV_DIMENSION_TEXTURE2D;
	const uint32_t descriptorSizeRTV = device->
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	int index = Allocate();
	D3D12_CPU_DESCRIPTOR_HANDLE rtvStartHandle =
		GetCPUDescriptorHandle(rtvDescriptorHeap.Get(), descriptorSizeRTV, index);

	rtvHandles[0] = rtvStartHandle;
	device->CreateRenderTargetView(
		swapChainResources[0].Get(), &rtvDesc, rtvHandles[0]);

	rtvHandles[1].ptr = rtvHandles[0].ptr + device->
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	device->CreateRenderTargetView(
		swapChainResources[1].Get(), &rtvDesc, rtvHandles[1]);
}

void DX12Common::MakeScreen(WinAPP* winApp)
{
	MakeCommandQueue();
	MakeCommandList();
	MakeSwapchain(winApp->GetHWND());
	MakeDescriptorHeap();

	rtvDescriptorHeap = CreateDescriptorHeap(
		D3D12_DESCRIPTOR_HEAP_TYPE_RTV,
		3,
		false);
	dsvDescriptorHeap = CreateDescriptorHeap(
		D3D12_DESCRIPTOR_HEAP_TYPE_DSV,
		1,
		false);

	BringResources();
	MakeRTV();
	MakeDSV();
}

void DX12Common::DX12Release()
{
#ifdef _DEBUG
	//debugController->Release();
#endif
}

ComPtr<ID3D12DescriptorHeap> DX12Common::CreateDescriptorHeap(
	D3D12_DESCRIPTOR_HEAP_TYPE heapType,
	UINT numDescriptors,
    bool shaderVisible)
{
	ComPtr<ID3D12DescriptorHeap> descriptorHeap = nullptr;
	D3D12_DESCRIPTOR_HEAP_DESC DescriptorHeapDesc{};
	DescriptorHeapDesc.Type = heapType;
	DescriptorHeapDesc.NumDescriptors = numDescriptors;
	DescriptorHeapDesc.Flags =
	    shaderVisible ? D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE : D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	hr = device->CreateDescriptorHeap(&DescriptorHeapDesc, IID_PPV_ARGS(&descriptorHeap));
	assert(SUCCEEDED(hr));
	return descriptorHeap;
}


void DX12Common::MakeDSV()
{
	dsvDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	int index = Allocate();
	device->CreateDepthStencilView(
		depthStencilResource.Get(),
		&dsvDesc,
		dsvDescriptorHeap->GetCPUDescriptorHandleForHeapStart());

	const uint32_t descriptorSizeDSV = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_DSV);
	index = Allocate(); 
	dsvHandle = GetCPUDescriptorHandle(dsvDescriptorHeap.Get(), descriptorSizeDSV, index);
}


ComPtr<ID3D12Resource> DX12Common::CreatedepthstencilTextureResource()
{
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = WinAPP::clientWidth_;
	resourceDesc.Height = WinAPP::clientHeight_;
	resourceDesc.MipLevels = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL;

	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_CLEAR_VALUE depthClearValue{};
	depthClearValue.DepthStencil.Depth = 1.0f;
	depthClearValue.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;

	ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthClearValue,
		IID_PPV_ARGS(&resource));
	assert(SUCCEEDED(hr));
	return resource;
}

D3D12_CPU_DESCRIPTOR_HANDLE DX12Common::GetCPUDescriptorHandle(
	ID3D12DescriptorHeap* descriptorHeap,
	uint32_t descriptorSize,
	uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE DX12Common::GetGPUDescriptorHandle(
	ID3D12DescriptorHeap* descriptorHeap,
	uint32_t descriptorSize,
	uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

//D3D12_CPU_DESCRIPTOR_HANDLE DX12Common::GetSRVCPUDescriptorHandle(uint32_t index)
//{
//	//return GetCPUDescriptorHandle(srvDescriptorHeap,descriptorSizeSRV,index);
//	return D3D12_CPU_DESCRIPTOR_HANDLE();
//}
//
//D3D12_GPU_DESCRIPTOR_HANDLE DX12Common::GetSRVGPUDescriptorHandle(uint32_t index)
//{
//	return D3D12_GPU_DESCRIPTOR_HANDLE();
//}

void DX12Common::DebugLayer()
{
	if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
	{
		debugController->EnableDebugLayer();
		debugController->SetEnableGPUBasedValidation(TRUE);
	}
}

void DX12Common::InfoQueue(ID3D12Device* device)
{
	ComPtr<ID3D12InfoQueue> InfoQueue = nullptr;
	if (SUCCEEDED(device->QueryInterface(IID_PPV_ARGS(&InfoQueue))))
	{
		InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_CORRUPTION, true);
		InfoQueue->SetBreakOnSeverity(D3D12_MESSAGE_SEVERITY_ERROR, true);
		D3D12_MESSAGE_ID denyIds[] =
		{
			D3D12_MESSAGE_ID_RESOURCE_BARRIER_MISMATCHING_COMMAND_LIST_TYPE
		};
		D3D12_MESSAGE_SEVERITY severities[] = { D3D12_MESSAGE_SEVERITY_INFO };
		D3D12_INFO_QUEUE_FILTER filter{};
		filter.DenyList.NumIDs = _countof(denyIds);
		filter.DenyList.pIDList = denyIds;
		filter.DenyList.NumSeverities = _countof(severities);
		filter.DenyList.pSeverityList = severities;
		InfoQueue->PushStorageFilter(&filter);
	}
}

void DX12Common::PreDraw()
{
	//書き込むバックバッファのインデックスを取得
	backBufferIndex = swapChain->GetCurrentBackBufferIndex();
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	//現在のバックバッファをバリアを張る対象にする
	barrier.Transition.pResource =swapChainResources[backBufferIndex].Get();
	//barrier.Transition.pResource = renderTextureResource.Get();
	//現在のresourceState
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	//遷移後のresourceState
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//transitionBarrierを張る
	commandList->ResourceBarrier(1, &barrier);

	ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] =
	{
		SRVManager::GetInstance()->GetSrvDescriptorHeap()
	};
	commandList->SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());
	rtv = rtvHandles[backBufferIndex];
	dsv = dsvHandle;

	commandList->RSSetViewports(1, &viewport);
	commandList->RSSetScissorRects(1, &scissorRect);

	commandList->ClearDepthStencilView(
		dsvHandle,
		D3D12_CLEAR_FLAG_DEPTH,
		1.0f,
		0,
		0,
		nullptr);

	commandList.Get()->//描画先のRTV設定
		OMSetRenderTargets(0, &rtv, false, &dsv);

	float newClearColor[4] =
	{
		kRenderTargetClearValue.x,
		kRenderTargetClearValue.y,
		kRenderTargetClearValue.z,
		kRenderTargetClearValue.a
	};
	//指定した色で画面をクリア
	commandList->ClearRenderTargetView(
		rtvHandles[backBufferIndex],
		newClearColor, 0, nullptr);
}

void DX12Common::PostDraw()
{
	//barrier.Transition.pResource = renderTextureResource.Get();
	barrier.Transition.pResource = swapChainResources[backBufferIndex].Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	//barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	commandList->ResourceBarrier(1, &barrier);

	//barrier.Transition.pResource = renderTextureResource.Get();
	//barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	//barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_COPY_SOURCE;
	//commandList->ResourceBarrier(1, &barrier);

	//swapChainResources[backBufferIndex] = renderTextureResource;
	//commandList->CopyResource(swapChainResources[backBufferIndex].Get(), renderTextureResource.Get());
}

void DX12Common::PreDrawImGui()
{
	backBufferIndex = swapChain->GetCurrentBackBufferIndex();

	barrier.Transition.pResource = swapChainResources[backBufferIndex].Get();
	//barrier.Transition.pResource = renderTextureResource.Get();

	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	//barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_COPY_SOURCE;
	//barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;

	//barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE;
	//barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;

	//barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	//barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	commandList->ResourceBarrier(1, &barrier);

	commandList->RSSetViewports(1, &viewport);
	commandList->RSSetScissorRects(1, &scissorRect);

	commandList->OMSetRenderTargets(1, &rtv, false, nullptr);

	commandList->ClearRenderTargetView(
		rtvHandles[backBufferIndex],
		clearColor, 0, nullptr);

	//fullScreenSprite = new FullScreenSprite;
	//fullScreenSprite->MeshInitialize(FullScreenSpriteCommon::GetInstance(), SRVManager::GetInstance());

}

void DX12Common::PostDrawImGui()
{
	backBufferIndex = swapChain->GetCurrentBackBufferIndex();
	barrier.Transition.pResource = swapChainResources[backBufferIndex].Get();
	//barrier.Transition.pResource = renderTextureResource.Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	commandList->ResourceBarrier(1, &barrier);

	HRESULT hr = commandList->Close();
	assert(SUCCEEDED(hr));

	ComPtr<ID3D12CommandList> commandLists[] =
	{
		commandList
	};
	commandQueue->ExecuteCommandLists(1, commandLists->GetAddressOf());
	swapChain->Present(1, 0);

	fenceValue++;
	commandQueue->Signal(fence.Get(), fenceValue);

	if (fence->GetCompletedValue() < fenceValue)
	{
		fence->SetEventOnCompletion(fenceValue, fenceEvent);
		WaitForSingleObject(fenceEvent, INFINITE);
	}

	hr = commandAllocator->Reset();
	assert(SUCCEEDED(hr));
	hr = commandList->Reset(commandAllocator.Get(), nullptr);
	assert(SUCCEEDED(hr));
}

void DX12Common::MakeFence()
{
	HRESULT hr = device->CreateFence(fenceValue,
		D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(hr));
	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);
}

ComPtr<ID3D12Resource> DX12Common::CreateRenderTextureResource(DXGI_FORMAT format, const Vector4& color)
{
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = WinAPP::clientWidth_;
	resourceDesc.Height = WinAPP::clientHeight_;
	resourceDesc.Format = format;
	resourceDesc.MipLevels = 1;
	resourceDesc.DepthOrArraySize = 1;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
	resourceDesc.Flags = D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET;

	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_DEFAULT;

	D3D12_CLEAR_VALUE clearValue;
	clearValue.Format = format;
	clearValue.Color[0] = color.x;
	clearValue.Color[1] = color.y;
	clearValue.Color[2] = color.z;
	clearValue.Color[3] = color.a;

	ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = DX12Common::GetInstance()->GetDevice()->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_RENDER_TARGET,
		&clearValue,
		IID_PPV_ARGS(&resource));

	assert(SUCCEEDED(hr));

	return resource;
}

uint32_t DX12Common::Allocate()
{
	assert(CheckNumTexture(useIndex));
	int index = useIndex;
	useIndex++;
	return index;
}
