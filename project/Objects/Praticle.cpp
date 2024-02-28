#include "Particle.h"

Particle::~Particle() {
}

void Particle::Initialize(const std::string& filename, int32_t width, int32_t height, Object3dCommon* object3dCommon)
{
	this->object3dCommon_ = object3dCommon;
	this->camera_ = object3dCommon_->GetDefaultCamera();
	kNumMaxInstance = 10;
	kSubdivision = 16;
	std::random_device seedGenerator;
	std::mt19937 randomEngine(seedGenerator());
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);

	ResetDXC();

	MakePSO();

	ColorSphere[0] = { 1.0f, 1.0f, 1.0f, 1.0f };

	Top[0] = { 0.0f, 1.75f, 0.0f, 1.0f };
	Right[0] = { 0.5f, -0.5f, 0.0f, 1.0f };
	Left[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	Color[0] = { 0.5f, 0.5f, 0.5f, 1.0f };
	texcoordTop[0] = { 0.5f,0.0f };
	texcoordRight[0] = { 1.0f,1.0f };
	texcoordLeft[0] = { 0.0f,1.0f };

	LeftTop[0] = { -0.5f, 0.5f, 0.0f, 1.0f };
	RightTop[0] = { 0.5f, 0.5f, 0.0f, 1.0f };
	RightBottom[0] = { 0.5f, -0.5f, 0.0f, 1.0f };
	LeftBottom[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
	ColorPlane[0] = { 1.0f, 1.0f, 1.0f, 1.0f };
	texcoordLeftTop[0] = { 0.0f,0.0f };
	texcoordRightTop[0] = { 0.0f,1.0f };
	texcoordRightBottom[0] = { 1.0f,1.0f };
	texcoordLeftBottom[0] = { 1.0f,0.0f };

	transformMatrixTriangle = {
			{1.0f, 1.0f, 1.0f},
			{0.0f, 0.0f, 0.0f},
			{0.0f, 0.0f, 0.0f}
	};

	for (uint32_t index = 0; index < kNumMaxInstance; ++index)
	{
		particlesPlane[index] = MakeNewParticle(randomEngine);
	}
	transformMatrixSphere = {
		{1.0f, 1.0f, 1.0f},
		{0.0f, 0.0f, 0.0f},
		{0.0f, 0.0f, 0.0f}
	};

	projectionMatrixTriangle = MakePerspectiveFovMatrix(0.65f, float(width) / float(height), 0.1f, 100.0f);
	vertexResourceTriangle = CreateBufferResource(sizeof(VertexData) * 3);
	materialResourceTriangle = CreateBufferResource(sizeof(Material));
	transformationMatrixResourceTriangle = CreateBufferResource(sizeof(TransformationMatrix));

	projectionMatrixPlane = MakePerspectiveFovMatrix(0.65f, float(width) / float(height), 0.1f, 100.0f);
	vertexResourcePlane = CreateBufferResource(sizeof(VertexData) * 6);
	materialResourcePlane = CreateBufferResource(sizeof(Material));
	indexResourcePlane = CreateBufferResource(sizeof(uint32_t) * 6);
	instancingResourcePlane = CreateBufferResource(sizeof(ParticleForGPU) * kNumMaxInstance);

	vertexResourceSphere = CreateBufferResource(sizeof(VertexData) * 6 * kSubdivision * kSubdivision);
	materialResourceSphere = CreateBufferResource(sizeof(Material));
	transformationMatrixResourceSphere = CreateBufferResource(sizeof(TransformationMatrix));
	indexResourceSphere = CreateBufferResource(sizeof(uint32_t) * 6 * kSubdivision * kSubdivision);

	MakeBufferView();

	//mipImages = LoadTexture(modelData.material.textureFilePath);
	//const DirectX::TexMetadata& metadata = mipImages.GetMetadata();
	//textureResource = CreateTextureResource(DX12Common::GetInstance()->GetDevice().Get(), metadata);
	//UploadTextureData(textureResource.Get(), mipImages, metadata);
	//mipImages2 = LoadTexture(modelData.material.textureFilePath);
	//const DirectX::TexMetadata& metadata2 = mipImages2.GetMetadata();
	//textureResource2 = CreateTextureResource(DX12Common::GetInstance()->GetDevice().Get(), metadata2);
	//UploadTextureData(textureResource2.Get(), mipImages2, metadata2);
	//MakeShaderResourceView(metadata);


	vertexResourcePlane->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataPlane));
	materialResourcePlane->Map(0, nullptr, reinterpret_cast<void**>(&materialDataPlane));
	instancingResourcePlane->Map(0, nullptr, reinterpret_cast<void**>(&instancingDataPlane));
	InputDataPlane(LeftTop[0], RightTop[0], RightBottom[0], LeftBottom[0], ColorPlane[0],
		texcoordLeftTop[0], texcoordRightTop[0], texcoordRightBottom[0], texcoordLeftBottom[0]);

	for (uint32_t index = 0; index < kNumMaxInstance; ++index)
	{
		instancingDataPlane[index].WVP = MakeIdentity4x4();
		instancingDataPlane[index].World = MakeIdentity4x4();

	}
	MakeShaderResourceViewInstance(instancingResourcePlane.Get());
	TextureManager::GetInstance()->LoadTexture(DX12Common::GetInstance(), filename);
	textureIndex = TextureManager::GetInstance()->GetTextureIndexByFilePath(filename);
}

