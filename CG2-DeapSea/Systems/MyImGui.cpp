#include "MyImGui.h"
#include"Managers/SRVManager.h"

namespace MyEngine
{
	void MyImGui::Initialize(
		HWND hwnd,
		ID3D12Device* device,
		DXGI_SWAP_CHAIN_DESC1 swapChainDesc,
		D3D12_RENDER_TARGET_VIEW_DESC rtvDesc,
		ID3D12DescriptorHeap* srvDescriptorHeap)
	{
		uint32_t index = SRVManager::GetInstance()->Allocate();
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::StyleColorsDark();
		ImGui_ImplWin32_Init(hwnd);
		ImGui_ImplDX12_Init(
			device,
			swapChainDesc.BufferCount,
			rtvDesc.Format,
			srvDescriptorHeap,
			SRVManager::GetInstance()->
			GetCPUDescriptorHandle(index),
			SRVManager::GetInstance()->
			GetGPUDescriptorHandle(index));
	}

	void MyImGui::Update()
	{
		ImGui_ImplDX12_NewFrame();
		ImGui_ImplWin32_NewFrame();
		ImGui::NewFrame();
	}

	void MyImGui::Endframe(ID3D12GraphicsCommandList* commandList)
	{
		ImGui::Render();
		ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList);
	}

	MyImGui* MyImGui::GetInstance()
	{
		if (instance == NULL)
		{
			instance = std::make_unique<MyImGui>();
		}
		return instance.get();
	}

	void MyImGui::Finalize()
	{
		instance.reset();
		ImGui_ImplDX12_Shutdown();
		ImGui_ImplWin32_Shutdown();
		ImGui::DestroyContext();
	}
}
