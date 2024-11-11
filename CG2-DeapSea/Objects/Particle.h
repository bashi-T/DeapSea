#pragma once
#include "Math/CGVector.h"
#include "Commons/DX12Common.h"
#include "Systems/MyImGui.h"
#include <cassert>
#include <dxcapi.h>
#include <fstream>
#include <sstream>
#include "Commons/Object3dCommon.h"
#include"Managers/TextureManager.h"

#include <random>
#include<numbers>
#pragma comment(lib, "dxcompiler.lib")

/// <summary>
/// particle本機能
/// </summary>

class ParticleCommon;
class Particle
{
public:
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	~Particle();
	struct ElementsParticle
	{
		float colorMin;
		float colorMax;
		float timeMin;
		float timeMax;
		float posxMin;
		float posxMax;
		float posyMin;
		float posyMax;
		float poszMin;
		float poszMax;
		float velxMin;
		float velxMax;
		float velyMin;
		float velyMax;
		float velzMin;
		float velzMax;
	};
	ElementsParticle elements;
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="filename"></param>
	/// <param name="particleCommon"></param>
	/// <param name="srvManager"></param>
	/// <param name="object3dCommon"></param>
	/// <param name="elements"></param>
	void Initialize(const std::string& filename, ParticleCommon* particleCommon, SRVManager* srvManager, Object3dCommon* object3dCommon, ElementsParticle elements);
	/// <summary>
	/// particleを生成
	/// </summary>
	/// <param name="elements"></param>
	void RandomInitialize(ElementsParticle elements);
	/// <summary>
	/// 更新
	/// </summary>
	/// <param name="isRevive"></param>
	/// <param name="elements"></param>
	void Update(bool isRevive,ElementsParticle elements);
	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	/// <summary>
	/// bufferResourceの生成
	/// </summary>
	/// <param name="particleCommon"></param>
	/// <param name="sizeInBytes"></param>
	/// <returns></returns>
	ComPtr<ID3D12Resource> CreateBufferResource(ParticleCommon* particleCommon, size_t sizeInBytes);
	/// <summary>
	/// bufferViewの生成
	/// </summary>
	void MakeBufferView();
	/// <summary>
	/// particleのデータを送る
	/// </summary>
	/// <param name="isRevive"></param>
	/// <param name="elements"></param>
	void InputData(bool isRevive,ElementsParticle elements);
	/// <summary>
	/// insatnce描画用のSRV生成
	/// </summary>
	void MakeShaderResourceViewInstance();

	struct VertexData
	{
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};
	struct MaterialData
	{
		std::string textureFilePath;
		uint32_t textureIndex = 0;
	};
	struct ModelData
	{
		std::vector<VertexData> vertices;
		MaterialData material;
	};
	ModelData modelData;
	struct Material
	{
		Vector4 color;
	};
	struct TransformationMatrix
	{
		Matrix4x4 WVP;
		Matrix4x4 World;
	};
	struct Particles
	{
		EulerTransform transform;
		Vector3 velocity;
		Vector4 color;
		float lifeTime;
		float currentTime;
	};
	struct ParticleForGPU
	{
		Matrix4x4 WVP;
		Matrix4x4 World;
		Vector4 color;
	};
	struct CameraTransform {
		Vector3 worldPosition;
	};

	CameraTransform* cameraData = nullptr;

