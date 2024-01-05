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
#include"SpriteCommon.h"

#pragma comment(lib, "d3d12.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "dxcompiler.lib")

class Mesh;
class Sprite
{
public:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	~Sprite();
	void Initialize(int32_t width, int32_t height, SpriteCommon* spriteCommon);
	void Update();
	void Draw();

	void ResetDXC();
	void MakePSO();
	ComPtr<IDxcBlob> CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile,
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler);


	ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);
	void MakeBufferView();
	void InputData(
		Vector4 LeftTop, Vector4 RightTop, Vector4 RightBottom, Vector4 LeftBottom, Vector4 color,
		Vector2 coordLeftTop, Vector2 coordRightTop, Vector2 coordRightBottom,
		Vector2 coordLeftBottom, int32_t width, int32_t height);
	//void MakeShaderResourceView(const DirectX::TexMetadata& metadata, const DirectX::TexMetadata& metadata2);

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
	//MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);

	struct ModelData
	{
		std::vector<VertexData> vertices;
		MaterialData material;
	};

	DirectX::ScratchImage LoadTexture(const std::string& filePath);
	ComPtr<ID3D12Resource> CreateTextureResource(
		ID3D12Device* device,
		const DirectX::TexMetadata& metadata);
	void UploadTextureData(
		ID3D12Resource* texture,
		const DirectX::ScratchImage& mipImages,
		const DirectX::TexMetadata& metadata);

	//void MeshRelease();

	ComPtr<ID3D12Resource> GetVertexResource() { return vertexResource; }
	TransformMatrix GetCameraTransform() { return cameraTransform; }
	Matrix4x4 GetCameraMatrix() { return cameraMatrix; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetTextureSrvHandleCPU() { return textureSrvHandleCPU; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureSrvHandleGPU() { return textureSrvHandleGPU; }
	Vector4 GetLeftTop(int i) { return LeftTop[i]; }
	Vector4 GetRightTop(int i) { return RightTop[i]; }
	Vector4 GetRightBottom(int i){ return RightBottom[i]; }
	Vector4 GetLeftBottom(int i){ return LeftBottom[i]; }
	Vector4 GetColor(int i){ return LeftTop[i]; }
	Vector2 GetTexcoordLeftTop(int i){ return coordLeftTop[i]; }
	Vector2 GetTexcoordRightTop(int i){ return coordRightTop[i]; }
	Vector2 GetTexcoordRightBottom(int i){ return coordRightBottom[i]; }
	Vector2 GetTexcoordLeftBottom(int i) { return coordLeftBottom[i]; }

	struct DirectionalLight {
		Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Vector3 direction = { 0.0f, -1.0f, 0.0f };
		float intensity = 1.0f;
	};
	DirectionalLight* DrawDirectionalLightData() { return DirectionalLightData; }

private:
	SpriteCommon* spriteCommon_ = nullptr;
	Debug* debug_;
	WinAPP* sWinApp;
	MyImGui* imgui_;
	SpriteCommon* spriteCom_;
	HRESULT hr = NULL;
	TransformMatrix transformMatrix;
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

	ComPtr<ID3D12Resource> indexResource = nullptr;
	D3D12_INDEX_BUFFER_VIEW indexBufferView{};

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	D3D12_BLEND_DESC blendDesc{};
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	ComPtr<ID3D12Resource> materialResource;

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
	TransformMatrix uvTransform{
		{1.0f,1.0f,1.0f},
		{0.0f,0.0f,0.0f},
		{0.0f,0.0f,0.0f},
	};
	Vector4 LeftTop[10];
	Vector4 RightTop[10];
	Vector4 RightBottom[10];
	Vector4 LeftBottom[10];
	Vector4 Color[10];
	Vector2 coordLeftTop[10];
	Vector2 coordRightTop[10];
	Vector2 coordRightBottom[10];
	Vector2 coordLeftBottom[10];


	TransformMatrix cameraTransform;
	DirectionalLight* DirectionalLightData = nullptr;
	TransformationMatrix* transformationMatrixData = nullptr;

	Matrix4x4 cameraMatrix;

	Matrix4x4 viewMatrix;

	Matrix4x4 projectionMatrix;

	Matrix4x4 ViewProjectionMatrix;

	Matrix4x4 worldViewProjectionMatrix;

	ComPtr<ID3D12Resource> textureResource;

	DirectX::ScratchImage mipImages;
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;


};

