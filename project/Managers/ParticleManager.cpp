//#include "ParticleManager.h"
//
//void ParticleManager::Initialize(DX12Common* dxCommon, SRVManager* srvManager)
//{
//	this->dx12Common_ = dxCommon;
//	this->srvManager_ = srvManager;
//	ResetDXC();
//
//	MakePSO(dx12Common_);
//
//
//	this->camera_ = object3dCommon_->GetDefaultCamera();
//	kNumMaxInstance = 10;
//	std::random_device seedGenerator;
//	std::mt19937 randomEngine(seedGenerator());
//	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
//
//	LeftTop[0] = { -0.5f, 0.5f, 0.0f, 1.0f };
//	RightTop[0] = { 0.5f, 0.5f, 0.0f, 1.0f };
//	RightBottom[0] = { 0.5f, -0.5f, 0.0f, 1.0f };
//	LeftBottom[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
//	Color[0] = { 1.0f, 1.0f, 1.0f, 1.0f };
//	texcoordLeftTop[0] = { 0.0f,0.0f };
//	texcoordRightTop[0] = { 0.0f,1.0f };
//	texcoordRightBottom[0] = { 1.0f,1.0f };
//	texcoordLeftBottom[0] = { 1.0f,0.0f };
//
//	for (uint32_t name = 0; name < kNumMaxInstance; ++name)
//	{
//		for (uint32_t index = 0; index < kNumMaxInstance; ++index)
//		{
//
//			particleGroups.particle = MakeNewParticle(randomEngine);
//			particleGroups.instancingData.WVP = MakeIdentity4x4();
//			instancingData.World = MakeIdentity4x4();
//			instancingData.color = particles.color;
//		}
//	}
//	MakeBufferView();
//	vertexResource = CreateBufferResource(sizeof(VertexData) * 6);
//	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
//
//}
//
//void ParticleManager::Update()
//{
//
//	InputData(LeftTop[0], RightTop[0], RightBottom[0], LeftBottom[0], Color[0],
//		texcoordLeftTop[0], texcoordRightTop[0], texcoordRightBottom[0], texcoordLeftBottom[0]);
//}
//
//void ParticleManager::Draw()
//{
//	DX12Common::GetInstance()->GetCommandList().Get()->SetPipelineState(graphicsPipelineState.Get());
//	DX12Common::GetInstance()->GetCommandList().Get()->SetGraphicsRootSignature(rootSignature.Get());
//	DX12Common::GetInstance()->GetCommandList().Get()->IASetPrimitiveTopology(
//		D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
//
//	DX12Common::GetInstance()->GetCommandList().Get()->IASetVertexBuffers(0, 1, &vertexBufferView);
//	DX12Common::GetInstance()->GetCommandList().Get()->
//		IASetIndexBuffer(&indexBufferView);
//
//	DX12Common::GetInstance()->GetCommandList().Get()->SetGraphicsRootConstantBufferView(
//		0, materialResource->GetGPUVirtualAddress());
//	DX12Common::GetInstance()->GetCommandList().Get()->
//		SetGraphicsRootDescriptorTable(
//			1, instancingSrvHandleGPU);
//
//	srvManager_->SetGraphicsRootDescriptorTable(
//		2, particleGroups);
//
//	//DX12Common::GetInstance()->GetCommandList().Get()->
//	//	SetGraphicsRootDescriptorTable(
//	//	2, TextureManager::GetInstance()->GetSRVHandleGPU(textureIndex));
//
//	D3D12_CPU_DESCRIPTOR_HANDLE rtv =
//		DX12Common::GetInstance()->GetRtvHandles(srvManager_->GetBackBufferIndex());
//	D3D12_CPU_DESCRIPTOR_HANDLE dsv = DX12Common::GetInstance()->GetDsvHandle();
//	DX12Common::GetInstance()->GetCommandList().Get()->OMSetRenderTargets(1, &rtv, false, &dsv);
//
//	DX12Common::GetInstance()->GetCommandList().Get()->DrawIndexedInstanced(6, kNumInstance, 0, 0, 0);
//
//}
//
//void ParticleManager::ResetDXC()
//{
//	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
//	assert(SUCCEEDED(hr));
//	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
//	assert(SUCCEEDED(hr));
//	hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
//	assert(SUCCEEDED(hr));
//}
//
//ComPtr<IDxcBlob> ParticleManager::CompileShader(
//	const std::wstring& filePath,
//	const wchar_t* profile,
//	IDxcUtils* dxcUtils_,
//	IDxcCompiler3* dxcCompiler_,
//	IDxcIncludeHandler* includeHandler_)
//{
//	debug_->Log(
//		debug_->ConvertString(std::format(L"Begin CompileShader,path{},\n", filePath, profile)));
//	ComPtr<IDxcBlobEncoding> shaderSource = nullptr;
//	hr = dxcUtils_->LoadFile(filePath.c_str(), nullptr, shaderSource.GetAddressOf());
//	assert(SUCCEEDED(hr));
//
//	DxcBuffer shaderSourceBuffer;
//	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
//	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
//	shaderSourceBuffer.Encoding = DXC_CP_UTF8;
//
//	LPCWSTR arguments[]{
//		filePath.c_str(), L"-E", L"main", L"-T", profile, L"-Zi", L"-Qembed_debug", L"-Od", L"-Zpr",
//	};
//	ComPtr<IDxcResult> shaderResult = nullptr;
//	hr = dxcCompiler_->Compile(
//		&shaderSourceBuffer, arguments, _countof(arguments), includeHandler_,
//		IID_PPV_ARGS(&shaderResult));
//	assert(SUCCEEDED(hr));
//
//	ComPtr<IDxcBlobUtf8> shaderError = nullptr;
//	shaderResult->GetOutput(DXC_OUT_ERRORS, IID_PPV_ARGS(&shaderError), nullptr);
//	if (shaderError != nullptr && shaderError->GetStringLength() != 0) {
//		debug_->Log(shaderError->GetStringPointer());
//		assert(SUCCEEDED(hr));
//	}
//	ComPtr<IDxcBlob> shaderBlob = nullptr;
//	hr = shaderResult->GetOutput(DXC_OUT_OBJECT, IID_PPV_ARGS(&shaderBlob), nullptr);
//	assert(SUCCEEDED(hr));
//	debug_->Log(
//		debug_->ConvertString(std::format(L"Compile Succeded,path:{}\n", filePath, profile)));
//	shaderSource->Release();
//	shaderResult->Release();
//	return shaderBlob;
//}
//
//void ParticleManager::MakePSO(DX12Common* dxCommon)
//{
//	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_{};
//	descriptionRootSignature_.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
//
//	D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
//	descriptorRange[0].BaseShaderRegister = 0;
//	descriptorRange[0].NumDescriptors = 1;
//	descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
//	descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
//
//	D3D12_ROOT_PARAMETER rootParameters[3] = {};
//	rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
//	rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
//	rootParameters[0].Descriptor.ShaderRegister = 0;
//
//	rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
//	rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
//	rootParameters[1].DescriptorTable.pDescriptorRanges = descriptorRange;
//	rootParameters[1].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);
//
//	rootParameters[2].ParameterType = D3D12_ROOT_PARAMETER_TYPE_DESCRIPTOR_TABLE;
//	rootParameters[2].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
//	rootParameters[2].DescriptorTable.pDescriptorRanges = descriptorRange;
//	rootParameters[2].DescriptorTable.NumDescriptorRanges = _countof(descriptorRange);
//
//	descriptionRootSignature_.pParameters = rootParameters;
//	descriptionRootSignature_.NumParameters = _countof(rootParameters);
//
//	D3D12_STATIC_SAMPLER_DESC staticSamplers[1] = {};
//	staticSamplers[0].Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
//	staticSamplers[0].AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//	staticSamplers[0].AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//	staticSamplers[0].AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
//	staticSamplers[0].ComparisonFunc = D3D12_COMPARISON_FUNC_NEVER;
//	staticSamplers[0].MaxLOD = D3D12_FLOAT32_MAX;
//	staticSamplers[0].ShaderRegister = 0;
//	staticSamplers[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
//	descriptionRootSignature_.pStaticSamplers = staticSamplers;
//	descriptionRootSignature_.NumStaticSamplers = _countof(staticSamplers);
//
//	hr = D3D12SerializeRootSignature(
//		&descriptionRootSignature_, D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
//
//	if (FAILED(hr))
//	{
//		debug_->Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
//		assert(false);
//	}
//
//	hr = dxCommon->GetDevice().Get()->CreateRootSignature(
//		0, signatureBlob->GetBufferPointer(), signatureBlob->GetBufferSize(),
//		IID_PPV_ARGS(&rootSignature));
//
//	D3D12_INPUT_ELEMENT_DESC inputElementDescs[3] = {};
//	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
//
//	inputElementDescs[0].SemanticName = "POSITION";
//	inputElementDescs[0].SemanticIndex = 0;
//	inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
//	inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
//	inputElementDescs[1].SemanticName = "TEXCOORD";
//	inputElementDescs[1].SemanticIndex = 0;
//	inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
//	inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
//	inputElementDescs[2].SemanticName = "NORMAL";
//	inputElementDescs[2].SemanticIndex = 0;
//	inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
//	inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
//
//	inputLayoutDesc.pInputElementDescs = inputElementDescs;
//	inputLayoutDesc.NumElements = _countof(inputElementDescs);
//
//	D3D12_BLEND_DESC blendDesc{};
//	blendDesc.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;
//	blendDesc.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
//	blendDesc.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
//	blendDesc.RenderTarget[0].DestBlend = D3D12_BLEND_ONE;
//	blendDesc.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
//	blendDesc.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
//	blendDesc.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
//
//	D3D12_RASTERIZER_DESC rasterizerDesc{};
//	rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
//	rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
//
//	ComPtr<IDxcBlob> vertexShaderBlob = nullptr;
//	ComPtr<IDxcBlob> pixelShaderBlob = nullptr;
//	vertexShaderBlob =
//		CompileShader(L"HLSL/Particle.VS.hlsl", L"vs_6_0", dxcUtils.Get(), dxcCompiler.Get(), includeHandler.Get());
//	assert(vertexShaderBlob != nullptr);
//
//	pixelShaderBlob =
//		CompileShader(L"HLSL/Particle.PS.hlsl", L"ps_6_0", dxcUtils.Get(), dxcCompiler.Get(), includeHandler.Get());
//	assert(pixelShaderBlob != nullptr);
//
//	D3D12_DEPTH_STENCIL_DESC depthStencilDesc{};
//	depthStencilDesc.DepthEnable = true;
//	depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ZERO;
//	depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;
//
//	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
//	graphicsPipelineStateDesc.pRootSignature = rootSignature.Get();
//	graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
//	graphicsPipelineStateDesc.VS = {
//		vertexShaderBlob->GetBufferPointer(), vertexShaderBlob->GetBufferSize() };
//	graphicsPipelineStateDesc.PS = {
//		pixelShaderBlob->GetBufferPointer(), pixelShaderBlob->GetBufferSize() };
//	graphicsPipelineStateDesc.BlendState = blendDesc;
//	graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;
//
//	graphicsPipelineStateDesc.NumRenderTargets = 1;
//	graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
//	graphicsPipelineStateDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
//
//	graphicsPipelineStateDesc.SampleDesc.Count = 1;
//	graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
//
//	graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
//	graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
//
//	hr = DX12Common::GetInstance()->GetDevice().Get()->CreateGraphicsPipelineState(
//		&graphicsPipelineStateDesc, IID_PPV_ARGS(&graphicsPipelineState));
//	assert(SUCCEEDED(hr));
//}
//
//void ParticleManager::CreateParticleGroup(const std::string name, const std::string textureFilePath)
//{
//	if (particleGroups.contains(textureFilePath))
//	{
//		return;
//	}
//
//	assert(srvManager_->CheckNumTexture(particleGroups.size()));
//
//	DirectX::ScratchImage image{};//テクスチャファイルをプログラムで扱えるように
//	std::wstring filePathW = debug_->ConvertString(textureFilePath);
//	HRESULT hr = DirectX::LoadFromWICFile(
//		filePathW.c_str(),
//		DirectX::WIC_FLAGS_FORCE_SRGB,
//		nullptr,
//		image);
//	assert(SUCCEEDED(hr));
//
//	DirectX::ScratchImage mipImages{};//MipMapの作成
//	hr = DirectX::GenerateMipMaps(
//		image.GetImages(),
//		image.GetImageCount(),
//		image.GetMetadata(),
//		DirectX::TEX_FILTER_SRGB,
//		0,
//		mipImages);
//	assert(SUCCEEDED(hr));
//
//	ParticleGroup& particleGroup = particleGroups[name];
//	particleGroup.materialData.textureFilePath = textureFilePath;
//	TextureManager::GetInstance()->LoadTexture(particleGroup.materialData.textureFilePath);
//	particleGroup.materialData.textureIndex = 
//		TextureManager::GetInstance()->GetSrvIndex(textureFilePath);
//	particleGroup.instancingResource= CreateBufferResource(sizeof(ParticleForGPU) * kNumMaxInstance);
//	particleGroup.srvIndex = srvManager_->Allocate();
//	srvManager_->CreateSRVforStructuredBuffer(
//		particleGroup.materialData.textureIndex,
//		particleGroup.instancingResource.Get(),
//		particleGroup.kNumInstance,
//		sizeof(particleGroup));
//}
//
//ComPtr<ID3D12Resource> ParticleManager::CreateBufferResource(size_t sizeInBytes)
//{
//	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
//
//	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
//	D3D12_RESOURCE_DESC ResourceDesc{};
//
//	ResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
//
//	ResourceDesc.Width = sizeInBytes * 3;
//
//	ResourceDesc.Height = 1;
//	ResourceDesc.DepthOrArraySize = 1;
//	ResourceDesc.MipLevels = 1;
//	ResourceDesc.SampleDesc.Count = 1;
//
//	ResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;
//
//	ComPtr<ID3D12Resource> Resource = nullptr;
//
//	hr = DX12Common::GetInstance()->GetDevice().Get()->CreateCommittedResource(
//		&uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &ResourceDesc,
//		D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&Resource));
//	assert(SUCCEEDED(hr));
//	return Resource;
//}
//
//ParticleManager::Particles ParticleManager::MakeNewParticle(std::mt19937& randomEngine)
//{
//	std::uniform_real_distribution<float> distribution(-1.0f, 1.0f);
//	std::uniform_real_distribution<float> distColor(0.0f, 1.0f);
//	std::uniform_real_distribution<float> distTime(1.0f, 3.0f);
//	Particles particle;
//	particle.transform.scale = { 1.0f,1.0f,1.0f };
//	particle.transform.rotate = { 0.0f,0.0f,0.0f };
//	particle.transform.translate = { distribution(randomEngine),distribution(randomEngine),distribution(randomEngine) };
//	particle.velocity = { distribution(randomEngine) ,distribution(randomEngine) ,distribution(randomEngine) };
//	particle.color = { distColor(randomEngine) ,distColor(randomEngine) ,distColor(randomEngine) ,1.0f };
//	particle.lifeTime = distTime(randomEngine);
//	particle.currentTime = 0;
//	return particle;
//}
//void ParticleManager::InputData(
//	Vector4 TopLeft, Vector4 TopRight, Vector4 BottomRight, Vector4 BottomLeft, Vector4 color,
//	Vector2 coordTopLeft, Vector2 coordTopRight, Vector2 coordBottomRight, Vector2 coordBottomLeft)
//{
//	uint32_t* indexData = nullptr;
//	indexResource->Map(0, nullptr, reinterpret_cast<void**>(&indexData));
//
//	materialData[0].color = color;
//	materialData[0].uvTransform = MakeIdentity4x4();
//	Matrix4x4 uvTransformMatrix = MakeScaleMatrix(uvTransform.scale);
//	uvTransformMatrix = Multiply(uvTransformMatrix, MakerotateZMatrix(uvTransform.rotate.z));
//	uvTransformMatrix = Multiply(uvTransformMatrix, MakeTranslateMatrix(uvTransform.translate));
//	materialData[0].uvTransform = uvTransformMatrix;
//
//	particleGroups;
//	for (uint32_t index = 0; index < kNumMaxInstance; ++index)
//	{
//		float alpha = 1.0f - (particles[index].currentTime / particles[index].lifeTime);
//		Matrix4x4 worldMatrix =
//			MakeAffineMatrix(
//				particles[index].transform.scale,
//				particles[index].transform.rotate,
//				particles[index].transform.translate);
//		if (particles[index].lifeTime <= particles[index].currentTime)
//		{
//			continue;
//		}
//		particles[index].transform.translate.x += particles[index].velocity.x * kDeltaTime;
//		particles[index].transform.translate.y += particles[index].velocity.y * kDeltaTime;
//		particles[index].transform.translate.z += particles[index].velocity.z * kDeltaTime;
//		particles[index].currentTime += kDeltaTime;
//		if (camera_)
//		{
//			const Matrix4x4& viewProjectionMatrix = camera_->GetViewProjectionMatrix();
//			worldViewProjectionMatrix = Multiply(worldMatrix, viewProjectionMatrix);
//		}
//		instancingData[kNumInstance].WVP = worldViewProjectionMatrix;
//		instancingData[kNumInstance].World = worldMatrix;
//		instancingData[kNumInstance].color = particles[index].color;
//		particles[index].color.a = alpha;
//		instancingData[kNumInstance].color.a = alpha;
//		++kNumInstance;
//	}
//
//	//float left = 0.0f - anchorPoint.x;
//	//float right = 1.0f - anchorPoint.x;
//	//float top = 0.0f - anchorPoint.y;
//	//float bottom = 1.0f - anchorPoint.y;
//	//
//	//if (isFlipX_)
//	//{
//	//	left = -left;
//	//	right = -right;
//	//}
//	//if (isFlipY_)
//	//{
//	//	top = -top;
//	//	bottom = -bottom;
//	//}
//	//const DirectX::TexMetadata& metadata =
//	//	TextureManager::GetInstance()->GetMetaData(textureIndex);
//	//float tex_left = TopLeft.x / metadata.width;
//	//float tex_right = (TopLeft.x + textureSize.x) / metadata.width;
//	//float tex_top = TopLeft.y / metadata.width;
//	//float tex_bottom = (TopLeft.y + textureSize.y) / metadata.width;
//	//
//	//vertexData[0].position = TopLeft;
//	//vertexData[1].position = TopRight;
//	//vertexData[2].position = BottomRight;
//	//vertexData[3].position = BottomLeft;
//	//vertexData[0].texcoord = { tex_left,tex_top };
//	//vertexData[1].texcoord = { tex_right,tex_top };
//	//vertexData[2].texcoord = { tex_right,tex_bottom };
//	//vertexData[3].texcoord = { tex_left,tex_bottom };
//	//
//	//vertexData[0].normal = { vertexData[0].position.x, vertexData[0].position.y, -vertexData[0].position.z };
//	//vertexData[1].normal = { vertexData[1].position.x, vertexData[1].position.y, -vertexData[1].position.z };
//	//vertexData[2].normal = { vertexData[2].position.x, vertexData[2].position.y, -vertexData[2].position.z };
//	//vertexData[3].normal = { vertexData[3].position.x, vertexData[3].position.y, -vertexData[3].position.z };
//
//	vertexData[0].position = TopLeft;
//	vertexData[0].texcoord = coordTopLeft;
//	vertexData[0].normal.x = vertexData[0].position.x;
//	vertexData[0].normal.y = vertexData[0].position.y;
//	vertexData[0].normal.z = vertexData[0].position.z;
//
//	vertexData[1].position = TopRight;
//	vertexData[1].texcoord = coordTopRight;
//	vertexData[1].normal.x = vertexData[1].position.x;
//	vertexData[1].normal.y = vertexData[1].position.y;
//	vertexData[1].normal.z = vertexData[1].position.z;
//
//	vertexData[2].position = BottomRight;
//	vertexData[2].texcoord = coordBottomRight;
//	vertexData[2].normal.x = vertexData[2].position.x;
//	vertexData[2].normal.y = vertexData[2].position.y;
//	vertexData[2].normal.z = vertexData[2].position.z;
//
//	vertexData[3].position = BottomLeft;
//	vertexData[3].texcoord = coordBottomLeft;
//	vertexData[3].normal.x = vertexData[3].position.x;
//	vertexData[3].normal.y = vertexData[3].position.y;
//	vertexData[3].normal.z = vertexData[3].position.z;
//
//	indexData[0] = 0;
//	indexData[1] = 1;
//	indexData[2] = 2;
//
//	indexData[3] = 0;
//	indexData[4] = 2;
//	indexData[5] = 3;
//
//}
//
//void ParticleManager::MakeBufferView()
//{
//	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
//	vertexBufferView.SizeInBytes = UINT(sizeof(VertexData)) * 6;
//	vertexBufferView.StrideInBytes = sizeof(VertexData);
//
//	indexBufferView.BufferLocation = indexResource->GetGPUVirtualAddress();
//	indexBufferView.SizeInBytes = sizeof(uint32_t) * 6;
//	indexBufferView.Format = DXGI_FORMAT_R32_UINT;
//}
