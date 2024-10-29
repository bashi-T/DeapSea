#pragma once
#include "Math/CGVector.h"
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

class Object3dCommon;
class Object3d
{
public:
	//~Object3d();
	void Initialize(Object3dCommon* object3dCommon, SRVManager* srvManager);
	void Update(Camera* camera);
	void SkeltonUpdate(Camera* camera);
	void AnimationUpdate(Camera* camera);
	void Draw(ModelCommon* modelCommon);
	void SkeltonDraw(ModelCommon* modelCommon);

	ComPtr<ID3D12Resource> CreateBufferResource(Object3dCommon* object3dCommon, size_t sizeInBytes);
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

	struct DirectionalLight
	{
		Vector4 color;
		Vector3 direction;
		float intensity;
	};
	DirectionalLight* GetDirectionalLightData() { return directionalLightData; }
	DirectionalLight& GetDirectionalLight() { return directionalLight; }
	void SetDirectionalLight(Vector4 color, Vector3 direction, float intensity) {
		directionalLight.color = color; 
		directionalLight.direction = direction;
		directionalLight.intensity = intensity;
	}
	Vector4 GetColor() { return model_->GetMaterialData()->color; }
	void SetColor(Vector4 color);

private:
	Object3dCommon* object3dCommon_ = nullptr;
	HRESULT hr = NULL;
	Model* model_ = nullptr;
	ModelCommon* modelCommon_ = nullptr;
	SRVManager* srvManager_ = nullptr;
	Camera* camera_ = nullptr;
	
	//Model::Animation& animation;
	float animationTime_ = 0.0f;
	float skeltonAnimationTime = 0.0f;

	DirectionalLight* directionalLightData = nullptr;
	DirectionalLight directionalLight =
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

	EulerTransform transformMatrix;
	Matrix4x4 worldViewProjectionMatrix;

	Matrix4x4 skeltonSpaceMatrix;

	ComPtr<ID3D12Resource> directionalLightResource;

	bool isAnimation = false;
};

