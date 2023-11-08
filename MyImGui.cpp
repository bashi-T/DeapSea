#include "MyImGui.h"

MyImGui::~MyImGui()
{
}

void MyImGui::Initialize(
    HWND hwnd,
	Microsoft::WRL::ComPtr<ID3D12Device> device,
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc,
	D3D12_RENDER_TARGET_VIEW_DESC rtvDesc,
	Microsoft::WRL::ComPtr<ID3D12DescriptorHeap> srvDescriptorHeap) {
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGui::StyleColorsDark();
	ImGui_ImplWin32_Init(hwnd);
	ImGui_ImplDX12_Init(
		device.Get(),
		swapChainDesc.BufferCount,
		rtvDesc.Format,
		srvDescriptorHeap.Get(),
	    srvDescriptorHeap->GetCPUDescriptorHandleForHeapStart(),
	    srvDescriptorHeap->GetGPUDescriptorHandleForHeapStart());
}

void MyImGui::Update()
{
	ImGui_ImplDX12_NewFrame();
	ImGui_ImplWin32_NewFrame();
	ImGui::NewFrame();

}

void MyImGui::Endframe(Microsoft::WRL::ComPtr<ID3D12GraphicsCommandList> commandList) {
	ImGui::Render();
	ImGui_ImplDX12_RenderDrawData(ImGui::GetDrawData(), commandList.Get());
}
