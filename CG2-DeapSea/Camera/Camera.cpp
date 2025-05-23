#include "Camera.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

namespace MyEngine
{
	Camera::Camera()
	{
	}

	Camera::~Camera()
	{
	}

	void Camera::Initialize()
	{
		transformMatrix = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
		fovY = 0.45f;
		aspectRetio = float(WinAPP::clientWidth_) / float(WinAPP::clientHeight_);
		nearClip = 0.1f;
		farClip = 200.0f;
		worldMatrix = MakeAffineMatrix(transformMatrix.scale, transformMatrix.rotate, transformMatrix.translate);
		viewMatrix = Inverse(worldMatrix);
		projectionMatrix = MakePerspectiveFovMatrix(fovY, aspectRetio, nearClip, farClip);
		viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
	}

	void Camera::Update()
	{
		worldMatrix = MakeAffineMatrix(transformMatrix.scale, transformMatrix.rotate, transformMatrix.translate);
		viewMatrix = Inverse(worldMatrix);
		projectionMatrix =
			MakePerspectiveFovMatrix(fovY, aspectRetio, nearClip, farClip);
		viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
	}

	void Camera::Offset(Vector3 distance, Vector3 tergetTranslate)//カメラをターゲットに追従させる
	{
		Matrix4x4 RotateMatrix = MakeRotateMatrix(transformMatrix.rotate);
		Vector3 offset = TransformNormal(distance, RotateMatrix);
		transformMatrix.translate = Add(tergetTranslate, offset);
	}

	Camera* Camera::GetInstance()
	{
		if (instance == NULL)
		{
			instance = std::make_unique<Camera>();
		}
		return instance.get();
	}

	void Camera::DeleteInstance()
	{
		instance.reset();
	}
}
