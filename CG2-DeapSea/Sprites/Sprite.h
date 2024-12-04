#pragma once
#include "Math/Collision.h"
#include "Commons/DX12Common.h"
#include "Systems/MyImGui.h"
#include <cassert>
#include <dxcapi.h>
#include <fstream>
#include <sstream>
#include"Managers/TextureManager.h"

#pragma comment(lib, "dxcompiler.lib")
namespace MyEngine

{
	class SpriteCommon;
	class Sprite
	{
	public:
		template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
		~Sprite();
		void Initialize(
			SpriteCommon* spriteCommon,
			SRVManager* srvManager,
			std::string texturefilePath);
		void Update();
		void Draw();
		void Finalize();

		ComPtr<ID3D12Resource> CreateBufferResource(SpriteCommon* spriteCommon, size_t sizeInBytes);
		void MakeBufferView();
		void InputData(Vector4 color);

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
		ComPtr<ID3D12Resource> CreateTextureResource(
			ID3D12Device* device,
			const DirectX::TexMetadata& metadata);
		void SetPositoin(const Vector2& Position) { position = Position; }
		void SetRotation(float Rotation) { rotation = Rotation; }
		void SetColor(const Vector4& color) { materialData->color = color; }
		void SetSize(const Vector2& Size) { size = Size; }
		void SetAnchorPoint(const Vector2& AnchorPoint) { anchorPoint = AnchorPoint; }
		void SetIsFlipX(const bool& isFlipX) { this->isFlipX_ = isFlipX; }
		void SetIsFlipY(const bool& isFlipY) { this->isFlipY_ = isFlipY; }
		void SetTextureLeftTop(const Vector2& texturelefttop) { this->textureLeftTop = texturelefttop; }
		void SetTextureSize(const Vector2& texturesize) { this->textureSize = texturesize; }
		void AdjestTextureSize();

		ComPtr<ID3D12Resource> GetVertexResource() { return vertexResource; }
		const EulerTransform& GetCameraTransform()const { return cameraTransform; }
		const Matrix4x4& GetCameraMatrix()const { return cameraMatrix; }
		const Vector4& GetLeftTop()const { return LeftTop; }
		const Vector4& GetRightTop()const { return RightTop; }
		const Vector4& GetRightBottom()const { return RightBottom; }
		const Vector4& GetLeftBottom()const { return LeftBottom; }
		const Vector2& GetTexcoordLeftTop()const { return coordLeftTop; }
		const Vector2& GetTexcoordRightTop()const { return coordRightTop; }
		const Vector2& GetTexcoordRightBottom()const { return coordRightBottom; }
		const Vector2& GetTexcoordLeftBottom()const { return coordLeftBottom; }
		const Vector2& GetPosition()const { return position; }
		const float& GetRotation()const { return rotation; }
		const Vector4& GetColor()const { return materialData->color; }
		const Vector2& GetSize()const { return size; }
		const Vector2& GetAnchorPoint()const { return anchorPoint; }
		const bool& GetIsFlipX()const { return isFlipX_; }
		const bool& GetIsFlipY()const { return isFlipY_; }
		const Vector2& GetTextureLeftTop()const { return textureLeftTop; }
		const Vector2& GetTextureSize()const { return textureSize; }

		struct DirectionalLight {
			Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
			Vector3 direction = { 0.0f, -1.0f, 0.0f };
			float intensity = 1.0f;
		};
		DirectionalLight* DrawDirectionalLightData() { return DirectionalLightData; }

	private:
		SpriteCommon* spriteCommon_;
		HRESULT hr = NULL;
		uint32_t textureIndex = 0;
		SRVManager* srvManager_ = nullptr;

		EulerTransform transformMatrix;
		Vector2 position = { 0.0f,0.0f };
		float rotation = 0.0f;
		Vector2 size = { 100.0f,100.0f };
		Vector2 anchorPoint = { 0.0f,0.0f };

		VertexData* vertexData = nullptr;
		ComPtr<ID3D12Resource> vertexResource = nullptr;
		D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

		uint32_t* indexData = nullptr;
		ComPtr<ID3D12Resource> indexResource = nullptr;
		D3D12_INDEX_BUFFER_VIEW indexBufferView{};

		struct Material
		{
			Vector4 color;
			Matrix4x4 uvTransform;
			MaterialData material;
		};
		Material* materialData = nullptr;
		ComPtr<ID3D12Resource> materialResource = nullptr;

		struct TransformationMatrix {
			Matrix4x4 WVP;
			Matrix4x4 World;
		};
		EulerTransform uvTransform{
			{1.0f,1.0f,1.0f},
			{0.0f,0.0f,0.0f},
			{0.0f,0.0f,0.0f},
		};
		ComPtr<ID3D12Resource> transformationMatrixResource;
		TransformationMatrix* transformationMatrixData = nullptr;

		Vector4 LeftTop;
		Vector4 RightTop;
		Vector4 RightBottom;
		Vector4 LeftBottom;
		Vector2 coordLeftTop;
		Vector2 coordRightTop;
		Vector2 coordRightBottom;
		Vector2 coordLeftBottom;

		EulerTransform cameraTransform = {};
		DirectionalLight* DirectionalLightData = nullptr;

		Matrix4x4 cameraMatrix = {};

		Matrix4x4 viewMatrix = {};

		Matrix4x4 projectionMatrix = {};

		Matrix4x4 ViewProjectionMatrix = {};

		Matrix4x4 worldViewProjectionMatrix = {};

		ComPtr<ID3D12Resource> textureResource;

		DirectX::ScratchImage mipImages;
		D3D12_CPU_DESCRIPTOR_HANDLE textureSrvHandleCPU;
		D3D12_GPU_DESCRIPTOR_HANDLE textureSrvHandleGPU;

		bool isFlipX_ = false;
		bool isFlipY_ = false;
		Vector2 textureLeftTop = { 0.0f,0.0f };
		Vector2 textureSize = { 100.0f,100.0f };
	};
}