void Particle::ResetDXC()
{
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr));
	hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr));
}

ComPtr<IDxcBlob> Particle::CompileShader(
	const std::wstring& filePath,
	const wchar_t* profile,
	IDxcUtils* dxcUtils_,
	IDxcCompiler3* dxcCompiler_,
	IDxcIncludeHandler* includeHandler_)
{
	debug_->Log(
	    debug_->ConvertString(std::format(L"Begin CompileShader,path{},\n", filePath, profile)));
	ComPtr<IDxcBlobEncoding> shaderSource = nullptr;
	hr = dxcUtils_->LoadFile(filePath.c_str(), nullptr, shaderSource.GetAddressOf());
	assert(SUCCEEDED(hr));

	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	LPCWSTR arguments[]{
	    filePath.c_str(), L"-E", L"main", L"-T", profile, L"-Zi", L"-Qembed_debug", L"-Od", L"-Zpr",
	};
	ComPtr<IDxcResult> shaderResult = nullptr;
	hr = dxcCompiler_->Compile(
	    &shaderSourceBuffer, arguments, _countof(arguments), includeHandler_,
	    IID_PPV_ARGS(&shaderResult));
	assert(SUCCEEDED(hr));

	ComPtr<IDxcBlobUtf8> shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
		debug_->Log(shaderError->GetStringPointer());
		assert(SUCCEEDED(hr));
	}
	ComPtr<IDxcBlob> shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	debug_->Log(
	    debug_->ConvertString(std::format(L"Compile Succeded,path:{}\n", filePath, profile)));
	shaderSource->Release();
	shaderResult->Release();
	return shaderBlob;
}

void Particle::MakePSO()
{
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_{};
	descriptionRootSignature_.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;

	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
	descriptorRange[0].BaseShaderRegister = 0;
	descriptorRange[0].NumDescriptors = 1;
	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_DESCRIPTOR_RANGE descriptorRangeForInstancing[1] = {};
	descriptorRangeForInstancing[0].BaseShaderRegister = 0;
	descriptorRangeForInstancing[0].NumDescriptors = 1;
	descriptorRangeForInstancing[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
	descriptorRangeForInstancing[0].OffsetInDescriptorsFromTableStart =
		D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;

	D3D12_ROOT_PARAMETER rootParameters[4] = {};
	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[0].Descriptor.ShaderRegister = 0;

	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	rootParameters[1].Descriptor.ShaderRegister = 0;
	rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRangeForInstancing;
	rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRangeForInstancing);

	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);

	descriptionRootSignature_.pParameters = rootParameters;
	descriptionRootSignature_.NumParameters = _countof(rootParameters);

	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
	staticSamplers[0].ShaderRegister = 0;
	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	descriptionRootSignature_.pStaticSamplers = staticSamplers;
	descriptionRootSignature_.NumStaticSamplers = _countof(staticSamplers);

	hr = D3D12SerializeRootSignature(
		&descriptionRootSignature_, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);

	if (FAILED(hr))
	{
		debug_->Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
		assert(false);
	}

	hr = DX12Common::GetInstance()->GetDevice().Get()->CreateRootSignature(
		0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(),
		IID_PPV_ARGS(&rootSignature));

	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};

	inputElementDescs[0].SemanticName = "POSITION";
	inputElementDescs[0].SemanticIndex = 0;
	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[1].SemanticName = "TEXCOORD";
	inputElementDescs[1].SemanticIndex = 0;
	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
	inputElementDescs[2].SemanticName = "NORMAL";
	inputElementDescs[2].SemanticIndex = 0;
	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

	inputLayoutDesc.pInputElementDescs = inputElementDescs;
	inputLayoutDesc.NumElements = _countof(inputElementDescs);

	D3D12_BLEND_DESC blendDesc{};
	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	D3D12_RASTERIZER_DESC rasterizerDesc{};
	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

	ComPtr<IDxcBlob> pixelShaderBlob = nullptr;
	ComPtr<IDxcBlob> vertexShaderBlob = nullptr;
	vertexShaderBlob =
		CompileShader(L"HLSL/Particle.VS.hlsl", L"vs_6_0", dxcUtils.Get(), dxcCompiler.Get(), includeHandler.Get());
	assert(vertexShaderBlob != nullptr);

	pixelShaderBlob =
		CompileShader(L"HLSL/Particle.PS.hlsl", L"ps_6_0", dxcUtils.Get(), dxcCompiler.Get(), includeHandler.Get());
	assert(pixelShaderBlob != nullptr);

	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	graphicsPipelineStateDesc.pRootSignature = rootSignature.Get();
	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
	graphicsPipelineStateDesc.VS = {
		vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.PS = {
		pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize() };
	graphicsPipelineStateDesc.BlendState = blendDesc;
	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;

	graphicsPipelineStateDesc.NumRenderTargets = 1;
	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;

	graphicsPipelineStateDesc.SampleDesc.Count = 1;
	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;

	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

	hr = DX12Common::GetInstance()->GetDevice().Get()->CreateGraphicsPipelineState(
		&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState));
	assert(SUCCEEDED(hr));
}

