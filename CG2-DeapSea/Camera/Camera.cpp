#include "Camera.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

Camera::Camera()
:transformMatrix({ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} })
, fovY(0.45f)
, aspectRetio((float(WinAPP::clientWidth_) / float(WinAPP::clientHeight_)))
, nearClip(0.1f)
, farClip(100.0f)
, worldMatrix(MakeAffineMatrix(transformMatrix.scale, transformMatrix.rotate, transformMatrix.translate))
, viewMatrix(Inverse(worldMatrix))
, projectionMatrix(MakePerspectiveFovMatrix(fovY, aspectRetio, nearClip, farClip))
, viewProjectionMatrix(Multiply(viewMatrix, projectionMatrix))
{}

Camera::~Camera()
{
}

void Camera::Initialize()
{
	//transformMatrix={ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	//fovY=0.45f;
	//aspectRetio = (float(width) / float(height));
	//nearClip = 0.1f;
	//farClip = 100.0f;
	//worldMatrix = MakeAffineMatrix(transformMatrix.scale, transformMatrix.rotate, transformMatrix.translate);
	//viewMatrix = Inverse(worldMatrix);
	//projectionMatrix =
	//	MakePerspectiveFovMatrix(fovY, aspectRetio, nearClip, farClip);
	//viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
}

void Camera::Update()
{
	worldMatrix = MakeAffineMatrix(transformMatrix.scale, transformMatrix.rotate, transformMatrix.translate);
	viewMatrix = Inverse(worldMatrix);
	projectionMatrix =
		MakePerspectiveFovMatrix(fovY, aspectRetio, nearClip, farClip);
	viewProjectionMatrix = Multiply(viewMatrix, projectionMatrix);
}

Camera* Camera::GetInstance()
{
	if (instance == NULL)
	{
		instance = new Camera;
	}
	return instance;
}

void Camera::DeleteInstance()
{
	if (instance != NULL)
	{
		delete instance;
	}
	instance = NULL;
}
