#pragma once
#include "Math/collision.h"
#include "Commons/DX12Common.h"
#include "Systems/MyImGui.h"
#include <cassert>
#include <dxcapi.h>
#include <fstream>
#include <sstream>
#include"Managers/TextureManager.h"
#include"Model.h"
#include"Managers/ModelManager.h"
#include"Camera/Camera.h"

#pragma comment(lib, "dxcompiler.lib")

namespace MyEngine
{
	class Object3dCommon;
	class Object3d
	{
	public:
		void Initialize();
		void Update(Camera* camera);
		void SkeltonUpdate(Camera* camera);
		void AnimationUpdate(Camera* camera);
		void Draw();
		void SkeltonDraw();

		ComPtr<ID3D12Resource> CreateBufferResource(size_t sizeInBytes);
		Vector3 CalculatevalueV(const std::vector<Model::KeyFrameVector3>& keyframes, float time);
		Quaternion CalculatevalueQ(const std::vector<Model::KeyFrameQuaternion>& keyframes, float time);
		void ApplyAnimation(Model::Skelton& skelton, const Model::Animation& animation, float animationTime);

		void SetModel(const std::string& filePath);
		void SetModel(Model* model) { this->model_ = model; }
		void SetScale(const Vector3& scale) { transformMatrix.scale = scale; }
		void SetRotate(const Vector3& rotate) { transformMatrix.rotate = rotate; }
		void SetTranslate(const Vector3& translate) { transformMatrix.translate = translate; }
		void SetCamera(Camera* cameras) { camera_ = cameras; }
		bool SetIsAnimation(bool IsAnimation) { return isAnimation = IsAnimation; }

		const Vector3& GetScale()const { return transformMatrix.scale; }
		const Vector3& GetRotate()const { return transformMatrix.rotate; }
		const Vector3& GetTranslate()const { return transformMatrix.translate; }
		const Matrix4x4& GetObjectMatrix()const { return transformationMatrixData->World; }

		struct DirectionalLight
		{
			Vector4 color;
			Vector3 direction;
			float intensity;
		};
		DirectionalLight* GetDirectionalLightData() { return directionalLightData; }
		const DirectionalLight& GetDirectionalLight()const { return directionalLight; }
		void SetDirectionalLight(Vector4 color, Vector3 direction, float intensity)
		{
			directionalLight.color = color;
			directionalLight.direction = direction;
			directionalLight.intensity = intensity;
		}

		struct PointLight
		{
			Vector4 color;
			Vector3 direction;
			float intensity;
		};
		PointLight* GetPointLightData() { return pointLightData; }
		const PointLight& GetPointLight()const { return pointLight; }
		void SetPointLight(Vector4 color, Vector3 direction, float intensity)
		{
			pointLight.color = color;
			pointLight.direction = direction;
			pointLight.intensity = intensity;
		}

		std::shared_ptr<Object3d>GetInstance();
		const Vector4& GetColor()const { return model_->GetMaterial()->color; }
		void SetColor(Vector4 color);

	private:
		HRESULT hr = NULL;
		std::shared_ptr<DX12Common> dx12Common_ = nullptr;
		std::shared_ptr<Object3dCommon> object3dCommon_ = nullptr;
		std::shared_ptr<ModelCommon> modelCommon_ = nullptr;
		std::shared_ptr<SRVManager> srvManager_ = nullptr;
		Model* model_ = nullptr;
		Camera* camera_ = nullptr;

		float animationTime_ = 0.0f;
		float skeltonAnimationTime = 0.0f;

		DirectionalLight* directionalLightData = nullptr;
		DirectionalLight directionalLight =
		{
			{1.0f,1.0f,1.0f,1.0f},
			{0.0f,-1.0f,0.0f},
			1.0f
		};
		PointLight* pointLightData = nullptr;
		PointLight pointLight =
		{
			{0.5f,0.5f,0.5f,1.0f},
			{0.0f,-1.0f,0.0f},
			1.0f
		};

		ComPtr<ID3D12Resource> transformationMatrixResource;
		ComPtr<ID3D12Resource> cameraResource;

		struct TransformationMatrix
		{
			Matrix4x4 WVP;
			Matrix4x4 World;
			Matrix4x4 WorldInverseTranspose;
		};

		struct CameraTransform
		{
			Vector3 worldPosition;
		};

		TransformationMatrix* transformationMatrixData = nullptr;
		CameraTransform* cameraData = nullptr;

		EulerTransform transformMatrix = {};
		Matrix4x4 worldViewProjectionMatrix = {};

		Matrix4x4 skeltonSpaceMatrix = {};

		ComPtr<ID3D12Resource> directionalLightResource;

		bool isAnimation = false;
	};
}

