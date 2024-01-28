#include "Object3d.h"
#include "Object3dCommon.h"

void Object3d::Initialize(Object3dCommon* object3dCommon, int32_t width, int32_t height, std::string textureFilePath, std::string objFilePath)
{
	this->object3dCommon_ = object3dCommon;
	modelData = LoadObjFile("Resource", objFilePath);

	vertexResource = CreateBufferResource(object3dCommon_,sizeof(VertexData) * modelData.vertices.size());
	materialResource = CreateBufferResource(object3dCommon_,sizeof(Material));
	transformationMatrixResource = CreateBufferResource(object3dCommon_,sizeof(TransformationMatrix));
	directionalLightResource = CreateBufferResource(object3dCommon_, sizeof(DirectionalLight));
	
	transformMatrix = {
	{1.0f, 1.0f, 1.0f},
	{0.0f, 0.0f, 0.0f},
	{0.0f, 0.0f, 0.0f}
	};
	cameraTransform = {
	{1.0f, 1.0f, 1.0f},
	{0.3f, 0.0f, 0.0f},
	{0.0f, 4.0f, -10.0f}
	};
	
	TextureManager::GetInstance()->LoadTexture(object3dCommon_->GetDx12Common(), modelData.material.textureFilePath);
	TextureManager::GetInstance()->GetTextureIndexByFilePath(modelData.material.textureFilePath);

	MakeBufferView();
	
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	transformationMatrixResource->Map(
		0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));
    directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&DirectionalLightData));
	
	materialData[0].color = Vector4(1.0f, 1.0f, 1.0f, 1.0f);
	materialData[0].enableLighting = false;
	materialData[0].uvTransform = MakeIdentity4x4();
	projectionMatrix =
		MakePerspectiveFovMatrix(0.45f, float(width) / float(height), 0.1f, 100.0f);
	transformationMatrixData->WVP = MakeIdentity4x4();
	Matrix4x4 worldMatrix = MakeAffineMatrix(
		transformMatrix.scale, transformMatrix.rotate, transformMatrix.translate);
	worldViewProjectionMatrix =
		Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	transformationMatrixData->WVP = worldViewProjectionMatrix;
	transformationMatrixData->World = worldMatrix;

    DirectionalLightData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
    DirectionalLightData->direction = { 0.0f, -1.0f, 0.0f };
    DirectionalLightData->intensity = 1.0f;
}

void Object3d::Update()
{
	Matrix4x4 worldMatrix = MakeAffineMatrix(
		transformMatrix.scale, transformMatrix.rotate, transformMatrix.translate);
	Matrix4x4 cameraMatrix =
		MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	viewMatrix = Inverse(cameraMatrix);
	worldViewProjectionMatrix =
		Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	transformationMatrixData->WVP = worldViewProjectionMatrix;
	transformationMatrixData->World = worldMatrix;

}

void Object3d::Draw(Object3dCommon* object3dCommon,bool useWorldMap)
{
	this->object3dCommon_ = object3dCommon;
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * modelData.vertices.size());

	Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTransform.scale);
	uvTransformMatrix = Multiply(uvTransformMatrix, MakerotateZMatrix(uvTransform.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransform.translate));
	materialData[0].uvTransform = uvTransformMatrix;

	object3dCommon_->GetDx12Common()->GetCommandList().Get()->
		SetPipelineState(object3dCommon_->GetGraphicsPipelineState().Get());
	object3dCommon_->GetDx12Common()->GetCommandList().Get()->
		SetGraphicsRootSignature(object3dCommon_->GetRootSignature().Get());
	object3dCommon_->GetDx12Common()->GetCommandList().Get()->
		IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	D3D12_CPU_DESCRIPTOR_HANDLE rtv = object3dCommon_->GetDx12Common()->GetRtvHandles(
		object3dCommon_->GetDx12Common()->GetBackBufferIndex());
	D3D12_CPU_DESCRIPTOR_HANDLE dsv = object3dCommon_->GetDx12Common()->GetDsvHandle();
	object3dCommon_->GetDx12Common()->GetCommandList().Get()->OMSetRenderTargets(1, &rtv, false, &dsv);

	//object3dCommon_->GetDx12Common()->GetCommandList().Get()->SetGraphicsRootDescriptorTable(
	//	2, useWorldMap ? GetTextureSrvHandleGPU2()
	//	: GetTextureSrvHandleGPU());
	object3dCommon_->GetDx12Common()->GetCommandList().Get()->
		SetGraphicsRootDescriptorTable(
			2, TextureManager::GetInstance()->GetSRVHandleGPU(modelData.material.textureIndex));

	object3dCommon_->GetDx12Common()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(
		3, directionalLightResource->GetGPUVirtualAddress());

	object3dCommon_->GetDx12Common()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(
		0, materialResource->GetGPUVirtualAddress());
	object3dCommon_->GetDx12Common()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(
		1, transformationMatrixResource->GetGPUVirtualAddress());

	object3dCommon_->GetDx12Common()->GetCommandList().Get()->IASetVertexBuffers(
		0, 1, &vertexBufferView);
	object3dCommon_->GetDx12Common()->GetCommandList().Get()->DrawInstanced(
		UINT(modelData.vertices.size()), 1, 0, 0);
}

