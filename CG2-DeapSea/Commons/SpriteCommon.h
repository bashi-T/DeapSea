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
	class SpriteCommon
	{
	public:
		void Initialize();
		//void Update();
		//void Draw(int32_t width, int32_t height);
		void ResetDXC();
		void MakePSO(DX12Common* dxcommon);
		static SpriteCommon* GetInstance();
		static void DeleteInstance();

		//void InputDataTriangle(
		//	Vector4 Top, Vector4 Right, Vector4 Left, Vector4 color, Vector2 coordTop,
		//	Vector2 coordRight, Vector2 coordLeft);

		//void DrawTriangle(
		//	Vector4 Top, Vector4 Right, Vector4 Left, Vector4 color, Vector2 coordTop,
		//	Vector2 coordRight, Vector2 coordLeft, bool useWorldMap);

		ComPtr<ID3D12Resource> GetVertexResource() { return vertexResource; }
		ComPtr<ID3D12PipelineState> GetGraphicsPipelineState() { return graphicsPipelineState; }
		ComPtr<ID3D12RootSignature> GetRootSignature() { return rootSignature; }
		DX12Common* GetDx12Common() { return DX12Common::GetInstance(); }

		struct DirectionalLight {
			Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
			Vector3 direction = { 0.0f, -1.0f, 0.0f };
			float intensity = 1.0f;
		};

		SpriteCommon() = default;
		~SpriteCommon() = default;
	private:
		SpriteCommon(SpriteCommon&) = delete;
		SpriteCommon& operator=(SpriteCommon&) = delete;

		Debug* debug_ = nullptr;
		WinAPP* sWinApp = nullptr;
		MyImGui* imgui_ = nullptr;
		HRESULT hr = NULL;
		DX12Common* dx12Common_ = nullptr;
		EulerTransform transformMatrix{};
		ComPtr<ID3D12Resource> transformationMatrixResource;
		static inline std::unique_ptr<SpriteCommon> instance;

		ComPtr<IDxcUtils> dxcUtils = nullptr;
		ComPtr<IDxcCompiler3> dxcCompiler = nullptr;
		ComPtr<IDxcIncludeHandler> includeHandler = nullptr;
		ComPtr<ID3D12RootSignature> rootSignature = nullptr;
		ComPtr<ID3D12PipelineState> graphicsPipelineState = NULL;
		ComPtr<ID3DBlob> signatureBlob = nullptr;
		ComPtr<ID3DBlob> errorBlob = nullptr;

		ComPtr<ID3D12Resource> vertexResource = nullptr;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

		ComPtr<ID3D12Resource> materialResource;

		ComPtr<ID3D12Resource> directionalLightResource;

	};
}