void Particle::Update()
{
	for (uint32_t index = 0; index < kNumMaxInstance; ++index)
	{
	};
	InputDataPlane(LeftTop[0], RightTop[0], RightBottom[0], LeftBottom[0], ColorPlane[0],
		texcoordLeftTop[0], texcoordRightTop[0], texcoordRightBottom[0], texcoordLeftBottom[0]);
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

void Particle::Draw()
{
	//DrawTriangle(Top[0],Right[0], Left[0],Color[0],texcoordTop[0],texcoordRight[0],texcoordLeft[0],true);

	DrawPlane();

	//DrawSphere(sphere, ColorSphere[0], true, width, height);

};

ComPtr<ID3D12Resource> Particle::CreateBufferResource(size_t sizeInBytes)
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

void Particle::MakeBufferView()
{
	vertexBufferViewTriangle.BufferLocation = vertexResourceTriangle->GetGPUVirtualAddress();
	vertexBufferViewTriangle.SizeInBytes = UINT(sizeof(VertexData)) * 3;
	vertexBufferViewTriangle.StrideInBytes = sizeof(VertexData);

	vertexBufferViewPlane.BufferLocation = vertexResourcePlane->GetGPUVirtualAddress();
	vertexBufferViewPlane.SizeInBytes = UINT(sizeof(VertexData)) * 6;
	vertexBufferViewPlane.StrideInBytes = sizeof(VertexData);

	indexBufferViewPlane.BufferLocation = indexResourcePlane->GetGPUVirtualAddress();
	indexBufferViewPlane.SizeInBytes = sizeof(uint32_t) * 6;
	indexBufferViewPlane.Format = DXGI_FORMAT_R32_UINT;

	vertexBufferViewSphere.BufferLocation = vertexResourceSphere->GetGPUVirtualAddress();
	vertexBufferViewSphere.SizeInBytes = sizeof(VertexData) * 6 * kSubdivision * kSubdivision;
	vertexBufferViewSphere.StrideInBytes = sizeof(VertexData);

	indexBufferViewSphere.BufferLocation = indexResourceSphere->GetGPUVirtualAddress();
	indexBufferViewSphere.SizeInBytes = sizeof(uint32_t) * 6 * kSubdivision * kSubdivision;
	indexBufferViewSphere.Format = DXGI_FORMAT_R32_UINT;

}

void Particle::InputDataTriangle(
    Vector4 Top, Vector4 Right, Vector4 Left, Vector4 color, Vector2 coordTop, Vector2 coordRight,
    Vector2 coordLeft) {
	VertexData* vertexData = nullptr;
	vertexResourceTriangle->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	std::memcpy(vertexData, modelData.vertices.data(), sizeof(VertexData) * 3);
	Material* materialData = nullptr;
	materialResourceTriangle->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	TransformationMatrix* transformationMatrixData = nullptr;
	transformationMatrixResourceTriangle->Map(
	    0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));

	materialData[0].color = color;
	materialData[0].enableLighting = true;
	materialData[0].uvTransform= MakeIdentity4x4();
	Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTransformTriangle.scale);
	uvTransformMatrix = Multiply(uvTransformMatrix, MakerotateZMatrix(uvTransformTriangle.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransformTriangle.translate));
	materialData[0].uvTransform = uvTransformMatrix;
	transformationMatrixData->WVP = MakeIdentity4x4();
	
	transformMatrixTriangle.rotate.y += 0.02f;
	Matrix4x4 worldMatrix =
	    MakeAffineMatrix(transformMatrixTriangle.scale, transformMatrixTriangle.rotate, transformMatrixTriangle.translate);
	worldViewProjectionMatrixTriangle = Multiply(worldMatrix, Multiply(viewMatrixTriangle, projectionMatrixTriangle));
	transformationMatrixData->WVP = worldViewProjectionMatrixTriangle;
	transformationMatrixData->World = worldMatrix;
	
	vertexData[0].position = Top;
	vertexData[1].position = Right;
	vertexData[2].position = Left;
	vertexData[0].texcoord = coordTop;
	vertexData[1].texcoord = coordRight;
	vertexData[2].texcoord = coordLeft;
	vertexData[0].normal.x = vertexData[0].position.x;
	vertexData[0].normal.y = vertexData[0].position.y;
	vertexData[0].normal.z = vertexData[0].position.z;
	vertexData[1].normal.x = vertexData[1].position.x;
	vertexData[1].normal.y = vertexData[1].position.y;
	vertexData[1].normal.z = vertexData[1].position.z;
	vertexData[2].normal.x = vertexData[2].position.x;
	vertexData[2].normal.y = vertexData[2].position.y;
	vertexData[2].normal.z = vertexData[2].position.z;

}

