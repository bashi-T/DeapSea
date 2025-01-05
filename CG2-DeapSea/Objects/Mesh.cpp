#include "Mesh.h"
namespace MyEngine
{
	Mesh::~Mesh()
	{
	}

	void Mesh::Initialize(const std::string& filename)
	{
		meshCommon_ = MeshCommon::GetInstance();
		srvManager_ = SRVManager::GetInstance();
		object3dCommon_ = Object3dCommon::GetInstance();
		camera_ = object3dCommon_->GetDefaultCamera();
		kSubdivision = 16;

		Sphere sphere = { { 0.0f,0.0f,0.0f },200.0f };
		ColorSphere[0] = { 1.0f, 1.0f, 1.0f, 1.0f };

		Top[0] = { 0.0f, 1.75f, 0.0f, 1.0f };
		Right[0] = { 0.5f, -0.5f, 0.0f, 1.0f };
		Left[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		Color[0] = { 0.5f, 0.5f, 0.5f, 1.0f };
		texcoordTop[0] = { 0.5f,0.0f };
		texcoordRight[0] = { 1.0f,1.0f };
		texcoordLeft[0] = { 0.0f,1.0f };


		transformMatrix = {
				{1.0f, 1.0f, 1.0f},
				{0.0f, 0.0f, 0.0f},
				{0.0f, 0.0f, 0.0f}
		};
		transformMatrixSphere = {
			{1.0f, 1.0f, 1.0f},
			{0.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, 0.0f}
		};
		cameraTransform = {
			{1.0f, 1.0f, 1.0f},
			{0.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, -15.0f}
		};
		projectionMatrix = MakePerspectiveFovMatrix(0.65f, float(WinAPP::clientWidth_) / float(WinAPP::clientHeight_), 0.1f, 100.0f);

		//vertexResource = CreateBufferResource(sizeof(VertexData) * 3);
		//materialResource = CreateBufferResource(sizeof(Material));
		//transformationMatrixResource = CreateBufferResource(sizeof(TransformationMatrix));
		vertexResourceSphere = CreateBufferResource(sizeof(VertexData) * 6 * kSubdivision * kSubdivision);
		materialResourceSphere = CreateBufferResource(sizeof(Material));
		transformationMatrixResourceSphere = CreateBufferResource(sizeof(TransformationMatrix));

		directionalLightResource = CreateBufferResource(sizeof(DirectionalLight));
		directionalLightResource->Map(0, nullptr, reinterpret_cast<void**>(&DirectionalLightData));
		cameraResource = CreateBufferResource(sizeof(CameraTransform));
		cameraResource->Map(0, nullptr, reinterpret_cast<void**>(&cameraData));

		indexResourceSphere = CreateBufferResource(sizeof(uint32_t) * 6 * kSubdivision * kSubdivision);
		MakeBufferView();

		DirectionalLightData->color = { 1.0f, 1.0f, 1.0f, 1.0f };
		DirectionalLightData->direction = { 0.0f, -1.0f, 0.0f };
		DirectionalLightData->intensity = 1.0f;


		materialData.textureFilePath = filename;
		TextureManager::GetInstance()->LoadTexture(filename);
		materialData.textureIndex = TextureManager::GetInstance()->GetSrvIndex(filename);

	}

	void Mesh::Update()
	{
		//cameraTransform.translate.x += 0.02f;
		//DirectionalLightData->direction = Normalize(DirectionalLightData->direction);
		//ImGui::Begin("Light");
		//ImGui::ColorEdit3("LightColor", (float*)&DirectionalLightData->color, 0.01f);
		//ImGui::DragFloat3("LightDirection", (float*)&DirectionalLightData->direction, 0.01f);
		//ImGui::DragFloat3("CameraRotate", (float*)&cameraTransform.rotate.x, 0.01f);
		//ImGui::DragFloat3("CameraTranslate", (float*)&cameraTransform.translate.x, 0.01f);
		//ImGui::DragFloat("Intensity", (float*)&DirectionalLightData->intensity, 0.01f);
		////ImGui::DragFloat2("UVTranslate", &uvTransformSprite.translate.x, 0.01f, -10.0f, 10.0f);
		////ImGui::DragFloat2("UVScale", &uvTransformSprite.scale.x, 0.01f, -10.0f, 10.0f);
		////ImGui::SliderAngle("UVRotate", &uvTransformSprite.rotate.z);
		//ImGui::DragFloat3("Sphere.rotate", (float*)&transformMatrixSphere.rotate, 0.01f);
		//ImGui::DragFloat3("Sphere.translate", (float*)&transformMatrixSphere.translate, 0.01f);
		//ImGui::End();
	}

	ComPtr<ID3D12Resource> Mesh::CreateBufferResource(size_t sizeInBytes)
	{
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

		hr = DX12Common::GetInstance()->GetDevice().Get()->CreateCommittedResource(
			&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &ResourceDesc,
			D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&Resource));
		assert(SUCCEEDED(hr));
		return Resource;
	}