	/// <summary>
	/// particleの色と出現時間を細かく設定
	/// </summary>
	/// <param name="randomEngine"></param>
	/// <param name="colorMin">particleの色の最小値</param>
	/// <param name="colorMax">particleの色の最大値</param>
	/// <param name="timeMin">particleの出現時間の最小値</param>
	/// <param name="timeMax">particleの出現時間の最大値</param>
	/// <returns></returns>
	Particles MakeNewParticle(std::mt19937& randomEngine, float colorMin, float colorMax, float timeMin, float timeMax);
	/// <summary>
	/// particleの座標関連の要素を細かく設定　
	/// </summary>
	/// <param name="randomEngine"></param>
	/// <param name="posxMin">particleが発生するX座標の最小値</param>
	/// <param name="posxMax">particleが発生するX座標の最大値</param>
	/// <param name="posyMin">particleが発生するY座標の最小値</param>
	/// <param name="posyMax">particleが発生するY座標の最大値</param>
	/// <param name="poszMin">particleが発生するZ座標の最小値</param>
	/// <param name="poszMax">particleが発生するZ座標の最大値</param>
	/// <param name="velxMin">particleが移動するX方向ベクトルの最小値</param>
	/// <param name="velxMax">particleが移動するX方向ベクトルの最大値</param>
	/// <param name="velyMin">particleが移動するY方向ベクトルの最小値</param>
	/// <param name="velyMax">particleが移動するY方向ベクトルの最大値</param>
	/// <param name="velzMin">particleが移動するZ方向ベクトルの最小値</param>
	/// <param name="velzMax">particleが移動するZ方向ベクトルの最大値</param>
	/// <returns></returns>
	Particles MakeNewParticlePosition(std::mt19937& randomEngine,
		float posxMin, float posxMax, float posyMin, float posyMax, float poszMin, float poszMax,
		float velxMin, float velxMax, float velyMin, float velyMax, float velzMin, float velzMax);
	/// <summary>
	/// ElementsParticleに任意の要素を入れる
	/// </summary>
	/// <param name="colorMin"></param>
	/// <param name="colorMax"></param>
	/// <param name="timeMin"></param>
	/// <param name="timeMax"></param>
	/// <param name="posxMin"></param>
	/// <param name="posxMax"></param>
	/// <param name="posyMin"></param>
	/// <param name="posyMax"></param>
	/// <param name="poszMin"></param>
	/// <param name="poszMax"></param>
	/// <param name="velxMin"></param>
	/// <param name="velxMax"></param>
	/// <param name="velyMin"></param>
	/// <param name="velyMax"></param>
	/// <param name="velzMin"></param>
	/// <param name="velzMax"></param>
	void SetElements(float colorMin, float colorMax, float timeMin, float timeMax,
		float posxMin, float posxMax, float posyMin, float posyMax, float poszMin, float poszMax,
		float velxMin, float velxMax, float velyMin, float velyMax, float velzMin, float velzMax);

	/// <summary>
	/// particleの拡縮(一律)
	/// </summary>
	/// <param name="scale"></param>
	void SetScale(Vector3 scale);
	/// <summary>
	/// particleの回転(一律)
	/// </summary>
	/// <param name="rotate"></param>
	void SetRotate(Vector3 rotate);

	ElementsParticle GetElements() { return elements; }
	Matrix4x4 GetCameraMatrix() { return cameraMatrix; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetTextureSrvHandleCPU() { return textureSrvHandleCPU; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureSrvHandleGPU() { return textureSrvHandleGPU; }
	D3D12_CPU_DESCRIPTOR_HANDLE GetTextureSrvHandleCPU2() { return textureSrvHandleCPU2; }
	D3D12_GPU_DESCRIPTOR_HANDLE GetTextureSrvHandleGPU2() { return textureSrvHandleGPU2; }
	ParticleForGPU* GetInstancingData() { return instancingData; }
	Particles* GetParticles() { return particles; }
private:
	SRVManager* srvManager_ = nullptr;
	DX12Common* dx12Common_ = nullptr;
	Object3dCommon* object3dCommon_ = nullptr;
	ParticleCommon* particleCommon_;
	Camera* camera_ = nullptr;
	DX12Common* dxCommon = nullptr;
	HRESULT hr = NULL;

	uint32_t kNumInstance = 10;
	uint32_t kNumMaxInstance = 10;
	ComPtr<ID3D12Resource> cameraResource;

	Particles particles[1000];

	ComPtr<ID3D12Resource> transformationMatrixResource;

	ComPtr<ID3D12Resource> instancingResource;

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
	ComPtr<ID3D12Resource> colorResource;

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	D3D12_BLEND_DESC blendDesc{};
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};

	ComPtr<ID3D12Resource> directionalLightResource = nullptr;
	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};

	Matrix4x4 cameraMatrix;

	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 ViewProjectionMatrix;
	Matrix4x4 worldViewProjectionMatrix;

	uint32_t kSubdivision = 16;

	ComPtr<ID3D12Resource> textureResource = nullptr;
	ComPtr<ID3D12Resource> textureResource2 = nullptr;

	uint32_t textureIndex;

	VertexData* vertexData = nullptr;
	MaterialData materialData;
	Material* colorData = nullptr;
	TransformationMatrix* transformationMatrixData = nullptr;
	ParticleForGPU* instancingData = nullptr;

	DirectX::ScratchImage mipImages;
	DirectX::ScratchImage mipImages2;
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;
	D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU2;
	D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU2;

	D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU;
	D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU;

	Vector4 ColorSphere[1];

	Sphere sphere = { { 0.0f,0.0f,0.0f },1.0f };

	Vector4 LeftTop;
	Vector4 RightTop;
	Vector4 RightBottom;
	Vector4 LeftBottom;
	Vector4 Color;
	Vector2 texcoordLeftTop;
	Vector2 texcoordLeftBottom;
	Vector2 texcoordRightTop;
	Vector2 texcoordRightBottom;

	const float kDeltaTime = 1.0f / 60.0f;
	Vector2 anchorPoint = { 0.0f,0.0f };
	Vector2 textureSize = { 100.0f,100.0f };
	bool isFlipX_ = false;
	bool isFlipY_ = false;

};
