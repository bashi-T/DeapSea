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
		void Initialize(DX12Common* dxcommon);
		ComPtr<IDxcBlob> CompileShader(
			const std::wstring& filePath,
			const wchar_t* profile,
			IDxcUtils* dxcUtils,
			IDxcCompiler3* dxcCompiler,
			IDxcIncludeHandler* includeHandler);
		void ResetDXC();
		void MakePSO(DX12Common* dxcommon);
		void SetDefaultCamera(Camera* camera) { this->defaultCamera = camera; }
		ComPtr<ID3D12PipelineState> GetGraphicsPipelineState() { return graphicsPipelineState; }
		ComPtr<ID3D12RootSignature> GetRootSignature() { return rootSignature; }
		DX12Common* GetDx12Common() { return DX12Common::GetInstance(); }

		static MeshCommon* GetInstance();
		static void DeleteInstance();

	private:
		MeshCommon() = default;
		~MeshCommon() = default;
		MeshCommon(const MeshCommon& obj) = delete;
		MeshCommon& oparator(const MeshCommon& obj) = delete;

		Debug* debug_;
		HRESULT hr = NULL;
		DX12Common* dx12Common_;
		EulerTransform transformMatrix;
		Camera* defaultCamera = nullptr;
		static inline MeshCommon* instance;

		ComPtr<IDxcUtils> dxcUtils = nullptr;
		ComPtr<IDxcCompiler3> dxcCompiler = nullptr;
		ComPtr<IDxcIncludeHandler> includeHandler = nullptr;
		ComPtr<ID3D12RootSignature> rootSignature = nullptr;
		ComPtr<ID3D12PipelineState> graphicsPipelineState = NULL;
		ComPtr<ID3DBlob> signatureBlob = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;
	};
}

