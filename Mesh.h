#pragma once
#include"DX12Common.h"
#include<dxcapi.h>
#include "list"
#include<cassert>
#include"Debug.h"
#include<d3d12.h>
#include<dxgi1_6.h>
#include"MyImGui.h"
#include"CGVector.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxcompiler.lib")


class Mesh
{
public:
	~Mesh();
	void Initialize(int32_t width, int32_t height);
	void ResetDXC();
	void Update();
	void Draw(
		Vector4 Top,
		Vector4 Right,
		Vector4 Left,
		Vector4 color,
		Vector2 coordTop,
		Vector2 coordRight,
		Vector2 coordLeft);
	IDxcBlob* CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile,
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);

	void MakePSO();
	ID3D12Resource* CreateBufferResource(size_t sizeInBytes);
		
	void MakeVertexBufferView();
	void InputDataTriangle(
		Vector4 Top,
		Vector4 Right,
		Vector4 Left,
		Vector4 color,
		Vector2 coordTop,
		Vector2 coordRight,
		Vector2 coordLeft);
	void DrawTriangle();
	void MeshRelease();

	ID3D12Resource* GetVertexResource() { return vertexResource; }

private:
	Debug* debug_;
	WinAPP* sWinApp;
	MyImGui* imgui_;
	HRESULT hr = NULL;
	TransformMatrix transformMatrix;

	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	IDxcIncludeHandler* includeHandler = nullptr;
	ID3D12RootSignature* rootSignature = nullptr;
	ID3D12PipelineState* graphicsPipelineState=NULL;
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_{};
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	IDxcBlob* pixelShaderBlob = nullptr;
	IDxcBlob* vertexShaderBlob = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	D3D12_VIEWPORT viewport{};
	D3D12_RECT scissorRect{};
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	D3D12_BLEND_DESC blendDesc{};
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	ID3D12Resource* vertexResource = nullptr;
	ID3D12Resource* materialResource;
	ID3D12Resource* wvpResource;
	float clearColor[4] = {0.1f, 0.25f, 0.5f, 1.0f};

	Vector4* materialData = nullptr;
	Matrix4x4* wvpData = nullptr;
	struct VertexData
	{
		Vector4 position;
		Vector2 texcoord;
	};

	TransformMatrix cameraTransform;

	Matrix4x4 cameraMatrix;
	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 worldViewProjectionMatrix;
};