void Particle::InputDataPlane(
	Vector4 TopLeft, Vector4 TopRight, Vector4 BottomRight, Vector4 BottomLeft, Vector4 color,
	Vector2 coordTopLeft, Vector2 coordTopRight, Vector2 coordBottomRight, Vector2 coordBottomLeft)
{
	uint32_t* indexDataPlane = nullptr;
	indexResourcePlane->Map(0, nullptr, reinterpret_cast<void**>(&indexDataPlane));

	materialDataPlane[0].color = color;
	materialDataPlane[0].uvTransform = MakeIdentity4x4();
	Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTransformPlane.scale);
	uvTransformMatrix = Multiply(uvTransformMatrix, MakerotateZMatrix(uvTransformPlane.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransformPlane.translate));
	materialDataPlane[0].uvTransform = uvTransformMatrix;

	kNumInstance = 0;
	for (uint32_t index = 0; index <kNumMaxInstance; ++index)
	{
		float alpha = 1.0f - (particlesPlane[index].currentTime / particlesPlane[index].lifeTime);
		Matrix4x4 worldMatrix =
			MakeAffineMatrix(
				particlesPlane[index].transform.scale,
				particlesPlane[index].transform.rotate,
				particlesPlane[index].transform.translate);
		if (particlesPlane[index].lifeTime <= particlesPlane[index].currentTime)
		{
			continue;
		}
		particlesPlane[index].transform.translate.x += particlesPlane[index].velocity.x * kDeltaTime;
		particlesPlane[index].transform.translate.y += particlesPlane[index].velocity.y * kDeltaTime;
		particlesPlane[index].transform.translate.z += particlesPlane[index].velocity.z * kDeltaTime;
		particlesPlane[index].currentTime += kDeltaTime;
		if (camera_)
		{
			const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
			worldViewProjectionMatrixPlane = Multiply(worldMatrix, viewProjectionMatrix);
		}
		instancingDataPlane[kNumInstance].WVP = worldViewProjectionMatrixPlane;
		instancingDataPlane[kNumInstance].World = worldMatrix;
		instancingDataPlane[kNumInstance].color = particlesPlane[index].color;
		instancingDataPlane[kNumInstance].color.w = alpha;
		++kNumInstance;
	}
	vertexDataPlane[0].position = TopLeft;
	vertexDataPlane[0].texcoord = coordTopLeft;
	vertexDataPlane[0].normal.x = vertexDataPlane[0].position.x;
	vertexDataPlane[0].normal.y = vertexDataPlane[0].position.y;
	vertexDataPlane[0].normal.z = vertexDataPlane[0].position.z;

	vertexDataPlane[1].position = TopRight;
	vertexDataPlane[1].texcoord = coordTopRight;
	vertexDataPlane[1].normal.x = vertexDataPlane[1].position.x;
	vertexDataPlane[1].normal.y = vertexDataPlane[1].position.y;
	vertexDataPlane[1].normal.z = vertexDataPlane[1].position.z;

	vertexDataPlane[2].position = BottomRight;
	vertexDataPlane[2].texcoord = coordBottomRight;
	vertexDataPlane[2].normal.x = vertexDataPlane[2].position.x;
	vertexDataPlane[2].normal.y = vertexDataPlane[2].position.y;
	vertexDataPlane[2].normal.z = vertexDataPlane[2].position.z;

	vertexDataPlane[3].position = BottomLeft;
	vertexDataPlane[3].texcoord = coordBottomLeft;
	vertexDataPlane[3].normal.x = vertexDataPlane[3].position.x;
	vertexDataPlane[3].normal.y = vertexDataPlane[3].position.y;
	vertexDataPlane[3].normal.z = vertexDataPlane[3].position.z;

	indexDataPlane[0] = 0;
	indexDataPlane[1] = 1;
	indexDataPlane[2] = 2;

	indexDataPlane[3] = 0;
	indexDataPlane[4] = 2;
	indexDataPlane[5] = 3;

}

