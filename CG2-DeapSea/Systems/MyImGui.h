#pragma once
#include<windows.h>
#include<cstdint>
#include "externals/imgui/imgui.h"
#include "externals/imgui/imgui_impl_dx12.h"
#include"externals/imgui/imgui_impl_win32.h"
#include<WRL.h>
#include<thread>

#include<d3d12.h>
#include<dxgi1_6.h>
#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")

	extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(
		HWND hWnd,
		UINT msg,
		WPARAM wparam,
		LPARAM lparam
	);

namespace MyEngine
{
	using Microsoft::WRL::ComPtr;

	/*
	 * MyImGui.h
	 * ImGui用の処理を行う
	 */
	class MyImGui
	{
	public:
		void Initialize(
			HWND hwnd,
			ID3D12Device* device,
			DXGI_SWAP_CHAIN_DESC1 swapChainDesc,
			D3D12_RENDER_TARGET_VIEW_DESC rtvDesc,
			ID3D12DescriptorHeap* srvDescriptorHeap);
		void Update();
		void Endframe(ID3D12GraphicsCommandList* commandList);
		static MyImGui* GetInstance();

		void Finalize();

		MyImGui() = default;
		~MyImGui() = default;
	private:
		MyImGui(MyImGui&) = delete;
		MyImGui& operator=(MyImGui&) = delete;

		static inline std::unique_ptr<MyImGui> instance;
	};
}
