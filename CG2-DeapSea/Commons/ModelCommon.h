#pragma once
#include "Math/Collision.h"
#include "DX12Common.h"
#include "Systems/MyImGui.h"
#include <cassert>
#include <dxcapi.h>
#include <fstream>
#include <sstream>
#include"Managers/TextureManager.h"

#pragma comment(lib, "dxcompiler.lib")
namespace MyEngine
{
	class ModelCommon
	{
	public:
		void Initialize();
		void ResetDXC();
		void MakePSO(DX12Common* dxcommon);
		void MakeSkeltonPSO(DX12Common* dxcommon);
		static std::shared_ptr<ModelCommon> GetInstance();
		void Finalize();

		std::shared_ptr<DX12Common> GetDx12Common()const { return dx12Common_; }

		~ModelCommon() = default;
		ModelCommon(ModelCommon&) = delete;
		ModelCommon& operator=(ModelCommon&) = delete;
	protected:
		ModelCommon() = default;

	private:
		std::shared_ptr<DX12Common> dx12Common_ = nullptr;
		static inline std::weak_ptr<ModelCommon> instance;
		HRESULT hr = NULL;
		Debug* debug_ = nullptr;

		ComPtr<IDxcUtils> dxcUtils = nullptr;
		ComPtr<IDxcCompiler3> dxcCompiler = nullptr;
		ComPtr<IDxcIncludeHandler> includeHandler = nullptr;
		ComPtr<ID3DBlob> signatureBlob = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;
		ComPtr<ID3D12RootSignature> rootSignature = nullptr;
		ComPtr<ID3D12PipelineState> graphicsPipelineState = NULL;
	};
}