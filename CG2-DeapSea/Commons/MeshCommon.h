#pragma once
#include "Math/Collision.h"
#include "DX12Common.h"
#include "Systems/MyImGui.h"
#include"Camera/Camera.h"
#include <cassert>
#include <dxcapi.h>
#include <fstream>
#include <sstream>

#pragma comment(lib, "dxcompiler.lib")

namespace MyEngine
{
	class MeshCommon
	{
	public:
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		void Initialize();
		void ResetDXC();
		void MakePSO(DX12Common* dxcommon);
		void SetDefaultCamera(Camera* camera) { this->defaultCamera = camera; }
		ComPtr<ID3D12PipelineState> GetGraphicsPipelineState() { return graphicsPipelineState; }
		ComPtr<ID3D12RootSignature> GetRootSignature() { return rootSignature; }
		DX12Common* GetDx12Common() { return DX12Common::GetInstance(); }

		static MeshCommon* GetInstance();
		static void DeleteInstance();

		MeshCommon() = default;
		~MeshCommon() = default;

	private:
		MeshCommon(const MeshCommon& obj) = delete;
		MeshCommon& oparator(const MeshCommon& obj) = delete;

		Debug* debug_ = nullptr;
		HRESULT hr = NULL;
		DX12Common* dx12Common_;
		EulerTransform transformMatrix{};
		Camera* defaultCamera = nullptr;

		static inline std::unique_ptr<MeshCommon> instance;

		ComPtr<IDxcUtils> dxcUtils = nullptr;
		ComPtr<IDxcCompiler3> dxcCompiler = nullptr;
		ComPtr<IDxcIncludeHandler> includeHandler = nullptr;
		ComPtr<ID3D12RootSignature> rootSignature = nullptr;
		ComPtr<ID3D12PipelineState> graphicsPipelineState = NULL;
		ComPtr<ID3DBlob> signatureBlob = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;
	};
}