	void Mesh::MakeBufferView()
	{
		//vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
		//vertexBufferView.SizeInBytes = UINT(sizeof(VertexData)) * 3;
		//vertexBufferView.StrideInBytes = sizeof(VertexData);

		vertexBufferViewSphere.BufferLocation = vertexResourceSphere->GetGPUVirtualAddress();
		vertexBufferViewSphere.SizeInBytes = sizeof(VertexData) * 6 * kSubdivision * kSubdivision;
		vertexBufferViewSphere.StrideInBytes = sizeof(VertexData);

		indexBufferViewSphere.BufferLocation = indexResourceSphere->GetGPUVirtualAddress();
		indexBufferViewSphere.SizeInBytes = sizeof(uint32_t) * 6 * kSubdivision * kSubdivision;
		indexBufferViewSphere.Format = DXGI_FORMAT_R32_UINT;

	}

//void Mesh::InputDataTriangle(
//    Vector4 Top, Vector4 Right, Vector4 Left, Vector4 color, Vector2 coordTop, Vector2 coordRight,
//    Vector2 coordLeft) {
//	VertexData* vertexData = nullptr;
//	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
//	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * 3);
//	Material* materialData = nullptr;
//	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
//	TransformationMatrix* transformationMatrixData = nullptr;
//	transformationMatrixResource->Map(
//	    0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));
//
//	materialData[0].color = color;
//	materialData[0].enableLighting = true;
//
//	transformationMatrixData->WVP = MakeIdentity4x4();
//	
//	transformMatrix.rotate.y += 0.02f;
//	Matrix4x4 worldMatrix =
//	    MakeAffineMatrix(transformMatrix.scale, transformMatrix.rotate, transformMatrix.translate);
//	worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
//	transformationMatrixData->WVP = worldViewProjectionMatrix;
//	transformationMatrixData->World = worldMatrix;
//	
//	vertexData[0].position = Top;
//	vertexData[1].position = Right;
//	vertexData[2].position = Left;
//	vertexData[0].texcoord = coordTop;
//	vertexData[1].texcoord = coordRight;
//	vertexData[2].texcoord = coordLeft;
//	vertexData[0].normal.x = vertexData[0].position.x;
//	vertexData[0].normal.y = vertexData[0].position.y;
//	vertexData[0].normal.z = vertexData[0].position.z;
//	vertexData[1].normal.x = vertexData[1].position.x;
//	vertexData[1].normal.y = vertexData[1].position.y;
//	vertexData[1].normal.z = vertexData[1].position.z;
//	vertexData[2].normal.x = vertexData[2].position.x;
//	vertexData[2].normal.y = vertexData[2].position.y;
//	vertexData[2].normal.z = vertexData[2].position.z;
//}
// 
//void Mesh::DrawTriangle(
//    Vector4 Top, Vector4 Right, Vector4 Left, Vector4 color, Vector2 coordTop, Vector2 coordRight,
//    Vector2 coordLeft, bool useWorldMap) {
//	InputDataTriangle(Top, Right, Left, color, coordTop, coordRight, coordLeft);
//	DX12Common::GetInstance()->GetCommandList().Get()->SetPipelineState(graphicsPipelineState.Get());
//	DX12Common::GetInstance()->GetCommandList().Get()->SetGraphicsRootSignature(rootSignature.Get());
//	DX12Common::GetInstance()->GetCommandList().Get()->IASetVertexBuffers(0, 1, &vertexBufferView);
//	DX12Common::GetInstance()->GetCommandList().Get()->
//		IASetIndexBuffer(&indexBufferViewSphere);
//	DX12Common::GetInstance()->GetCommandList().Get()->IASetPrimitiveTopology(
//	    D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	DX12Common::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(
//	    0, materialResource->GetGPUVirtualAddress());
//	DX12Common::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(
//	    1, transformationMatrixResource->GetGPUVirtualAddress());
//
//	D3D12_CPU_DESCRIPTOR_HANDLE rtv =
//	    DX12Common::GetInstance()->GetRtvHandles(DX12Common::GetInstance()->GetBackBufferIndex());
//	D3D12_CPU_DESCRIPTOR_HANDLE dsv = DX12Common::GetInstance()->GetDsvHandle();
//
//	DX12Common::GetInstance()->GetCommandList().Get()->OMSetRenderTargets(1, &rtv, false, &dsv);
//
//	DX12Common::GetInstance()->GetCommandList().Get()->SetGraphicsRootDescriptorTable(
//	    2, useWorldMap ? GetTextureSrvHandleGPU2()
//	                   : GetTextureSrvHandleGPU());
//
//	DX12Common::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(
//		3, directionalLightResource->GetGPUVirtualAddress());
//
//	DX12Common::GetInstance()->GetCommandList().Get()->DrawInstanced(3, 1, 0, 0);
//}

