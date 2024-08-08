//#pragma once
//#include "Math/CGVector.h"
//#include "Commons/DX12Common.h"
//#include"SRVManager.h"
//#include <cassert>
//#include <dxcapi.h>
//#include <fstream>
//#include <sstream>
//#include<unordered_map>
//#include<string>
//#include "externals/DirectXTex/DirectXTex.h"
//#include<WRL.h>
//#include"TextureManager.h"
//#include"Camera/Camera.h"
//#include "Commons/Object3dCommon.h"
//
//#include <random>
//
//#pragma comment(lib, "d3d12.lib")
//#pragma comment(lib, "dxgi.lib")
//#pragma comment(lib, "dxcompiler.lib")
//
//
//class ParticleManager
//{
//public:
//	struct VertexData
//	{
//		Vector4 position;
//		Vector2 texcoord;
//		Vector3 normal;
//	};
//	struct MaterialData
//	{
//		std::string textureFilePath;
//		uint32_t textureIndex = 0;
//	};
//	struct Material
//	{
//		Vector4 color;
//		bool enableLighting;
//		float padding[3];
//		Matrix4x4 uvTransform;
//	};
//	struct TransformationMatrix
//	{
//		Matrix4x4 WVP;
//		Matrix4x4 World;
//	};
//	struct Particles
//	{
//		TransformMatrix transform;
//		Vector3 velocity;
//		Vector4 color;
//		float lifeTime;
//		float currentTime;
//	};
//	struct ParticleForGPU
//	{
//		Matrix4x4 WVP;
//		Matrix4x4 World;
//		Vector4 color;
//	};
//	struct ParticleGroup {
//		MaterialData materialData;
//		std::list<Particles>particle[10];
//		uint32_t srvIndex;
//		ComPtr<ID3D12Resource> instancingResource;
//		uint32_t kNumInstance;
//		ParticleForGPU* instancingData;
//	};
//	std::unordered_map<std::string, ParticleGroup>particleGroups;
//	std::list<Particles> MakeNewParticle(std::mt19937& randomEngine);
//	void Initialize(DX12Common* dxCommon, SRVManager* srvManager, const std::string& particlename, const std::string& filename);
//	void Update(const std::string& particlename);
//	void Draw(ParticleGroup* particle);
//	void ResetDXC();
//	void MakePSO(DX12Common* dxCommon);
//	void CreateParticleGroup(const std::string name, const std::string textureFilePath);
//	ComPtr<IDxcBlob> CompileShader(
//		const std::wstring& filePath,
//		const wchar_t* profile,
//		IDxcUtils* dxcUtils,
//		IDxcCompiler3* dxcCompiler,
//		IDxcIncludeHandler* includeHandler);
//	ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);
//	void InputData(
//		Vector4 TopLeft, Vector4 TopRight, Vector4 BottomRight, Vector4 BottomLeft, Vector4 color,
//		Vector2 coordTopLeft, Vector2 coordTopRight, Vector2 coordBottomRight, Vector2 coordBottomLeft, const std::string& particlename);
//
//	void MakeBufferView();
//
//private:
//	DX12Common* dx12Common_;
//	Object3dCommon* object3dCommon_;
//	SRVManager* srvManager_ = nullptr;
//	Debug* debug_;
//	WinAPP* sWinApp;
//	MyImGui* imgui_;
//	Camera* camera_;
//
//	HRESULT hr = NULL;
//	ComPtr<IDxcUtils> dxcUtils = nullptr;
//	ComPtr<IDxcCompiler3> dxcCompiler = nullptr;
//	ComPtr<IDxcIncludeHandler> includeHandler = nullptr;
//	ComPtr<ID3D12RootSignature> rootSignature = nullptr;
//	ComPtr<ID3D12PipelineState> graphicsPipelineState = NULL;
//	ComPtr<ID3DBlob> signatureBlob = nullptr;
//	ComPtr<ID3DBlob> errorBlob = nullptr;
//	uint32_t kNumMaxInstance = 10;
//	
//	Vector4 LeftTop[1];
//	Vector4 RightTop[1];
//	Vector4 RightBottom[1];
//	Vector4 LeftBottom[1];
//	Vector4 Color[1];
//	Vector2 texcoordLeftTop[1];
//	Vector2 texcoordLeftBottom[1];
//	Vector2 texcoordRightTop[1];
//	Vector2 texcoordRightBottom[1];
//
//	ComPtr<ID3D12Resource> vertexResource = nullptr;
//	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
//	ComPtr<ID3D12Resource> indexResource = nullptr;
//	D3D12_INDEX_BUFFER_VIEW indexBufferView{};
//	ComPtr<ID3D12Resource> materialResource;
//	TransformMatrix uvTransform
//	{
//		{1.0f,1.0f,1.0f},
//		{0.0f,0.0f,0.0f},
//		{0.0f,0.0f,0.0f},
//	};
//	Matrix4x4 viewMatrix;
//	Matrix4x4 projectionMatrix;
//	Matrix4x4 ViewProjectionMatrix;
//	Matrix4x4 worldViewProjectionMatrix;
//	VertexData* vertexData = nullptr;
//	Material* materialData = nullptr;
//	TransformationMatrix* transformationMatrixData = nullptr;
//
//	const float kDeltaTime = 1.0f / 60.0f;
//	Vector2 anchorPoint = { 0.0f,0.0f };
//	Vector2 textureSize = { 100.0f,100.0f };
//	bool isFlipX_ = false;
//	bool isFlipY_ = false;
//
//	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU;
//	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU;
//
//};
//
