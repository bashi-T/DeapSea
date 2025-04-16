#pragma once
#include "Math/Collision.h"
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

namespace MyEngine
{
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

		struct RevolveDistance
		{
		    float distancex;
		    float distancey;
		    float distancez;
		};
	    RevolveDistance revolveDistance;
		/// <summary>
		/// 初期化
		/// </summary>
		/// <param name="filename"></param>
		/// <param name="particleCommon"></param>
		/// <param name="srvManager"></param>
		/// <param name="object3dCommon"></param>
		/// <param name="elements"></param>
		/// <param name="numMaxInstance"></param>
		void Initialize(const std::string& filename, ElementsParticle elements, int32_t numMaxInstance);
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
		void Update(bool isRevive, ElementsParticle elements);
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
		ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);
		/// <summary>
		/// bufferViewの生成
		/// </summary>
		void MakeBufferView();
		/// <summary>
		/// particleのデータを送る
		/// </summary>
		/// <param name="isRevive"></param>
		/// <param name="elements"></param>
		void InputData(bool isRevive, ElementsParticle elements);
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
		struct CameraTransform
		{
			Vector3 worldPosition;
		};

		CameraTransform* cameraData = nullptr;

		/// <summary>
		/// particleの要素を細かく設定　
		/// </summary>
		/// <param name="randomEngine"></param>
		/// <param name="colorMin">particleの色の最小値</param>
		/// <returns></returns>
		Particles MakeNewParticle(std::mt19937& randomEngine,ElementsParticle element);
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
	    /// 公転用の距離設定
	    /// </summary>
	    /// <param name="distanceX"></param>
	    /// <param name="distanceY"></param>
	    /// <param name="distanceZ"></param>
	    void SetDistance(float distanceX, float distanceY, float distanceZ);
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
		/// <summary>
		/// 赤色調整
		/// </summary>
		/// <param name="red"></param>
		void SetColorRed(float red);
	    /// <summary>
	    /// 公転
	    /// </summary>
	    /// <param name="distance"></param>
	    /// <param name="tergetTranslate"></param>
	    void Revolution(RevolveDistance distance, Vector3 tergetTranslate);

		const ElementsParticle& GetElements()const { return elements; }
		const Matrix4x4& GetCameraMatrix()const { return cameraMatrix; }
		const D3D12_CPU_DESCRIPTOR_HANDLE& GetTextureSrvHandleCPU()const { return textureSrvHandleCPU; }
		const D3D12_GPU_DESCRIPTOR_HANDLE& GetTextureSrvHandleGPU()const { return textureSrvHandleGPU; }
		ParticleForGPU* GetInstancingData() { return instancingData; }
		Particles* GetParticles() { return particles; }
	    const RevolveDistance GetDistance() { return revolveDistance; }
	    const uint32_t GetNumMaxInstance() { return kNumMaxInstance; }
	    EulerTransform GetTransform(int index) { return particles[index].transform; }
	private:

		SRVManager* srvManager_ = nullptr;
		Object3dCommon* object3dCommon_ = nullptr;
		ParticleCommon* particleCommon_;
		TextureManager* textureManager_;
		DX12Common* dx12Common_ = nullptr;
		Camera* camera_ = nullptr;
		HRESULT hr = NULL;

		uint32_t kNumInstance = 10;
		uint32_t kNumMaxInstance = 10;
		ComPtr<ID3D12Resource> cameraResource;

		Particles particles[1000];

		ComPtr<ID3D12Resource> transformationMatrixResource;

		ComPtr<ID3D12Resource> instancingResource;

		ComPtr<ID3D12Resource> vertexResource = nullptr;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
		ComPtr<ID3D12Resource> indexResource = nullptr;
		D3D12_INDEX_BUFFER_VIEW indexBufferView{};
		ComPtr<ID3D12Resource> colorResource;

		ComPtr<ID3D12Resource> directionalLightResource = nullptr;
		D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};

		Matrix4x4 cameraMatrix{};

		Matrix4x4 viewMatrix{};
	    Matrix4x4 projectionMatrix{};
	    Matrix4x4 ViewProjectionMatrix{};
	    Matrix4x4 worldViewProjectionMatrix{};

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

		D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;

		D3D12_CPU_DESCRIPTOR_HANDLE instancingSrvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE instancingSrvHandleGPU;

		Vector4 ColorSphere[1];

		Sphere sphere = { { 0.0f,0.0f,0.0f },1.0f };

		Vector4 LeftTop{};
		Vector4 RightTop{};
		Vector4 RightBottom{};
		Vector4 LeftBottom{};
		Vector4 Color{};
		Vector2 texcoordLeftTop{};
		Vector2 texcoordLeftBottom{};
		Vector2 texcoordRightTop{};
		Vector2 texcoordRightBottom{};
	    Vector3 distance{};

		const float kDeltaTime = 1.0f / 60.0f;
		Vector2 anchorPoint = { 0.0f,0.0f };
		Vector2 textureSize = { 100.0f,100.0f };
		bool isFlipX_ = false;
		bool isFlipY_ = false;

	};
}