void Particle::InputDataSphere(
    Vector4 LeftTop, Vector4 RightTop, Vector4 RightBottom, Vector4 LeftBottom, Vector4 color,
    Vector2 coordLeftTop, Vector2 coordRightTop, Vector2 coordRightBottom, Vector2 coordLeftBottom,
    uint32_t count, int32_t width, int32_t height)
{
	VertexData* vertexData = nullptr;
	vertexResourceSphere->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));

	Material* materialData = nullptr;
	materialResourceSphere->Map(0, nullptr, reinterpret_cast<void**>(&materialData));

	uint32_t* indexDataSphere = nullptr;
	indexResourceSphere->Map(0, nullptr, reinterpret_cast<void**>(&indexDataSphere));

	TransformationMatrix* transformationMatrixData = nullptr;
	transformationMatrixResourceSphere->Map(
	    0, nullptr, reinterpret_cast<void**>(&transformationMatrixData));
	projectionMatrixSphere =
	    MakePerspectiveFovMatrix(0.45f, float(width) / float(height), 0.1f, 100.0f);
	transformationMatrixData->WVP = MakeIdentity4x4();

	Matrix4x4 worldMatrix = MakeAffineMatrix(
	    transformMatrixSphere.scale, transformMatrixSphere.rotate, transformMatrixSphere.translate);
	worldViewProjectionMatrixSphere =
	    Multiply(worldMatrix, Multiply(viewMatrixSphere, projectionMatrixSphere));
	transformationMatrixData->WVP = worldViewProjectionMatrixSphere;
	transformationMatrixData->World = worldMatrix;

	materialData[0].color = color;
	materialData[0].enableLighting = true;
	materialData[0].uvTransform = MakeIdentity4x4();
	Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTransformSphere.scale);
	uvTransformMatrix = Multiply(uvTransformMatrix, MakerotateZMatrix(uvTransformSphere.rotate.z));
	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransformSphere.translate));
	materialData[0].uvTransform = uvTransformMatrix;

	vertexData[count * 4].position = LeftTop;
	vertexData[count * 4].texcoord = coordLeftTop;
	vertexData[count * 4].normal.x = vertexData[count * 4].position.x;
	vertexData[count * 4].normal.y = vertexData[count * 4].position.y;
	vertexData[count * 4].normal.z = vertexData[count * 4].position.z;
	
	vertexData[count * 4 + 1].position = RightTop;
	vertexData[count * 4 + 1].texcoord = coordRightTop;
	vertexData[count * 4 + 1].normal.x = vertexData[count * 4 + 1].position.x;
	vertexData[count * 4 + 1].normal.y = vertexData[count * 4 + 1].position.y;
	vertexData[count * 4 + 1].normal.z = vertexData[count * 4 + 1].position.z;
	
	vertexData[count * 4 + 2].position = RightBottom;
	vertexData[count * 4 + 2].texcoord = coordRightBottom;
	vertexData[count * 4 + 2].normal.x = vertexData[count * 4 + 2].position.x;
	vertexData[count * 4 + 2].normal.y = vertexData[count * 4 + 2].position.y;
	vertexData[count * 4 + 2].normal.z = vertexData[count * 4 + 2].position.z;
	
	vertexData[count * 4 + 3].position = LeftBottom;
	vertexData[count * 4 + 3].texcoord = coordLeftBottom;
	vertexData[count * 4 + 3].normal.x = vertexData[count * 4 + 3].position.x;
	vertexData[count * 4 + 3].normal.y = vertexData[count * 4 + 3].position.y;
	vertexData[count * 4 + 3].normal.z = vertexData[count * 4 + 3].position.z;

	indexDataSphere[count * 6 + 0] = count * 4 + 0;
	indexDataSphere[count * 6 + 1] = count * 4 + 1;
	indexDataSphere[count * 6 + 2] = count * 4 + 2;

	indexDataSphere[count * 6 + 3] = count * 4 + 0;
	indexDataSphere[count * 6 + 4] = count * 4 + 2;
	indexDataSphere[count * 6 + 5] = count * 4 + 3;
}


