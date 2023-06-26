#pragma once
#include<cassert>
#include"Debug.h"
#include<dxcapi.h>
#include<d3d12.h>
#include<dxgi1_6.h>

#pragma comment(lib,"dxcompiler.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class Mesh
{
public:
	void Initialize(int32_t width, int32_t height);
	static void ResetDXC();

	static IDxcBlob* CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile,
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);
	static void MakePSO(ID3D12Device* device);
	static void MakeVertexResource(ID3D12Device* device);
	static void MakeVertexBufferView();
	static void InputData();
	static void Draw(ID3D12GraphicsCommandList* commandList);
	static void MeshRelease();

	static ID3D12PipelineState* GetGraphicsPipelineState() { return graphicsPipelineState; }

private:
	static inline Debug* debug_;
	static inline HRESULT hr = NULL;
	static inline ID3D12GraphicsCommandList* commandList;

	static inline IDxcUtils* dxcUtils = nullptr;
	static inline IDxcCompiler3* dxcCompiler = nullptr;
	static inline IDxcIncludeHandler* includeHandler = nullptr;
	static inline ID3D12RootSignature* rootSignature = nullptr;
	static inline ID3D12PipelineState* graphicsPipelineState = nullptr;
	static inline ID3DBlob* signatureBlob = nullptr;
	static inline ID3DBlob* errorBlob = nullptr;
	static inline IDxcBlob* pixelShaderBlob = nullptr;
	static inline IDxcBlob* vertexShaderBlob = nullptr;
	static inline D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	static inline ID3D12Resource* vertexResource = nullptr;
	static inline D3D12_VIEWPORT viewport{};
	static inline D3D12_RECT scissorRect{};
};
struct Vector4 final {
	float x;
	float y;
	float z;
	float w;
};
