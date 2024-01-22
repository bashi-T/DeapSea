#pragma once
#include "CGVector.h"
#include "DX12Common.h"
#include "MyImGui.h"
#include <cassert>
#include <dxcapi.h>
#include <fstream>
#include <sstream>
#include"TextureManager.h"

#pragma comment(lib, "dxcompiler.lib")

class SpriteCommon
{
public:
	~SpriteCommon();
	void Initialize(DX12Common* dxcommon);
	//void Update();
	//void Draw(int32_t width, int32_t height);
	ComPtr<IDxcBlob> CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile,
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);
	void ResetDXC();
	void MakePSO(DX12Common* dxcommon);
	ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes, DX12Common* dxcommon);

	//void InputDataTriangle(
	//	Vector4 Top, Vector4 Right, Vector4 Left, Vector4 color, Vector2 coordTop,
	//	Vector2 coordRight, Vector2 coordLeft);

	//void DrawTriangle(
	//	Vector4 Top, Vector4 Right, Vector4 Left, Vector4 color, Vector2 coordTop,
	//	Vector2 coordRight, Vector2 coordLeft, bool useWorldMap);

	void MakeShaderResourceView(const DirectX::TexMetadata& metadata, DX12Common* dxcommon);

	struct VertexData
	{
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};
	struct MaterialData
	{
		std::string textureFilePath;
	};
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	struct ModelData
	{
		std::vector<VertexData> vertices;
		MaterialData material;
	};

	void UploadTextureData(
		ID3D12Resource* texture,
		const DirectX::ScratchImage& mipImages,
		const DirectX::TexMetadata& metadata);

	//void MeshRelease();

	ComPtr<ID3D12Resource> GetVertexResource() { return vertexResource; }
	TransformMatrix GetCameraTransform() { return cameraTransform; }
	Matrix4x4 GetCameraMatrix() { return cameraMatrix; }
	ComPtr<ID3D12PipelineState> GetGraphicsPipelineState() { return graphicsPipelineState; }
	ComPtr<ID3D12RootSignature> GetRootSignature() { return rootSignature; }
	DX12Common* GetDx12Common() { return DX12Common::GetInstance(); }

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
	DX12Common* dx12Common_;
	TransformMatrix transformMatrix;
	TextureManager* textureManager_;
	ComPtr<ID3D12Resource> transformationMatrixResource;

	ComPtr<IDxcUtils> dxcUtils = nullptr;
	ComPtr<IDxcCompiler3> dxcCompiler = nullptr;
	ComPtr<IDxcIncludeHandler> includeHandler = nullptr;
	ComPtr<ID3D12RootSignature> rootSignature = nullptr;
	ComPtr<ID3D12PipelineState> graphicsPipelineState = NULL;
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_{};
	ComPtr<ID3DBlob> signatureBlob = nullptr;
	ComPtr<ID3DBlob> errorBlob = nullptr;
	ComPtr<IDxcBlob> pixelShaderBlob = nullptr;
	ComPtr<IDxcBlob> vertexShaderBlob = nullptr;

	ComPtr<ID3D12Resource> vertexResource = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	D3D12_BLEND_DESC blendDesc{};
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	ComPtr<ID3D12Resource> materialResource;

	ComPtr<ID3D12Resource> directionalLightResource;
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};

	struct Material 
	{
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};
	struct TransformationMatrix
	{
		Matrix4x4 WVP;
		Matrix4x4 World;
	};

	TransformMatrix cameraTransform;
	DirectionalLight* DirectionalLightData = nullptr;

	Matrix4x4 cameraMatrix;
	Matrix4x4 viewMatrix;

	Matrix4x4 projectionMatrix;

	Matrix4x4 ViewProjectionMatrix;

	Matrix4x4 worldViewProjectionMatrix;

	uint32_t kSubdivision = 16;
	ModelData modelData;

	ComPtr<ID3D12Resource> textureResource;
	ComPtr<ID3D12Resource> textureResource2;

	DirectX::ScratchImage mipImages;
	DirectX::ScratchImage mipImages2;
	const int32_t kNumTriangle = 1;

	Vector4 Top[1];
	Vector4 Left[1];
	Vector4 Right[1];
	Vector4 Color[1];
	Vector2 texcoordTop[1];
	Vector2 texcoordLeft[1];
	Vector2 texcoordRight[1];

};

