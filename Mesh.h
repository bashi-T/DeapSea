#pragma once
#include "CGVector.h"
#include "DX12Common.h"
#include "Debug.h"
#include "MyImGui.h"
#include "list"
#include <cassert>
#include <d3d12.h>
#include <dxcapi.h>
#include <dxgi1_6.h>
#include <fstream>
#include <sstream>

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxcompiler.lib")

class Mesh {
public:
	~Mesh();
	void Initialize(int32_t width, int32_t height);
	void ResetDXC();
	void Update();
	IDxcBlob* CompileShader(
	    const std::wstring& filePath, const wchar_t* profile, IDxcUtils* dxcUtils,
	    IDxcCompiler3* dxcCompiler, IDxcIncludeHandler* includeHandler);
	void DrawTriangle(
	    Vector4 Top, Vector4 Right, Vector4 Left, Vector4 color, Vector2 coordTop,
	    Vector2 coordRight, Vector2 coordLeft, bool useWorldMap);
	void DrawSprite(
	    Vector4 LeftTop, Vector4 RightTop, Vector4 RightBottom, Vector4 LeftBottom, Vector4 color,
	    Vector2 coordLeftTop, Vector2 coordRightTop, Vector2 coordRightBottom,
	    Vector2 coordLeftBottom, int32_t width, int32_t height);
	void DrawSphere(
		const Sphere& sphere_, Vector4 color, bool useWorldMap, int32_t width, int32_t height);
	void DrawOBJ(
		Vector4 color, bool useWorldMap, int32_t width, int32_t height);

	void MakePSO();
	ID3D12Resource* CreateBufferResource(size_t sizeInBytes);
	void MakeBufferView();
	void InputDataTriangle(
	    Vector4 Top, Vector4 Right, Vector4 Left, Vector4 color, Vector2 coordTop,
	    Vector2 coordRight, Vector2 coordLeft);
	void InputDataSprite(
	    Vector4 LeftTop, Vector4 RightTop, Vector4 RightBottom, Vector4 LeftBottom, Vector4 color,
	    Vector2 coordLeftTop, Vector2 coordRightTop, Vector2 coordRightBottom,
	    Vector2 coordLeftBottom, int32_t width, int32_t height);
	void InputDataSphere(
		Vector4 LeftTop, Vector4 RightTop, Vector4 RightBottom, Vector4 LeftBottom, Vector4 color,
		Vector2 coordLeftTop, Vector2 coordRightTop, Vector2 coordRightBottom,
		Vector2 coordLeftBottom, uint32_t count, int32_t width, int32_t height);

	struct VertexData {
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};
	struct ModelData {
		std::vector<VertexData> vertices;
	};
	ModelData LoadObjFile(const std::string& directryPath, const std::string& filename);

	void MeshRelease();

	ID3D12Resource* GetVertexResource() { return vertexResource; }
	TransformMatrix GetCameraTransform() { return cameraTransform; }
	Matrix4x4 GetCameraMatrix() { return cameraMatrix; }

	struct DirectionalLight {
		Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Vector3 direction = { 0.0f, -1.0f, 0.0f };
		float intensity = 1.0f;
	};
	DirectionalLight* DrawDirectionalLightData() { return DirectionalLightData; }

private:
	Debug* debug_;
	WinAPP* sWinApp;
	MyImGui* imgui_;
	HRESULT hr = NULL;
	TransformMatrix transformMatrix;
	TransformMatrix transformMatrixSprite;
	TransformMatrix transformMatrixSphere;
	TransformMatrix transformMatrixObj;

	ID3D12Resource* transformationMatrixResource;
	ID3D12Resource* transformationMatrixResourceSprite;
	ID3D12Resource* transformationMatrixResourceSphere;
	ID3D12Resource* transformationMatrixResourceObj;

	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	IDxcIncludeHandler* includeHandler = nullptr;
	ID3D12RootSignature* rootSignature = nullptr;
	ID3D12PipelineState* graphicsPipelineState = NULL;
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_{};
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	IDxcBlob* pixelShaderBlob = nullptr;
	IDxcBlob* vertexShaderBlob = nullptr;

	ID3D12Resource* vertexResource = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	ID3D12Resource* vertexResourceSprite = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSprite{};

	ID3D12Resource* vertexResourceSphere = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSphere{};

	ID3D12Resource* vertexResourceObj = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferViewObj{};

	ID3D12Resource* indexResourceSprite = nullptr;
	D3D12_INDEX_BUFFER_VIEW indexBufferViewSprite{};

	ID3D12Resource* indexResourceSphere = nullptr;
	D3D12_INDEX_BUFFER_VIEW indexBufferViewSphere{};

	ID3D12Resource* indexResourceObj = nullptr;
	D3D12_INDEX_BUFFER_VIEW indexBufferViewObj{};

	D3D12_VIEWPORT viewport{};
	D3D12_RECT scissorRect{};
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	D3D12_BLEND_DESC blendDesc{};
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	ID3D12Resource* materialResource;
	ID3D12Resource* materialResourceSprite;
	ID3D12Resource* materialResourceSphere;
	ID3D12Resource* materialResourceObj;

	ID3D12Resource* directionalLightResource;
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};

	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};
	struct TransformationMatrix {
		Matrix4x4 WVP;
		Matrix4x4 World;
	};
	TransformMatrix uvTransformSprite{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};
	TransformMatrix uvTransformSphere{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};
	TransformMatrix uvTransformObj{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};

	TransformMatrix cameraTransform;
	DirectionalLight* DirectionalLightData = nullptr;
	TransformationMatrix* transformationMatrixDataSprite = nullptr;
	TransformationMatrix* transformationMatrixDataSphere = nullptr;
	TransformationMatrix* transformationMatrixDataObj = nullptr;

	Matrix4x4 cameraMatrix;
	Matrix4x4 viewMatrix;
	Matrix4x4 viewMatrixSprite;
	Matrix4x4 viewMatrixSphere;
	Matrix4x4 viewMatrixObj;

	Matrix4x4 projectionMatrix;
	Matrix4x4 projectionMatrixSprite;
	Matrix4x4 projectionMatrixSphere;
	Matrix4x4 projectionMatrixObj;

	Matrix4x4 worldViewProjectionMatrix;
	Matrix4x4 worldViewProjectionMatrixSprite;
	Matrix4x4 worldViewProjectionMatrixSphere;
	Matrix4x4 worldViewProjectionMatrixObj;

	const uint32_t kSubdivision = 16;
	ModelData modelData;

};
