#pragma once
#include"DX12Common.h"
#include<dxcapi.h>
#include "list"
#include<cassert>
#include"Debug.h"
#include<d3d12.h>
#include<dxgi1_6.h>
#include"MyImGui.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxcompiler.lib")

struct Vector4 final {
	float x;
	float y;
	float z;
	float w;
};

class Mesh
{
public:
	//Mesh();
	~Mesh();
	void Initialize(int32_t width, int32_t height, int NumTriangle);
	void ResetDXC();
	void Update();
	void Draw(Vector4 Top, Vector4 Right, Vector4 Left, Vector4 color);
	IDxcBlob* CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile,
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);

	void MakePSO();
	void MakeVertexResource(int NumTriangle);
	void MakeMaterialResource(int NumTriangle);
	void MakeVertexBufferView(int NumTriangle);
	void InputDataTriangle(Vector4 Top, Vector4 Right, Vector4 Left, Vector4 color);
	void DrawTriangle();
	void MeshRelease();

	ID3D12Resource* GetVertexResource() { return vertexResource; }

private:
	Debug* debug_;
	WinAPP* sWinApp;
	MyImGui* imgui_;
	HRESULT hr = NULL;
	
	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	IDxcIncludeHandler* includeHandler = nullptr;
	ID3D12RootSignature* rootSignature = nullptr;
	ID3D12PipelineState* graphicsPipelineState=NULL;
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_{};
	D3D12_ROOT_PARAMETER rootParameters[1] = {};
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	IDxcBlob* pixelShaderBlob = nullptr;
	IDxcBlob* vertexShaderBlob = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	ID3D12Resource* vertexResource = nullptr;
	D3D12_VIEWPORT viewport{};
	D3D12_RECT scissorRect{};
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	D3D12_BLEND_DESC blendDesc{};
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	ID3D12Resource* materialResource;
};