	void Mesh::InputDataSphere(
		Vector4 LeftTop, Vector4 RightTop, Vector4 RightBottom, Vector4 LeftBottom, Vector4 color,
		Vector2 coordLeftTop, Vector2 coordRightTop, Vector2 coordRightBottom, Vector2 coordLeftBottom,
		uint32_t count, int32_t width, int32_t height)
	{
		VertexData* vertexDataSphere = nullptr;

		vertexResourceSphere->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSphere));

		Material* materialDataSphere = nullptr;
		materialResourceSphere->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSphere));

		uint32_t* indexDataSphere = nullptr;
		indexResourceSphere->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSphere));

		transformationMatrixResourceSphere->Map(
			0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSphere));
		projectionMatrixSphere =
			MakePerspectiveFovMatrix(0.45f, float(width) / float(height), 0.1f, 100.0f);
		transformationMatrixDataSphere->WVP = MakeIdentity4x4();

		Matrix4x4 worldMatrix = MakeAffineMatrix(
			transformMatrixSphere.scale, transformMatrixSphere.rotate, transformMatrixSphere.translate);
		worldViewProjectionMatrixSphere =
			Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrixSphere));
		transformationMatrixDataSphere->WVP = worldViewProjectionMatrixSphere;
		transformationMatrixDataSphere->World = worldMatrix;

		materialDataSphere[0].color = color;
		materialDataSphere[0].enableLighting = true;
		materialDataSphere[0].uvTransform = MakeIdentity4x4();
		Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTransformSphere.scale);
		uvTransformMatrix = Multiply(uvTransformMatrix, MakerotateZMatrix(uvTransformSphere.rotate.z));
		uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransformSphere.translate));
		materialDataSphere[0].uvTransform = uvTransformMatrix;

		vertexDataSphere[count * 4].position = LeftTop;
		vertexDataSphere[count * 4].texcoord = coordLeftTop;
		vertexDataSphere[count * 4].normal.x = vertexDataSphere[count * 4].position.x;
		vertexDataSphere[count * 4].normal.y = vertexDataSphere[count * 4].position.y;
		vertexDataSphere[count * 4].normal.z = vertexDataSphere[count * 4].position.z;

		vertexDataSphere[count * 4 + 1].position = RightTop;
		vertexDataSphere[count * 4 + 1].texcoord = coordRightTop;
		vertexDataSphere[count * 4 + 1].normal.x = vertexDataSphere[count * 4 + 1].position.x;
		vertexDataSphere[count * 4 + 1].normal.y = vertexDataSphere[count * 4 + 1].position.y;
		vertexDataSphere[count * 4 + 1].normal.z = vertexDataSphere[count * 4 + 1].position.z;

		vertexDataSphere[count * 4 + 2].position = RightBottom;
		vertexDataSphere[count * 4 + 2].texcoord = coordRightBottom;
		vertexDataSphere[count * 4 + 2].normal.x = vertexDataSphere[count * 4 + 2].position.x;
		vertexDataSphere[count * 4 + 2].normal.y = vertexDataSphere[count * 4 + 2].position.y;
		vertexDataSphere[count * 4 + 2].normal.z = vertexDataSphere[count * 4 + 2].position.z;

		vertexDataSphere[count * 4 + 3].position = LeftBottom;
		vertexDataSphere[count * 4 + 3].texcoord = coordLeftBottom;
		vertexDataSphere[count * 4 + 3].normal.x = vertexDataSphere[count * 4 + 3].position.x;
		vertexDataSphere[count * 4 + 3].normal.y = vertexDataSphere[count * 4 + 3].position.y;
		vertexDataSphere[count * 4 + 3].normal.z = vertexDataSphere[count * 4 + 3].position.z;

		indexDataSphere[count * 6 + 0] = count * 4 + 0;
		indexDataSphere[count * 6 + 1] = count * 4 + 1;
		indexDataSphere[count * 6 + 2] = count * 4 + 2;

		indexDataSphere[count * 6 + 3] = count * 4 + 0;
		indexDataSphere[count * 6 + 4] = count * 4 + 2;
		indexDataSphere[count * 6 + 5] = count * 4 + 3;
	}

	void Mesh::DrawSphere(const Sphere& sphere_, Vector4 color)
	{
		float pi = 3.141592f;
		const float kLonevery = 2.0f / kSubdivision * pi;
		const float kLatevery = pi / kSubdivision;
		uint32_t sphereCount = 0;

		for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex)
		{
			float lat = -pi / 2.0f + kLatevery * latIndex; // theta
			float latB = pi / kSubdivision;

			for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex)
			{
				float lon = lonIndex * kLonevery; // phi
				float lonC = 2 * pi / kSubdivision;

				Vector4 a = {
					sphere_.center.x + sphere_.radius * cos(lat) * cos(lon),
					sphere_.center.y + sphere_.radius * sin(lat),
					sphere_.center.z + sphere_.radius * cos(lat) * sin(lon), 1.0f };
				Vector4 b = {
					sphere_.center.x + sphere_.radius * cos(lat + latB) * cos(lon),
					sphere_.center.y + sphere_.radius * sin(lat + latB),
					sphere_.center.z + sphere_.radius * cos(lat + latB) * sin(lon), 1.0f };
				Vector4 c = {
					sphere_.center.x + sphere_.radius * cos(lat) * cos(lon + lonC),
					sphere_.center.y + sphere_.radius * sin(lat),
					sphere_.center.z + sphere_.radius * cos(lat) * sin(lon + lonC), 1.0f };
				Vector4 d = {
					sphere_.center.x + sphere_.radius * cos(lat + latB) * cos(lon + lonC),
					sphere_.center.y + sphere_.radius * sin(lat + latB),
					sphere_.center.z + sphere_.radius * cos(lat + latB) * sin(lon + lonC), 1.0f };
				Vector2 texcoordA
				{
					float(lonIndex) / float(kSubdivision),
					1.0f - float(latIndex) / float(kSubdivision)
				};
				Vector2 texcoordB
				{
					float(lonIndex) / float(kSubdivision),
					1.0f - float(latIndex + 1) / float(kSubdivision)
				};
				Vector2 texcoordC
				{
					float(lonIndex + 1) / float(kSubdivision),
					1.0f - float(latIndex) / float(kSubdivision)
				};
				Vector2 texcoordD
				{
					float(lonIndex + 1) / float(kSubdivision),
					1.0f - float(latIndex + 1) / float(kSubdivision)
				};

				InputDataSphere(
					b, d, c, a, color, texcoordB, texcoordD, texcoordC, texcoordA, sphereCount, WinAPP::clientWidth_, WinAPP::clientHeight_);

				DX12Common::GetInstance()->GetCommandList().Get()->
					SetPipelineState(meshCommon_->GetGraphicsPipelineState().Get());

				DX12Common::GetInstance()->GetCommandList().Get()->
					SetGraphicsRootSignature(meshCommon_->GetRootSignature().Get());

				DX12Common::GetInstance()->GetCommandList().Get()->
					IASetPrimitiveTopology(
						D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

				DX12Common::GetInstance()->GetCommandList().Get()->
					SetGraphicsRootConstantBufferView(
						0, materialResourceSphere->GetGPUVirtualAddress());

				DX12Common::GetInstance()->GetCommandList().Get()->
					SetGraphicsRootConstantBufferView(
						1, transformationMatrixResourceSphere->GetGPUVirtualAddress());

				srvManager_->SetGraphicsRootDescriptorTable(
					2, materialData.textureIndex);
				DX12Common::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(
					3, cameraResource->GetGPUVirtualAddress());
				DX12Common::GetInstance()->GetCommandList().Get()->
					SetGraphicsRootConstantBufferView(
						4, directionalLightResource->GetGPUVirtualAddress());
				D3D12_CPU_DESCRIPTOR_HANDLE rtv =
					DX12Common::GetInstance()->GetRtvHandles(srvManager_->GetBackBufferIndex());
				D3D12_CPU_DESCRIPTOR_HANDLE dsv = DX12Common::GetInstance()->GetDsvHandle();
				DX12Common::GetInstance()->GetCommandList().Get()->OMSetRenderTargets(1, &rtv, false, &dsv);

				DX12Common::GetInstance()->GetCommandList().Get()->
					IASetVertexBuffers(
						0, 1, &vertexBufferViewSphere);
				DX12Common::GetInstance()->GetCommandList().Get()->
					IASetIndexBuffer(&indexBufferViewSphere);

				sphereCount++;
			}
		}
		DX12Common::GetInstance()->GetCommandList().Get()->DrawIndexedInstanced(
			6 * kSubdivision * kSubdivision, 1, 0, 0, 0);
	}

