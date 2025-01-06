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
	class Object3dCommon
	{
	public:
		void Initialize();
		void ResetDXC();
		void MakePSO(DX12Common* dxcommon);
		void MakeSkeltonPSO(DX12Common* dxcommon);
		void SetDefaultCamera(Camera* camera) { this->defaultCamera = camera; }
		static Object3dCommon* GetInstance();
		static void DeleteInstance();

		ComPtr<ID3D12PipelineState> GetGraphicsPipelineStates(int32_t num) { return graphicsPipelineStates[num]; }
		ComPtr<ID3D12RootSignature> GetRootSignatures(int32_t num) { return rootSignatures[num]; }
		DX12Common* GetDx12Common() { return DX12Common::GetInstance(); }
		Camera* GetDefaultCamera()const { return defaultCamera; }

		Object3dCommon() = default;
		~Object3dCommon() = default;

	private:
		Object3dCommon(const Object3dCommon& obj) = delete;
		Object3dCommon& oparator(const Object3dCommon& obj) = delete;

		Debug* debug_ = nullptr;
		HRESULT hr = NULL;
		EulerTransform transformMatrix{};
		Camera* defaultCamera = nullptr;

		DX12Common* dx12Common_;
		static inline std::unique_ptr<Object3dCommon> instance;

		ComPtr<ID3D12Resource> transformationMatrixResource;

		ComPtr<IDxcUtils> dxcUtils = nullptr;
		ComPtr<IDxcCompiler3> dxcCompiler = nullptr;
		ComPtr<IDxcIncludeHandler> includeHandler = nullptr;
		std::array<ComPtr<ID3D12RootSignature>, 2> rootSignatures;
		std::array < ComPtr<ID3D12PipelineState>, 2>  graphicsPipelineStates;
		ComPtr<ID3DBlob> signatureBlob = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;

		ComPtr<ID3D12Resource> materialResource;

		ComPtr<ID3D12Resource> directionalLightResource;

	};
}
