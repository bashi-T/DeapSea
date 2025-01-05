#pragma once
#include "Math/Collision.h"
#include "DX12Common.h"
#include "Systems/MyImGui.h"
#include <cassert>
#include <dxcapi.h>
#include <fstream>
#include <sstream>

#pragma comment(lib, "dxcompiler.lib")

namespace MyEngine
{
	class ParticleCommon
	{
	public:
		void Initialize();
		void ResetDXC();
		void MakePSO(DX12Common* dxcommon);

		static std::shared_ptr<ParticleCommon> GetInstance();
		static void DeleteInstance();

		ComPtr<ID3D12PipelineState> GetGraphicsPipelineState() { return graphicsPipelineState; }
		ComPtr<ID3D12RootSignature> GetRootSignature() { return rootSignature; }
		std::shared_ptr<DX12Common> GetDx12Common() { return DX12Common::GetInstance(); }

		~ParticleCommon() = default;
		ParticleCommon(const ParticleCommon& obj) = delete;
		ParticleCommon& oparator(const ParticleCommon& obj) = delete;
	protected:
		ParticleCommon() = default;

	private:
		Debug* debug_ = nullptr;
		WinAPP* sWinApp = nullptr;
		MyImGui* imgui_ = nullptr;
		HRESULT hr = NULL;
		std::shared_ptr<DX12Common> dx12Common_;
		EulerTransform transformMatrix{};
		ComPtr<ID3D12Resource> transformationMatrixResource;
		static inline std::weak_ptr<ParticleCommon> instance;

		ComPtr<IDxcUtils> dxcUtils_ = nullptr;
		ComPtr<IDxcCompiler3> dxcCompiler_ = nullptr;
		ComPtr<IDxcIncludeHandler> includeHandler_ = nullptr;
		ComPtr<ID3D12RootSignature> rootSignature = nullptr;
		ComPtr<ID3D12PipelineState> graphicsPipelineState = NULL;
		ComPtr<ID3DBlob> signatureBlob = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;

		ComPtr<ID3D12Resource> materialResource;

		ComPtr<ID3D12Resource> directionalLightResource;
	};
}