void Particle::DrawTriangle(
    Vector4 Top, Vector4 Right, Vector4 Left, Vector4 color, Vector2 coordTop, Vector2 coordRight,
    Vector2 coordLeft, bool useWorldMap) {
	InputDataTriangle(Top, Right, Left, color, coordTop, coordRight, coordLeft);
	DX12Common::GetInstance()->GetCommandList().Get()->SetPipelineState(graphicsPipelineState.Get());
	DX12Common::GetInstance()->GetCommandList().Get()->SetGraphicsRootSignature(rootSignature.Get());
	DX12Common::GetInstance()->GetCommandList().Get()->IASetVertexBuffers(0, 1, &vertexBufferViewTriangle);
	DX12Common::GetInstance()->GetCommandList().Get()->
		IASetIndexBuffer(&indexBufferViewSphere);
	DX12Common::GetInstance()->GetCommandList().Get()->IASetPrimitiveTopology(
	    D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DX12Common::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(
	    0, materialResourceTriangle->GetGPUVirtualAddress());
	DX12Common::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(
	    1, transformationMatrixResourceTriangle->GetGPUVirtualAddress());

	D3D12_CPU_DESCRIPTOR_HANDLE rtv =
	    DX12Common::GetInstance()->GetRtvHandles(DX12Common::GetInstance()->GetBackBufferIndex());
	D3D12_CPU_DESCRIPTOR_HANDLE dsv = DX12Common::GetInstance()->GetDsvHandle();

	DX12Common::GetInstance()->GetCommandList().Get()->OMSetRenderTargets(1, &rtv, false, &dsv);

	DX12Common::GetInstance()->GetCommandList().Get()->SetGraphicsRootDescriptorTable(
	    2, useWorldMap ? GetTextureSrvHandleGPU2()
	                   : GetTextureSrvHandleGPU());

	DX12Common::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(
		3, directionalLightResource->GetGPUVirtualAddress());

	DX12Common::GetInstance()->GetCommandList().Get()->DrawInstanced(3, 1, 0, 0);
}

void Particle::DrawPlane()
{
	DX12Common::GetInstance()->GetCommandList().Get()->SetPipelineState(graphicsPipelineState.Get());
	DX12Common::GetInstance()->GetCommandList().Get()->SetGraphicsRootSignature(rootSignature.Get());
	DX12Common::GetInstance()->GetCommandList().Get()->IASetPrimitiveTopology(
		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DX12Common::GetInstance()->GetCommandList().Get()->IASetVertexBuffers(0, 1, &vertexBufferViewPlane);
	DX12Common::GetInstance()->GetCommandList().Get()->
		IASetIndexBuffer(&indexBufferViewPlane);

	DX12Common::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(
		0, materialResourcePlane->GetGPUVirtualAddress());
	DX12Common::GetInstance()->GetCommandList().Get()->SetGraphicsRootDescriptorTable(
		1, instancingSrvHandleGPU);
	DX12Common::GetInstance()->GetCommandList().Get()->SetGraphicsRootDescriptorTable(
		2, TextureManager::GetInstance()->GetSRVHandleGPU(textureIndex));

	D3D12_CPU_DESCRIPTOR_HANDLE rtv =
		DX12Common::GetInstance()->GetRtvHandles(DX12Common::GetInstance()->GetBackBufferIndex());
	D3D12_CPU_DESCRIPTOR_HANDLE dsv = DX12Common::GetInstance()->GetDsvHandle();
	DX12Common::GetInstance()->GetCommandList().Get()->OMSetRenderTargets(1, &rtv, false, &dsv);

	DX12Common::GetInstance()->GetCommandList().Get()->DrawIndexedInstanced(6, kNumInstance, 0, 0, 0);
}

void Particle::DrawSphere(
	const Sphere& sphere_, Vector4 color, int32_t width, int32_t height)
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

			DX12Common::GetInstance()->GetCommandList().Get()->
				SetPipelineState(graphicsPipelineState.Get());

			DX12Common::GetInstance()->GetCommandList().Get()->
				SetGraphicsRootSignature(rootSignature.Get());

			DX12Common::GetInstance()->GetCommandList().Get()->
				IASetPrimitiveTopology(
				D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			D3D12_CPU_DESCRIPTOR_HANDLE rtv = DX12Common::GetInstance()->GetRtvHandles(
				DX12Common::GetInstance()->GetBackBufferIndex());
			D3D12_CPU_DESCRIPTOR_HANDLE dsv = DX12Common::GetInstance()->GetDsvHandle();
			DX12Common::GetInstance()->GetCommandList().Get()->
				OMSetRenderTargets(1, &rtv, false, &dsv);

			DX12Common::GetInstance()->GetCommandList().Get()->
				SetGraphicsRootDescriptorTable(
				2, TextureManager::GetInstance()->GetSRVHandleGPU(textureIndex));

			DX12Common::GetInstance()->GetCommandList().Get()->
				SetGraphicsRootConstantBufferView(
				3, directionalLightResource->GetGPUVirtualAddress());

			InputDataSphere(
				b, d, c, a, color, texcoordB, texcoordD, texcoordC, texcoordA, sphereCount, width, height);

			DX12Common::GetInstance()->GetCommandList().Get()->
				SetGraphicsRootConstantBufferView(
				0, materialResourceSphere->GetGPUVirtualAddress());

			DX12Common::GetInstance()->GetCommandList().Get()->
				SetGraphicsRootConstantBufferView(
				1, transformationMatrixResourceSphere->GetGPUVirtualAddress());

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

void Particle::MakeShaderResourceView(const DirectX::TexMetadata& metadata, const DirectX::TexMetadata& metadata2)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};
	srvDesc.Format = metadata.format;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc.Texture2D.MipLevels = UINT(metadata.mipLevels);

	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc2{};
	srvDesc2.Format = metadata2.format;
	srvDesc2.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc2.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
	srvDesc2.Texture2D.MipLevels = UINT(metadata2.mipLevels);

	const uint32_t descriptorSizeSRV = DX12Common::GetInstance()->
		GetDevice().Get()->GetDescriptorHandleIncrementSize(
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	textureSrvHandleCPU = DX12Common::GetInstance()->
		GetCPUDescriptorHandle(DX12Common::GetInstance()->
			GetSrvDescriptorHeap().Get(), descriptorSizeSRV, 1);
	textureSrvHandleGPU = DX12Common::GetInstance()->
		GetGPUDescriptorHandle(DX12Common::GetInstance()->
			GetSrvDescriptorHeap().Get(), descriptorSizeSRV, 1);

	textureSrvHandleCPU2 = DX12Common::GetInstance()->
		GetCPUDescriptorHandle(DX12Common::GetInstance()->
			GetSrvDescriptorHeap().Get(), descriptorSizeSRV, 2);
	textureSrvHandleGPU2 = DX12Common::GetInstance()->
		GetGPUDescriptorHandle(DX12Common::GetInstance()->
			GetSrvDescriptorHeap().Get(), descriptorSizeSRV, 2);

	DX12Common::GetInstance()->GetDevice().Get()->
		CreateShaderResourceView(textureResource.Get(), &srvDesc, textureSrvHandleCPU);
	DX12Common::GetInstance()->GetDevice().Get()->
		CreateShaderResourceView(textureResource2.Get(), &srvDesc2, textureSrvHandleCPU2);
}

void Particle::MakeShaderResourceViewInstance(ID3D12Resource* instancingResource)
{
	D3D12_SHADER_RESOURCE_VIEW_DESC instancingSrvDesc{};
	instancingSrvDesc.Format = DXGI_FORMAT_UNKNOWN;
	instancingSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	instancingSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_BUFFER;
	instancingSrvDesc.Buffer.FirstElement = 0;
	instancingSrvDesc.Buffer.Flags = D3D12_BUFFER_SRV_FLAG_NONE;
	instancingSrvDesc.Buffer.NumElements =kNumMaxInstance;
	instancingSrvDesc.Buffer.StructureByteStride = sizeof(ParticleForGPU);

	const uint32_t descriptorSizeSRV = DX12Common::GetInstance()->
		GetDevice().Get()->GetDescriptorHandleIncrementSize(
			D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);

	instancingSrvHandleCPU = DX12Common::GetInstance()->
		GetCPUDescriptorHandle(DX12Common::GetInstance()->
			GetSrvDescriptorHeap().Get(), descriptorSizeSRV, 11);
	instancingSrvHandleGPU = DX12Common::GetInstance()->
		GetGPUDescriptorHandle(DX12Common::GetInstance()->
			GetSrvDescriptorHeap().Get(), descriptorSizeSRV, 11);

	DX12Common::GetInstance()->GetDevice().Get()->CreateShaderResourceView(
		instancingResource, &instancingSrvDesc, instancingSrvHandleCPU);
}

Particle::Particles Particle::MakeNewParticle(std::mt19937& randomEngine)
{
	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
	std::uniform_real_distribution<float> distTime(1.0f, 3.0f);
	Particles particle;
	particle.transform.scale = { 1.0f,1.0f,1.0f };
	particle.transform.rotate = { 0.0f,0.0f,0.0f };
	particle.transform.translate = { distribution(randomEngine),distribution(randomEngine),distribution(randomEngine) };
	particle.velocity = { distribution(randomEngine) ,distribution(randomEngine) ,distribution(randomEngine) };
	particle.color = { distColor(randomEngine) ,distColor(randomEngine) ,distColor(randomEngine) ,1.0f };
	particle.lifeTime = distTime(randomEngine);
	particle.currentTime = 0;
	return particle;
}

DirectX::ScratchImage Particle::LoadTexture(const std::string& filePath)
{
	DirectX::ScratchImage image{};
	std::wstring filePathW = debug_->ConvertString(filePath);
	HRESULT hr = DirectX::LoadFromWICFile(
		filePathW.c_str(),
		DirectX::WIC_FLAGS_FORCE_SRGB,
		nullptr,
		image);
	assert(SUCCEEDED(hr));

	DirectX::ScratchImage mipImages{};
	hr = DirectX::GenerateMipMaps(
		image.GetImages(),
		image.GetImageCount(),
		image.GetMetadata(),
		DirectX::TEX_FILTER_SRGB,
		0,
		mipImages);

	return mipImages;
}

ComPtr<ID3D12Resource> Particle::CreateTextureResource(ID3D12Device* device, const DirectX::TexMetadata& metadata)
{
	D3D12_RESOURCE_DESC resourceDesc{};
	resourceDesc.Width = UINT(metadata.width);
	resourceDesc.Height = UINT(metadata.height);
	resourceDesc.MipLevels = UINT16(metadata.mipLevels);
	resourceDesc.DepthOrArraySize = UINT16(metadata.arraySize);
	resourceDesc.Format = metadata.format;
	resourceDesc.SampleDesc.Count = 1;
	resourceDesc.Dimension = D3D12_RESOURCE_DIMENSION(metadata.dimension);

	D3D12_HEAP_PROPERTIES heapProperties{};
	heapProperties.Type = D3D12_HEAP_TYPE_CUSTOM;
	heapProperties.CPUPageProperty = D3D12_CPU_PAGE_PROPERTY_WRITE_BACK;
	heapProperties.MemoryPoolPreference = D3D12_MEMORY_POOL_L0;

	ComPtr<ID3D12Resource> resource = nullptr;
	HRESULT hr = device->CreateCommittedResource(
		&heapProperties,
		D3D12_HEAP_FLAG_NONE,
		&resourceDesc,
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&resource));

	assert(SUCCEEDED(hr));

	return resource;
}

void Particle::UploadTextureData(ID3D12Resource* texture, const DirectX::ScratchImage& mipImages, const DirectX::TexMetadata& metadata)
{
	for (size_t mipLevel = 0; mipLevel < metadata.mipLevels; mipLevel++)
	{
		const DirectX::Image* img = mipImages.GetImage(mipLevel, 0, 0);
		HRESULT hr = texture->WriteToSubresource(
			UINT(mipLevel),
			nullptr,
			img->pixels,
			UINT(img->rowPitch),
			UINT(img->slicePitch)
		);
		assert(SUCCEEDED(hr));
	}
}


