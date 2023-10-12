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
	IDxcBlob* CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile,
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);
	void DrawTriangle(
		Vector4 Top,
		Vector4 Right,
		Vector4 Left,
		Vector4 color,
		Vector2 coordTop,
		Vector2 coordRight,
		Vector2 coordLeft);
	void DrawSprite(
		Vector4 LeftTop,
		Vector4 RightTop,
		Vector4 RightBottom,
		Vector4 LeftBottom,
		Vector4 color,
		Vector2 coordLeftTop,
		Vector2 coordRightTop,
		Vector2 coordRightBottom,
		Vector2 coordLeftBottom);
	void DrawSphere(const Sphere& sphere_,
		Vector4 color);
	

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
	void InputDataSprite(
		Vector4 LeftTop,
		Vector4 RightTop,
		Vector4 RightBottom,
		Vector4 LeftBottom,
		Vector4 color,
		Vector2 coordLeftTop,
		Vector2 coordRightTop,
		Vector2 coordRightBottom,
		Vector2 coordLeftBottom);
	void MeshRelease();

	ID3D12Resource* GetVertexResource() { return vertexResource; }

private:
	Debug* debug_;
	WinAPP* sWinApp;
	MyImGui* imgui_;
	HRESULT hr = NULL;
	TransformMatrix transformMatrix;
	TransformMatrix transformMatrixSprite;
	ID3D12Resource* transformationMatrixResourceSprite;
	Matrix4x4* wvpData = nullptr;
	Matrix4x4* transformationMatrixDataSprite = nullptr;
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

	ID3D12Resource* vertexResource = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	ID3D12Resource* vertexResourceSprite = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite{};

	D3D12_VIEWPORT viewport{};
	D3D12_RECT scissorRect{};
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[2] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	D3D12_BLEND_DESC blendDesc{};
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	ID3D12Resource* materialResource;
	ID3D12Resource* materialResourceSprite;
	ID3D12Resource* wvpResource;
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	Vector4* materialData = nullptr;
	struct VertexData
	{
		Vector4 position;
		Vector2 texcoord;
	};


	TransformMatrix cameraTransform;
	Matrix4x4 cameraMatrix;
	Matrix4x4 cameraMatrixSprite;
	Matrix4x4 viewMatrix;
	Matrix4x4 viewMatrixSprite;
	Matrix4x4 projectionMatrix;
	Matrix4x4 projectionMatrixSprite;
	Matrix4x4 worldViewProjectionMatrix;
	Matrix4x4 worldViewProjectionMatrixSprite;
};

