#pragma once
#include"Objects/Object3d.h"
#include"Commons/Object3dCommon.h"

class SkyDome
{
public:
	~SkyDome();
	void Initialize();
	void Update();
	void Draw();

	//void SphereInitialize();
	//void SphereUpdate();
	//void SphereDraw();

	//void ResetDXC();
	//void MakePSO();


private:
	Object3d* object3d;

	//EulerTransform transformMatrixSphere;
	//ComPtr<ID3D12Resource> transformationMatrixResourceSphere;
	//ComPtr<ID3D12Resource> vertexResourceSphere = nullptr;
	//D3D12_VERTEX_BUFFER_VIEW vertexBufferViewSphere{};


	//ComPtr<ID3D12Resource> indexResourceSphere = nullptr;
	//D3D12_INDEX_BUFFER_VIEW indexBufferViewSphere{};

	//ComPtr<ID3D12Resource> materialResourceSphere;

	//EulerTransform uvTransformSphere{
	//	{1.0f,1.0f,1.0f},
	//	{0.0f,0.0f,0.0f},
	//	{0.0f,0.0f,0.0f},
	//};
	//struct TransformationMatrix {
	//	Matrix4x4 WVP;
	//	Matrix4x4 World;
	//};
	//TransformationMatrix* transformationMatrixDataSphere = nullptr;

	//Matrix4x4 viewMatrixSphere;
	//Matrix4x4 projectionMatrixSphere;
	//Matrix4x4 worldViewProjectionMatrixSphere;

	//uint32_t kSubdivision = 16;

	//Vector4 ColorSphere[1];

	//Sphere sphere = { { 0.0f,0.0f,0.0f },1.0f };


};

