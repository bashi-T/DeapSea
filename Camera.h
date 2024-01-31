#pragma once
#include"CGVector.h"

class Camera
{
private:
	TransformMatrix transformMatrix;
	Matrix4x4 worldMatrix;
	Matrix4x4 viewMatrix;

public:
	void Update();

};

