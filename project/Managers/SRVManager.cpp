#include "SRVManager.h"

const uint32_t SRVManager::kMaxSRVCount = 512;
const uint32_t SRVManager::kSRVIndexTop = 0;

void SRVManager::Initialize(DX12Common* dxCommon)
{
	this->dxCommon_ = dxCommon;
	MakeFence();
	descriptorHeap = dxCommon_->CreateDescriptorHeap(
		D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
		kMaxSRVCount,
		true);
	descriptorSize = dxCommon_->GetDevice()->
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
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

}

uint32_t SRVManager::Allocate()
{
	assert(CheckNumTexture(useIndex));
	int index = useIndex;
	useIndex++;
	return index;
}

D3D12_CPU_DESCRIPTOR_HANDLE SRVManager::GetCPUDescriptorHandle(uint32_t index)
{
	D3D12_CPU_DESCRIPTOR_HANDLE handleCPU = descriptorHeap->GetCPUDescriptorHandleForHeapStart();
	handleCPU.ptr += (descriptorSize * index);
	return handleCPU;
}

D3D12_GPU_DESCRIPTOR_HANDLE SRVManager::GetGPUDescriptorHandle(uint32_t index)
{
	D3D12_GPU_DESCRIPTOR_HANDLE handleGPU = descriptorHeap->GetGPUDescriptorHandleForHeapStart();
	handleGPU.ptr += (descriptorSize * index);
	return handleGPU;
}

void SRVManager::CreateSRVforTexture2D(
	uint32_t srvIndex,
	ID3D12Resource* pResource,
	DXGI_FORMAT Format,
	UINT MipLevels)
{
	//D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	//srvDesc.Format = Format;
	//srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	//srvDesc.Texture2D.MipLevels = UINT(MipLevels);
	//
	//dxCommon_->GetDevice().Get()->CreateShaderResourceView(
	//	pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
	//
	//D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = dxCommon_->GetRtvDesc();
	//auto renderTextureResource = CreateRenderTextureResource(
	//	rtvDesc.Format, kRenderTargetClearValue);
	//const uint32_t descriptorSizeRTV = dxCommon_->GetDevice()->
	//	GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	//dxCommon_->GetDevice()->CreateRenderTargetView(
	//	renderTextureResource.Get(),
	//	&rtvDesc,
	//	dxCommon_->GetCPUDescriptorHandle(dxCommon_->GetRtvDescriptorHeap().Get(), descriptorSizeRTV, 2));
	//
	//D3D12_SHADER_RESOURCE_VIEW_DESC renderTextureSrvDesc{};
	//renderTextureSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	//renderTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	//renderTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	//renderTextureSrvDesc.Texture2D.MipLevels = 1;
	//
	//dxCommon_->GetDevice()->CreateShaderResourceView(
	//	renderTextureResource.Get(), &renderTextureSrvDesc,
	//	GetCPUDescriptorHandle(srvIndex));

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = Format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(MipLevels);

	dxCommon_->GetDevice().Get()->CreateShaderResourceView(
		pResource, &srvDesc, GetCPUDescriptorHandle(srvIndex));
}

void SRVManager::CreateSRVforStructuredBuffer(
	uint32_t srvIndex,
	ID3D12Resource* pResource,
	UINT numElements,
	UINT structureByteStride)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements = numElements;
	instancingSrvDesc.Buffer.StructureByteStride =structureByteStride;

	dxCommon_->GetDevice().Get()->CreateShaderResourceView(
		pResource, &instancingSrvDesc, GetCPUDescriptorHandle(srvIndex));
}

void SRVManager::SetGraphicsRootDescriptorTable(UINT RootParamaterIndex, uint32_t srvIndex)
{
	dxCommon_->GetCommandList()->SetGraphicsRootDescriptorTable(RootParamaterIndex, GetGPUDescriptorHandle(srvIndex));
}

bool SRVManager::CheckNumTexture(uint32_t textureIndex)
{
	if (kMaxSRVCount > textureIndex)
	{
		return true;
	}
	else {
		return false;
	}
}

