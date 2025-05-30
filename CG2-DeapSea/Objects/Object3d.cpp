#include "Object3d.h"
#include "Commons/Object3dCommon.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

namespace MyEngine
{
	void Object3d::Initialize()
	{
		object3dCommon_ = Object3dCommon::GetInstance();
		srvManager_ = SRVManager::GetInstance();
		dx12Common_ = object3dCommon_->GetDx12Common();
		modelManager_ = ModelManager::GetInstance();

		transformationMatrixResource = CreateBufferResource(sizeof(TransformationMatrix));
		directionalLightResource = CreateBufferResource(sizeof(DirectionalLight));
		camera_ = object3dCommon_->GetDefaultCamera();
		cameraResource = CreateBufferResource(sizeof(CameraTransform));
		transformMatrix =
		{
		{1.0f, 1.0f, 1.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f}
		};

		transformationMatrixResource->Map(
			0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));
		directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&directionalLightData));
		cameraResource->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));
		directionalLightData->color = directionalLight.color;
		directionalLightData->direction = directionalLight.direction;
		directionalLightData->intensity = directionalLight.intensity;
	}

	void Object3d::Update(Camera* camera)
	{
		camera_ = camera;
		Matrix4x4 worldMatrix = MakeAffineMatrix(
			transformMatrix.scale, transformMatrix.rotate, transformMatrix.translate);

		if (camera_)
		{
			const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
			worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);
		}

		cameraData->worldPosition =
		{
			camera_->GetWorldMatrix().m[3][0],
			camera_->GetWorldMatrix().m[3][1],
			camera_->GetWorldMatrix().m[3][2]
		};
		directionalLightData->color = directionalLight.color;
		directionalLightData->direction = directionalLight.direction;
		directionalLightData->intensity = directionalLight.intensity;

		transformationMatrixData->WVP = Multiply(model_->GetModelData()->rootNode.localMatrix, worldViewProjectionMatrix);
		transformationMatrixData->World = Multiply(model_->GetModelData()->rootNode.localMatrix, worldMatrix);
		transformationMatrixData->WorldInverseTranspose = Transpose(Inverse(transformationMatrixData->World));
	}

	void Object3d::AnimationUpdate(Camera* camera)//処理に問題の可能性あり
	{
		camera_ = camera;
		Matrix4x4 worldMatrix = MakeAffineMatrix(
			transformMatrix.scale, transformMatrix.rotate, transformMatrix.translate);

		if (camera_)
		{
			const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
			worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);
		}

		cameraData->worldPosition =
		{
			camera_->GetWorldMatrix().m[3][0],
			camera_->GetWorldMatrix().m[3][1],
			camera_->GetWorldMatrix().m[3][2]
		};

		if (isAnimation == true)
		{
			animationTime_ += 1.0f / 60.0f;
		}
		animationTime_ = std::fmod(animationTime_, model_->GetAnimation().duration);
		ApplyAnimation(model_->GetSkelton(), model_->GetAnimation(), animationTime_);
		Model::NodeAnimation& rootNodeAnimation = model_->GetAnimation().nodeAnimations[model_->GetModelData()->rootNode.name];
		Vector3 translate = CalculatevalueV(rootNodeAnimation.translate.keyframes, animationTime_);
		Quaternion rotate = CalculatevalueQ(rootNodeAnimation.rotate.keyframes, animationTime_);
		Vector3 scale = CalculatevalueV(rootNodeAnimation.scale.keyframes, animationTime_);
		Matrix4x4 localMatrix = MakeAffineMatrix(scale, rotate, translate);

		transformationMatrixData->WVP = Multiply(localMatrix, worldViewProjectionMatrix);
		transformationMatrixData->World = Multiply(localMatrix, worldMatrix);
		transformationMatrixData->WorldInverseTranspose = Transpose(Inverse(transformationMatrixData->World));
	}

	void Object3d::SkeltonUpdate(Camera* camera)
	{
		Matrix4x4 worldMatrix = MakeAffineMatrix(
			transformMatrix.scale, transformMatrix.rotate, transformMatrix.translate);
		if (camera)
		{
			const Matrix4x4& viewProjectionMatrix = camera->GetViewProjectionMatrix();
			worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);
		}

		cameraData->worldPosition =
		{
			camera->GetWorldMatrix().m[3][0],
			camera->GetWorldMatrix().m[3][1],
			camera->GetWorldMatrix().m[3][2]
		};
		if (isAnimation == true)
		{
			skeltonAnimationTime += 1.0f / 60.0f;
		}
		//skeltonAnimationTime = 0.5f;
		skeltonAnimationTime = std::fmod(skeltonAnimationTime, model_->GetAnimation().duration);
		ApplyAnimation(model_->GetSkelton(), model_->GetAnimation(), skeltonAnimationTime);

		int32_t i = 0;
		for (Model::Joint& joint : model_->GetSkelton().joints)
		{
			joint.localMatrix = MakeAffineMatrix(joint.transform.scale, joint.transform.rotate, joint.transform.translate);

			i++;
			if (joint.parent)
			{
				joint.skeltonSpaceMatrix = Multiply(joint.localMatrix, model_->GetSkelton().joints[*joint.parent].skeltonSpaceMatrix);
			}
			else
			{
				joint.skeltonSpaceMatrix = joint.localMatrix;
			}
			joint.worldMatrix = Multiply(joint.skeltonSpaceMatrix, worldMatrix);
		}
		transformationMatrixData->WVP = worldViewProjectionMatrix;
		transformationMatrixData->World = worldMatrix;
		transformationMatrixData->WorldInverseTranspose = Transpose(Inverse(worldViewProjectionMatrix));

		for (size_t jointIndex = 0; jointIndex < model_->GetSkelton().joints.size(); jointIndex++)
		{
			assert(jointIndex < model_->GetSkinCluster().inverseBindPoseMatrices.size());

			model_->GetSkinCluster().mappedPalette[jointIndex].skeltonSpaceMatrix =
				Multiply(model_->GetSkinCluster().inverseBindPoseMatrices[jointIndex],
					model_->GetSkelton().joints[jointIndex].skeltonSpaceMatrix);

			model_->GetSkinCluster().mappedPalette[jointIndex].skeltonSpaceInverseTransposeMatrix =
				Transpose(Inverse(model_->GetSkinCluster().mappedPalette[jointIndex].skeltonSpaceMatrix));
		}

		//auto&& test = model_->GetSkinCluster();
		//(void)test;
	}

	void Object3d::Draw(ModelCommon* modelCommon)
	{
		modelCommon_ = modelCommon;

		dx12Common_->GetCommandList().Get()->
			SetPipelineState(object3dCommon_->GetGraphicsPipelineStates(0).Get());

		dx12Common_->GetCommandList().Get()->
			SetGraphicsRootSignature(object3dCommon_->GetRootSignatures(0).Get());

		dx12Common_->GetCommandList().Get()->
			IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		D3D12_CPU_DESCRIPTOR_HANDLE rtv = dx12Common_->GetRtvHandles(
			srvManager_->GetBackBufferIndex());

		D3D12_CPU_DESCRIPTOR_HANDLE dsv = dx12Common_->GetDsvHandle();
		dx12Common_->GetCommandList().Get()->
			OMSetRenderTargets(1, &rtv, false, &dsv);

		dx12Common_->GetCommandList().Get()->
			SetGraphicsRootConstantBufferView(
				1, transformationMatrixResource->GetGPUVirtualAddress());

		dx12Common_->GetCommandList().Get()->
			SetGraphicsRootConstantBufferView(
				3, directionalLightResource->GetGPUVirtualAddress());

		dx12Common_->GetCommandList()->
			SetGraphicsRootConstantBufferView(
				4, cameraResource->GetGPUVirtualAddress());

		if (model_)
		{
			model_->Draw(modelCommon_, srvManager_);
		}
	}

	void Object3d::SkeltonDraw(ModelCommon* modelCommon)
	{
		modelCommon_ = modelCommon;

		dx12Common_->GetCommandList().Get()->
			SetPipelineState(object3dCommon_->GetGraphicsPipelineStates(1).Get());

		dx12Common_->GetCommandList().Get()->
			SetGraphicsRootSignature(object3dCommon_->GetRootSignatures(1).Get());

		dx12Common_->GetCommandList().Get()->
			IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		D3D12_CPU_DESCRIPTOR_HANDLE rtv = dx12Common_->GetRtvHandles(
			srvManager_->GetBackBufferIndex());

		D3D12_CPU_DESCRIPTOR_HANDLE dsv = dx12Common_->GetDsvHandle();
		dx12Common_->GetCommandList().Get()->
			OMSetRenderTargets(1, &rtv, false, &dsv);

		dx12Common_->GetCommandList().Get()->
			SetGraphicsRootConstantBufferView(
				1, transformationMatrixResource->GetGPUVirtualAddress());

		dx12Common_->GetCommandList().Get()->
			SetGraphicsRootConstantBufferView(
				3, directionalLightResource->GetGPUVirtualAddress());

		dx12Common_->GetCommandList()->
			SetGraphicsRootConstantBufferView(
				4, cameraResource->GetGPUVirtualAddress());

		if (model_)
		{
			model_->SkeltonDraw(modelCommon_, srvManager_);
		}
	}

	ComPtr<ID3D12Resource> Object3d::CreateBufferResource(size_t sizeInBytes)
	{
		D3D12_HEAP_PROPERTIES uploadHeapProperties{};

		uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
		D3D12_RESOURCE_DESC ResourceDesc{};

		ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;

		ResourceDesc.Width = sizeInBytes;

		ResourceDesc.Height = 1;
		ResourceDesc.DepthOrArraySize = 1;
		ResourceDesc.MipLevels = 1;
		ResourceDesc.SampleDesc.Count = 1;

		ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

		ComPtr<ID3D12Resource> Resource = nullptr;

		hr = dx12Common_->GetDevice().Get()->CreateCommittedResource(
			&uploadHeapProperties,
			D3D12_HEAP_FLAG_NONE,
			&ResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&Resource));
		assert(SUCCEEDED(hr));
		return Resource;
	}

	Vector3 Object3d::CalculatevalueV(const std::vector<Model::KeyFrameVector3>& keyframes, float time)
	{
		assert(!keyframes.empty());//キーがないと止まる
		if (keyframes.size() == 1 || time <= keyframes[0].time)
		{
			return keyframes[0].value;
		}
		for (size_t index = 0; index < keyframes.size() - 1; ++index)
		{
			size_t nextIndex = index + 1;
			if (keyframes[index].time <= time && time <= keyframes[nextIndex].time)
			{
				float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
				return Lerp(keyframes[index].value, keyframes[nextIndex].value, t);
			}
		}
		return (*keyframes.rbegin()).value;
	}

	Quaternion Object3d::CalculatevalueQ(const std::vector<Model::KeyFrameQuaternion>& keyframes, float time)
	{
		assert(!keyframes.empty());//キーがないと止まる
		if (keyframes.size() == 1 || time <= keyframes[0].time)
		{
			return keyframes[0].value;
		}
		for (size_t index = 0; index < keyframes.size() - 1; ++index)
		{
			size_t nextIndex = index + 1;
			if (keyframes[index].time <= time && time <= keyframes[nextIndex].time)
			{
				float t = (time - keyframes[index].time) / (keyframes[nextIndex].time - keyframes[index].time);
				return Slerp(keyframes[index].value, keyframes[nextIndex].value, t);
			}
		}
		return (*keyframes.rbegin()).value;
	}

	void Object3d::ApplyAnimation(Model::Skelton& skelton, const Model::Animation& animation, float animationTime)
	{
		for (Model::Joint& joint : skelton.joints)
		{
			if (auto it = animation.nodeAnimations.find(joint.name); it != animation.nodeAnimations.end())
			{
				const Model::NodeAnimation& rootNodeAnimation = (*it).second;
				joint.transform.translate = CalculatevalueV(rootNodeAnimation.translate.keyframes, animationTime);
				joint.transform.rotate = CalculatevalueQ(rootNodeAnimation.rotate.keyframes, animationTime);
				joint.transform.scale = CalculatevalueV(rootNodeAnimation.scale.keyframes, animationTime);
			}
		}
	}

	void Object3d::SetModel(const std::string& filePath)
	{
		model_ = modelManager_->FindModel(filePath);
	}

    void Object3d::Revolution(Vector3 distance, Vector3 tergetTranslate)
	{
	    Matrix4x4 RotateMatrix = MakeRotateMatrix(transformMatrix.rotate);
	    Vector3 offset = TransformNormal(distance, RotateMatrix);
	    transformMatrix.translate = Add(tergetTranslate, offset);
    }


	void Object3d::SetColor(Vector4 color)
	{
		model_->SetColor(color);
	}
}
