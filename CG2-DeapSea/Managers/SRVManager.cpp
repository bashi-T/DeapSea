#include "SRVManager.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

namespace MyEngine
{
	const uint32_t SRVManager::kMaxSRVCount = 5120;
	const uint32_t SRVManager::kSRVIndexTop = 0;

	void SRVManager::Initialize()
	{
		dx12Common_ = DX12Common::GetInstance();
		MakeFence();
		descriptorHeap = dx12Common_->CreateDescriptorHeap(
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV,
			kMaxSRVCount,
			true);
		descriptorSize = dx12Common_->GetDevice()->
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
		int32_t index = useIndex;
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
		D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
		srvDesc.Format = Format;
		srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
		srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
		srvDesc.Texture2D.MipLevels = UINT(MipLevels);

		dx12Common_->GetDevice().Get()->CreateShaderResourceView(
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
		instancingSrvDesc.Buffer.StructureByteStride = structureByteStride;

		dx12Common_->GetDevice()->CreateShaderResourceView(
			pResource, &instancingSrvDesc, GetCPUDescriptorHandle(srvIndex));
	}

	void SRVManager::SetGraphicsRootDescriptorTable(UINT RootParamaterIndex, uint32_t srvIndex)
	{
		dx12Common_->GetCommandList()->SetGraphicsRootDescriptorTable(RootParamaterIndex, GetGPUDescriptorHandle(srvIndex));
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
		backBufferIndex = dx12Common_->GetSwapChain()->GetCurrentBackBufferIndex();

		barrier.Type = D3D12_RESOURCE_BARRIER_TYPE_TRANSITION;
		barrier.Flags = D3D12_RESOURCE_BARRIER_FLAG_NONE;
		barrier.Transition.pResource = dx12Common_->GetSwapChainResources()[backBufferIndex].Get();
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_PRESENT;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_RENDER_TARGET;
		dx12Common_->GetCommandList()->ResourceBarrier(1, &barrier);

		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles = dx12Common_->GetRtvHandles(backBufferIndex);
		dx12Common_->GetCommandList()->OMSetRenderTargets(1,
			&rtvHandles, false, nullptr);

		dx12Common_->GetCommandList()->ClearRenderTargetView(
			rtvHandles, clearColor, 0, nullptr);

		dx12Common_->GetCommandList()->ClearDepthStencilView(
			dx12Common_->GetDsvHandle(),
			D3D12_CLEAR_FLAG_DEPTH,
			1.0f,
			0,
			0,
			nullptr);

		ComPtr<ID3D12DescriptorHeap> descriptorHeaps[] =
		{
			descriptorHeap.Get()
		};
		dx12Common_->GetCommandList()->
			SetDescriptorHeaps(1, descriptorHeaps->GetAddressOf());
		dx12Common_->GetCommandList()->RSSetViewports(1, &viewport);
		dx12Common_->GetCommandList()->RSSetScissorRects(1, &scissorRect);
	}

	void SRVManager::PostDraw()
	{
		backBufferIndex = dx12Common_->GetSwapChain()->GetCurrentBackBufferIndex();
		barrier.Transition.StateBefore = D3D12_RESOURCE_STATE_RENDER_TARGET;
		barrier.Transition.StateAfter = D3D12_RESOURCE_STATE_PRESENT;
		dx12Common_->GetCommandList()->ResourceBarrier(1, &barrier);

		hr = dx12Common_->GetCommandList()->Close();
		assert(SUCCEEDED(hr));

		ComPtr<ID3D12CommandList> commandLists[] =
		{
			dx12Common_->GetCommandList().Get()
		};
		dx12Common_->GetCommandQueue()->ExecuteCommandLists(1, commandLists->GetAddressOf());
		dx12Common_->GetSwapChain()->Present(1, 0);
		fenceValue++;
		dx12Common_->GetCommandQueue()->Signal(fence.Get(), fenceValue);

		if (fence->GetCompletedValue() < fenceValue)
		{
			fence->SetEventOnCompletion(fenceValue, fenceEvent);
			WaitForSingleObject(fenceEvent, INFINITE);
		}

		hr = dx12Common_->GetCommandAllocator()->Reset();
		assert(SUCCEEDED(hr));
		hr = dx12Common_->GetCommandList()->Reset(dx12Common_->GetCommandAllocator().Get(), nullptr);
		assert(SUCCEEDED(hr));
	}

	SRVManager* SRVManager::GetInstance()
	{
		if (instance == NULL)
		{
			instance = std::make_unique<SRVManager>();
		}
		return instance.get();
	}

	void SRVManager::Finalize()
	{
		instance.reset();
	}

	void SRVManager::MakeFence()
	{
		hr = dx12Common_->GetDevice()->CreateFence(fenceValue,
			D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence));
		assert(SUCCEEDED(hr));
		fenceEvent = CreateEvent(NULL, FALSE, FALSE, NULL);
		assert(fenceEvent != nullptr);
	}
}