void Object3d::MakeBufferView()
{
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData) * modelData.vertices.size());
	vertexBufferView.StrideInBytes = sizeof(VertexData);
}

Object3d::ModelData Object3d::LoadObjFile(const std::string& directryPath, const std::string& filename)
{
	ModelData modelData;
	std::vector<Vector4> positions;
	std::vector<Vector3> normals;
	std::vector<Vector2> texcoords;
	std::string line;

	std::ifstream file(directryPath + "/" + filename);
	assert(file.is_open());

	while (std::getline(file, line))
	{
		std::string identifier;
		std::istringstream s(line);
		s >> identifier;

		if (identifier == "v")
		{
			Vector4 position;
			position.w = 1.0f;
			s >> position.x >> position.y >> position.z;
			position.w = 1.0f;
			positions.push_back(position);
		}
		else if (identifier == "vt")
		{
			Vector2 texcoord;
			s >> texcoord.x >> texcoord.y;
			texcoord.y = 1.0f - texcoord.y;
			texcoords.push_back(texcoord);
		}
		else if (identifier == "vn")
		{
			Vector3 normal;
			s >> normal.x >> normal.y >> normal.z;
			normals.push_back(normal);
		}
		else if (identifier == "mtllib")
		{
			std::string materialFilemane;
			s >> materialFilemane;
			modelData.material = LoadMaterialTemplateFile(directryPath, materialFilemane);
		}
		else if (identifier == "f")
		{
			VertexData triangle[3];

			for (int32_t faceVertex = 0; faceVertex < 3; ++faceVertex) {
				std::string vertexDefinition;
				s >> vertexDefinition;

				std::istringstream v(vertexDefinition);
				uint32_t elementIndices[3];
				for (int32_t element = 0; element < 3; ++element) {
					std::string index;
					std::getline(v, index, '/');
					elementIndices[element] = std::stoi(index);
				}

				Vector4 position = positions[elementIndices[0] - 1];
				Vector2 texcoord = texcoords[elementIndices[1] - 1];
				Vector3 normal = normals[elementIndices[2] - 1];
				VertexData vertex = { position, texcoord, normal };
				modelData.vertices.push_back(vertex);
				triangle[faceVertex] = { position, texcoord, normal };
			}
			modelData.vertices.push_back(triangle[0]);
			modelData.vertices.push_back(triangle[1]);
			modelData.vertices.push_back(triangle[2]);
		}
	}
	return modelData;
}

Object3d::MaterialData Object3d::LoadMaterialTemplateFile(const std::string& directoryPath, const std::string& filename)
{
	MaterialData materialData;
	std::string line;

	std::ifstream file(directoryPath + "/" + filename);
	assert(file.is_open());

	while (std::getline(file, line)) {
		std::string identifier;
		std::stringstream s(line);
		s >> identifier;

		if (identifier == "map_Kd") {
			std::string textureFilename;
			s >> textureFilename;
			materialData.textureFilePath = directoryPath + "/" + textureFilename;
		}
	}
	return materialData;
}

ComPtr<ID3D12Resource> Object3d::CreateBufferResource(Object3dCommon* object3dCommon, size_t sizeInBytes)
{
	this->object3dCommon_ = object3dCommon;
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};

	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC ResourceDesc{};

	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;

	ResourceDesc.Width = sizeInBytes * 3;

	ResourceDesc.Height = 1;
	ResourceDesc.DepthOrArraySize = 1;
	ResourceDesc.MipLevels = 1;
	ResourceDesc.SampleDesc.Count = 1;

	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	ComPtr<ID3D12Resource> Resource = nullptr;

	hr = object3dCommon_->GetDx12Common()->GetDevice().Get()->CreateCommittedResource(
		&uploadHeapProperties,
		D3D12_HEAP_FLAG_NONE,
		&ResourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&Resource));
	assert(SUCCEEDED(hr));
	return Resource;
}
