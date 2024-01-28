#pragma once
#include "CGVector.h"
#include "DX12Common.h"
#include "MyImGui.h"
#include <cassert>
#include <dxcapi.h>
#include <fstream>
#include <sstream>
#include"TextureManager.h"

#pragma comment(lib, "dxcompiler.lib")

class Object3dCommon;
class Object3d
{
private:
	Object3dCommon* object3dCommon_ = nullptr;
	HRESULT hr = NULL;

	struct DirectionalLight
	{
		Vector4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		Vector3 direction = { 0.0f, -1.0f, 0.0f };
		float intensity = 1.0f;
	};
	DirectionalLight* DirectionalLightData = nullptr;

	TransformMatrix transformMatrix;
	ComPtr<ID3D12Resource> transformationMatrixResource;
	TransformMatrix cameraTransform;

	struct VertexData
	{
		Vector4 position;
		Vector2 texcoord;
		Vector3 normal;
	};
	VertexData* vertexData = nullptr;
	ComPtr<ID3D12Resource> vertexResource = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};

	struct Material {
		Vector4 color;
		int32_t enableLighting;
		float padding[3];
		Matrix4x4 uvTransform;
	};
	Material* materialData = nullptr;
	ComPtr<ID3D12Resource> materialResource = nullptr;

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

	TransformMatrix uvTransform
	{
			{1.0f,1.0f,1.0f},
			{0.0f,0.0f,0.0f},
			{0.0f,0.0f,0.0f},
	};
	struct TransformationMatrix
	{
		Matrix4x4 WVP;
		Matrix4x4 World;
	};

	TransformationMatrix* transformationMatrixData = nullptr;

	Matrix4x4 viewMatrix;
	Matrix4x4 projectionMatrix;
	Matrix4x4 worldViewProjectionMatrix;

	ComPtr<ID3D12Resource> directionalLightResource;
public:
	void Initialize(Object3dCommon* object3dCommon, int32_t width, int32_t height, std::string textureFilePath, std::string objFilePath);
	void Update();
	void Draw(Object3dCommon* object3dCommon,bool useWorldMap);
	
	void MakeBufferView();
	ModelData LoadObjFile(const std::string& directryPath, const std::string& filename);
	MaterialData LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename);
	ComPtr<ID3D12Resource> CreateBufferResource(Object3dCommon* object3dCommon, size_t sizeInBytes);

	DirectionalLight* DrawDirectionalLightData() { return DirectionalLightData; }
};