//void Mesh::MakeShaderResourceView(const DirectX::TexMetadata& metadata, const DirectX::TexMetadata& metadata2)
//{
//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
//	srvDesc.Format = metadata.format;
//	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
//	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);
//
//	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};
//	srvDesc2.Format = metadata2.format;
//	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
//	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
//	srvDesc2.Texture2D.MipLevels = UINT(metadata2.mipLevels);
//
//	const uint32_t descriptorSizeSRV = DX12Common::GetInstance()->
//		GetDevice().Get()->GetDescriptorHandleIncrementSize(
//			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
//
//	//textureSrvHandleCPU = DX12Common::GetInstance()->
//	//	GetCPUDescriptorHandle(DX12Common::GetInstance()->
//	//		GetSrvDescriptorHeap().Get(), descriptorSizeSRV, 1);
//	//textureSrvHandleGPU = DX12Common::GetInstance()->
//	//	GetGPUDescriptorHandle(DX12Common::GetInstance()->
//	//		GetSrvDescriptorHeap().Get(), descriptorSizeSRV, 1);
//
//	//textureSrvHandleCPU2 = DX12Common::GetInstance()->
//	//	GetCPUDescriptorHandle(DX12Common::GetInstance()->
//	//		GetSrvDescriptorHeap().Get(), descriptorSizeSRV, 2);
//	//textureSrvHandleGPU2 = DX12Common::GetInstance()->
//	//	GetGPUDescriptorHandle(DX12Common::GetInstance()->
//	//		GetSrvDescriptorHeap().Get(), descriptorSizeSRV, 2);
//
//	DX12Common::GetInstance()->GetDevice().Get()->CreateShaderResourceView(textureResource.Get(), &srvDesc, textureSrvHandleCPU);
//	DX12Common::GetInstance()->GetDevice().Get()->CreateShaderResourceView(textureResource2.Get(), &srvDesc2, textureSrvHandleCPU2);
//}
//
//DirectX::ScratchImage Mesh::LoadTexture(const std::string& filePath)
//{
//	DirectX::ScratchImage image{};
//	std::wstring filePathW = debug_->ConvertString(filePath);
//	HRESULT hr = DirectX::LoadFromWICFile(
//		filePathW.c_str(),
//		DirectX::WIC_FLAGS_FORCE_SRGB,
//		nullptr,
//		image);
//	assert(SUCCEEDED(hr));
//
//	DirectX::ScratchImage mipImages{};
//	hr = DirectX::GenerateMipMaps(
//		image.GetImages(),
//		image.GetImageCount(),
//		image.GetMetadata(),
//		DirectX::TEX_FILTER_SRGB,
//		0,
//		mipImages);
//
//	return mipImages;
//}
//
//ComPtr<ID3D12Resource> Mesh::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata)
//{
//	D3D12_RESOURCE_DESC resourceDesc{};
//	resourceDesc.Width = UINT(metadata.width);
//	resourceDesc.Height = UINT(metadata.height);
//	resourceDesc.MipLevels = UINT16(metadata.mipLevels);
//	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);
//	resourceDesc.Format = metadata.format;
//	resourceDesc.SampleDesc.Count = 1;
//	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);
//
//	D3D12_HEAP_PROPERTIES heapProperties{};
//	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;
//	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
//	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;
//
//	ComPtr<ID3D12Resource> resource = nullptr;
//	HRESULT hr = device->CreateCommittedResource(
//		&heapProperties,
//		D3D12_HEAP_FLAG_NONE,
//		&resourceDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ,
//		nullptr,
//		IID_PPV_ARGS(&resource));
//
//	assert(SUCCEEDED(hr));
//
//	return resource;
//}
//
//void Mesh::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages, const DirectX::TexMetadata& metadata)
//{
//	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; mipLevel++)
//	{
//		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
//		HRESULT hr = texture->WriteToSubresource(
//			UINT(mipLevel),
//			nullptr,
//			img->pixels,
//			UINT(img->rowPitch),
//			UINT(img->slicePitch)
//		);
//		assert(SUCCEEDED(hr));
//	}
//}
}