void SRVManager::PreDraw()
{
	backBufferIndex = dxCommon_->GetSwapChain()->GetCurrentBackBufferIndex();
	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = dxCommon_->GetSwapChainResources()[backBufferIndex].Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
	dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier);

	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = dxCommon_->GetRtvDesc();
	auto renderTextureResource = CreateRenderTextureResource(
		rtvDesc.Format, kRenderTargetClearValue);
	const uint32_t descriptorSizeRTV = dxCommon_->GetDevice()->
		GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	dxCommon_->GetDevice()->CreateRenderTargetView(
		renderTextureResource.Get(),
		&rtvDesc,
		dxCommon_->GetCPUDescriptorHandle(dxCommon_->GetRtvDescriptorHeap().Get(), descriptorSizeRTV, 2));

	D3D12_SHADER_RESOURCE_VIEW_DESC renderTextureSrvDesc{};
	renderTextureSrvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	renderTextureSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	renderTextureSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	renderTextureSrvDesc.Texture2D.MipLevels = 1;

	dxCommon_->GetDevice()->CreateShaderResourceView(
		renderTextureResource.Get(), &renderTextureSrvDesc,
		GetCPUDescriptorHandle(2));

	D3D12_CPU_DESCRIPTOR_HANDLE rtv =
		dxCommon_->GetRtvHandles(backBufferIndex);
	D3D12_CPU_DESCRIPTOR_HANDLE dsv = dxCommon_->GetDsvHandle();
	dxCommon_->GetCommandList().Get()->
		OMSetRenderTargets(1, &rtv, false, &dsv);

	float newClearColor[4] =
	{
		kRenderTargetClearValue.x,
		kRenderTargetClearValue.y,
		kRenderTargetClearValue.z,
		kRenderTargetClearValue.a
	};
		dxCommon_->GetCommandList()->ClearRenderTargetView(
		dxCommon_->GetRtvHandles(backBufferIndex),
			newClearColor, 0, nullptr);

	dxCommon_->GetCommandList()->ClearDepthStencilView(
		dxCommon_->GetDsvHandle(),
		D3D12_CLEAR_FLAG_DEPTH,
		1.0f,
		0,
		0,
		nullptr);

	ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] =
	{
		descriptorHeap
	};
	dxCommon_->GetCommandList()->
		SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());
	dxCommon_->GetCommandList()->RSSetViewports(1, &viewport);
	dxCommon_->GetCommandList()->RSSetScissorRects(1, &scissorRect);

}

void SRVManager::PreDrawImGui()
{
	backBufferIndex = dxCommon_->GetSwapChain()->GetCurrentBackBufferIndex();

	barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
	//barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
	barrier.Transition.pResource = dxCommon_->GetSwapChainResources()[backBufferIndex].Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier);

	D3D12_CPU_DESCRIPTOR_HANDLE rtv = dxCommon_->GetRtvHandles(backBufferIndex);
	dxCommon_->GetCommandList()->OMSetRenderTargets(1, &rtv, false, nullptr);
/*
	dxCommon_->GetCommandList()->ClearRenderTargetView(
		dxCommon_->GetRtvHandles(backBufferIndex),
		clearColor, 0, nullptr);
		*/
	ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] =
	{
		descriptorHeap.Get()
	};
	dxCommon_->GetCommandList()->
		SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());
	dxCommon_->GetCommandList()->RSSetViewports(1, &viewport);
	dxCommon_->GetCommandList()->RSSetScissorRects(1, &scissorRect);
}

void SRVManager::PostDraw()
{
	/*
	backBufferIndex = dxCommon_->GetSwapChain()->GetCurrentBackBufferIndex();
	barrier.Transition.pResource = dxCommon_->GetSwapChainResources()[backBufferIndex].Get();
	barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
	barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
	dxCommon_->GetCommandList()->ResourceBarrier(1, &barrier);
	*/
	HRESULT hr = dxCommon_->GetCommandList()->Close();
	assert(SUCCEEDED(hr));

	ComPtr<ID3D12CommandList> commandLists[] =
	{
		dxCommon_->GetCommandList().Get()
	};
	dxCommon_->GetCommandQueue()->ExecuteCommandLists(1, commandLists->GetAddressOf());
	dxCommon_->GetSwapChain()->Present(1, 0);
	fenceValue++;
	dxCommon_->GetCommandQueue()->Signal(fence.Get(), fenceValue);

	if (fence->GetCompletedValue() < fenceValue)
	{
		fence->SetEventOnCompletion(fenceValue, fenceEvent);
		WaitForSingleObject(fenceEvent, INFINITE);
	}

	hr = dxCommon_->GetCommandAllocator()->Reset();
	assert(SUCCEEDED(hr));
	hr = dxCommon_->GetCommandList()->Reset(dxCommon_->GetCommandAllocator().Get(), nullptr);
	assert(SUCCEEDED(hr));
}

SRVManager* SRVManager::GetInstance()
{
	if (instance == NULL)
	{
		instance = new SRVManager;
	}
	return instance;
}

ComPtr<ID3D12Resource> SRVManager::CreateRenderTextureResource(DXGI_FORMAT format, const Vector4& color)
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

void SRVManager::MakeFence()
{
	HRESULT hr = dxCommon_->GetDevice()->CreateFence(fenceValue,
		D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
	assert(SUCCEEDED(hr));
	fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
	assert(fenceEvent != nullptr);
}
