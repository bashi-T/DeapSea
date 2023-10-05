#include "Mesh.h"

Mesh::~Mesh()
{
	delete dxcUtils;
	delete dxcCompiler;
	delete includeHandler;
	delete rootSignature;
	delete graphicsPipelineState;
}

void Mesh::Initialize(int32_t width, int32_t height)
{
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;
	viewport.Width = width;
	viewport.Height = height;
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;

	scissorRect.left = 0.0f;
	scissorRect.right = width;
	scissorRect.top = 0.0f;
	scissorRect.bottom = height;

	Mesh::ResetDXC();

	Mesh::MakePSO();

	transformMatrix={ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	cameraTransform={ {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-5.0f} };
	projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(width) / float(height), 0.1f, 100.0f);
}

void Mesh::ResetDXC()
{
	hr = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(&dxcUtils));
	assert(SUCCEEDED(hr));
	hr = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(&dxcCompiler));
	assert(SUCCEEDED(hr));
	hr = dxcUtils->CreateDefaultIncludeHandler(&includeHandler);
	assert(SUCCEEDED(hr));
}

IDxcBlob* Mesh::CompileShader(
	const std::wstring& filePath,
	const wchar_t* profile,
	IDxcUtils* dxcUtils_,
    IDxcCompiler3* dxcCompiler_,
	IDxcIncludeHandler* includeHandler_
	)
{
	debug_->Log(debug_->ConvertString(std::format(
		L"Begin CompileShader,path{},\n", filePath, profile)));
	IDxcBlobEncoding* shaderSource = nullptr;
	hr = dxcUtils_->LoadFile(filePath.c_str(), nullptr, &shaderSource);
	assert(SUCCEEDED(hr));

	DxcBuffer shaderSourceBuffer;
	shaderSourceBuffer.Ptr = shaderSource->GetBufferPointer();
	shaderSourceBuffer.Size = shaderSource->GetBufferSize();
	shaderSourceBuffer.Encoding = DXC_CP_UTF8;

	LPCWSTR arguments[]
	{
		filePath.c_str(),
		L"-E",
		L"main",
		L"-T",
		profile,
		L"-Zi",
		L"-Qembed_debug",
		L"-Od",
		L"-Zpr",
	};
	IDxcResult* shaderResult = nullptr;
	hr = dxcCompiler_->Compile(
		&shaderSourceBuffer,
		arguments,
		_countof(arguments),
		includeHandler_,
		IID_PPV_ARGS(&shaderResult)
	);
	assert(SUCCEEDED(hr));

	IDxcBlobUtf8* shaderError = nullptr;
	shaderResult->GetOutput(DXC_OUT_ERRORS,
		IID_PPV_ARGS(&shaderError), nullptr);
	if (shaderError != nullptr && shaderError->GetStringLength() != 0)
	{
		debug_->Log(shaderError->GetStringPointer());
		assert(SUCCEEDED(hr));
	}
	IDxcBlob* shaderBlob = nullptr;
	hr = shaderResult->GetOutput(DXC_OUT_OBJECT,
		IID_PPV_ARGS(&shaderBlob), nullptr);
	assert(SUCCEEDED(hr));
	debug_->Log(debug_->ConvertString(std::format(
		L"Compile Succeded,path:{}\n", filePath, profile)));
	shaderSource->Release();
	shaderResult->Release();
	return shaderBlob;
}

void Mesh::MakePSO()
{
		descriptionRootSignature_.Flags =
			D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
	
		D3D12_ROOT_PARAMETER rootParameters[3] = {};
	    rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	    rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	    rootParameters[0].Descriptor.ShaderRegister = 0;
	    rootParameters[1].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	    rootParameters[1].ShaderVisibility = D3D12_SHADER_VISIBILITY_VERTEX;
	    rootParameters[1].Descriptor.ShaderRegister = 0;

		D3D12_DESCRIPTOR_RANGE descriptorRange[1] = {};
		descriptorRange[0].BaseShaderRegister = 0;
		descriptorRange[0].NumDescriptors = 1;
		descriptorRange[0].RangeType = D3D12_DESCRIPTOR_RANGE_TYPE_SRV;
		descriptorRange[0].OffsetInDescriptorsFromTableStart = D3D12_DESCRIPTOR_RANGE_OFFSET_APPEND;
		
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

		hr = D3D12SerializeRootSignature(&descriptionRootSignature_,
			D3D_ROOT_SIGNATURE_VERSION_1, &signatureBlob, &errorBlob);
	
		if (FAILED(hr))
		{
			debug_->Log(reinterpret_cast<char*>(errorBlob->GetBufferPointer()));
			assert(false);
		}
	
		hr = DX12Common::GetInstance()->GetDevice()->CreateRootSignature(
	        0,
			signatureBlob->GetBufferPointer(),
			signatureBlob->GetBufferSize(),
	        IID_PPV_ARGS(&rootSignature));

		inputElementDescs[0].SemanticName = "POSITION";
		inputElementDescs[0].SemanticIndex = 0;
		inputElementDescs[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
		inputElementDescs[0].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;
		inputElementDescs[1].SemanticName = "TEXCOORD";
		inputElementDescs[1].SemanticIndex = 0;
		inputElementDescs[1].Format = DXGI_FORMAT_R32G32_FLOAT;
		inputElementDescs[1].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputLayoutDesc.pInputElementDescs = inputElementDescs;
		inputLayoutDesc.NumElements = _countof(inputElementDescs);
	
		blendDesc.RenderTarget[0].RenderTargetWriteMask =
			D3D12_COLOR_WRITE_ENABLE_ALL;
	
		rasterizerDesc.CullMode = D3D12_CULL_MODE_BACK;
		rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;
	
		vertexShaderBlob = CompileShader(L"Object3d.VS.hlsl", L"vs_6_0",
			dxcUtils, dxcCompiler, includeHandler);
		assert(vertexShaderBlob != nullptr);
	
		pixelShaderBlob = CompileShader(L"Object3d.PS.hlsl", L"ps_6_0",
			dxcUtils, dxcCompiler, includeHandler);
		assert(pixelShaderBlob != nullptr);
	
		graphicsPipelineStateDesc.pRootSignature = rootSignature;
		graphicsPipelineStateDesc.InputLayout = inputLayoutDesc;
		graphicsPipelineStateDesc.VS =
		{
			vertexShaderBlob->GetBufferPointer(),
			vertexShaderBlob->GetBufferSize()
		};
		graphicsPipelineStateDesc.PS =
		{
			pixelShaderBlob->GetBufferPointer(),
			pixelShaderBlob->GetBufferSize()
		};
		graphicsPipelineStateDesc.BlendState = blendDesc;
		graphicsPipelineStateDesc.RasterizerState = rasterizerDesc;
	
		graphicsPipelineStateDesc.NumRenderTargets = 1;
		graphicsPipelineStateDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM_SRGB;
		graphicsPipelineStateDesc.PrimitiveTopologyType =
			D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	
		graphicsPipelineStateDesc.SampleDesc.Count = 1;
		graphicsPipelineStateDesc.SampleMask = D3D12_DEFAULT_SAMPLE_MASK;
	
		hr = DX12Common::GetInstance()->GetDevice()->
			CreateGraphicsPipelineState(
			&graphicsPipelineStateDesc,
			IID_PPV_ARGS(&graphicsPipelineState));
		assert(SUCCEEDED(hr));
}

void Mesh::Update()
{
	    vertexResource = CreateBufferResource(sizeof(VertexData) * 3);
	    MakeVertexBufferView();
	    materialResource = CreateBufferResource(sizeof(Vector4));
	    wvpResource=CreateBufferResource(sizeof(Matrix4x4));


}

ID3D12Resource* Mesh::CreateBufferResource(size_t sizeInBytes) {
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

	ID3D12Resource* Resource = nullptr;

	hr = DX12Common::GetInstance()->GetDevice()->CreateCommittedResource(
	    &uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &ResourceDesc,
	    D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&Resource));
	assert(SUCCEEDED(hr));
	return Resource;
}

void Mesh::MakeVertexBufferView()
{
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(VertexData) * 3;
	vertexBufferView.StrideInBytes = sizeof(VertexData);
}

void Mesh::InputDataTriangle(
	Vector4 Top,
	Vector4 Right,
	Vector4 Left,
	Vector4 color,
	Vector2 coordTop,
	Vector2 coordRight,
	Vector2 coordLeft)
{
	VertexData* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	*materialData = color;
	*wvpData = MakeIdentity4x4();

	//transformMatrix.rotate.y += 0.03f;
	Matrix4x4 worldMatrix = MakeAffineMatrix(transformMatrix.scale, transformMatrix.rotate, transformMatrix.translate);
	cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);
	viewMatrix = Inverse(cameraMatrix);
	worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	*wvpData = worldViewProjectionMatrix;

	vertexData[0].position = Top;
	vertexData[1].position = Right;
	vertexData[2].position = Left;
	vertexData[0].texcoord = coordTop;
	vertexData[1].texcoord = coordRight;
	vertexData[2].texcoord = coordLeft;
}

void Mesh::DrawTriangle() {
	DX12Common::GetInstance()->GetCommandList()->ClearRenderTargetView(
	    DX12Common::GetInstance()->
		GetRtvHandles(DX12Common::GetInstance()->GetBackBufferIndex()),
	    clearColor, 0, nullptr);
}

void Mesh::Draw(
	Vector4 Top,
	Vector4 Right,
	Vector4 Left,
	Vector4 color,
	Vector2 coordTop,
	Vector2 coordRight,
	Vector2 coordLeft)
{
	DX12Common::GetInstance()->GetCommandList()->
		RSSetViewports(1, &viewport);

	DX12Common::GetInstance()->GetCommandList()->
		RSSetScissorRects(1, &scissorRect);

		InputDataTriangle(
			Top,
			Right,
			Left,
			color,
			coordTop,
			coordRight,
			coordLeft);

	DX12Common::GetInstance()->GetCommandList()->
		SetPipelineState(graphicsPipelineState);

	DX12Common::GetInstance()->GetCommandList()->
		SetGraphicsRootSignature(rootSignature);

	DX12Common::GetInstance()->GetCommandList()->
		IASetVertexBuffers(0, 1, &vertexBufferView);

	DX12Common::GetInstance()->GetCommandList()->
		IASetPrimitiveTopology( D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DX12Common::GetInstance()->GetCommandList()->
	    SetGraphicsRootConstantBufferView(0,
			materialResource->GetGPUVirtualAddress());

	DX12Common::GetInstance()->GetCommandList()->
	    SetGraphicsRootConstantBufferView(1,
			wvpResource->GetGPUVirtualAddress());

	DX12Common::GetInstance()->GetCommandList()->
		SetGraphicsRootDescriptorTable(2,
			DX12Common::GetInstance()->GetTextureSrvHandleGPU());

	DX12Common::GetInstance()->GetCommandList()->
		DrawInstanced(3, 1, 0, 0);


}

void Mesh::MeshRelease()
{
	vertexResource->Release();
	materialResource->Release();
	wvpResource->Release();
	graphicsPipelineState->Release();
	signatureBlob->Release();
	if (errorBlob)
	{
		errorBlob->Release();
	}
	rootSignature->Release();
	pixelShaderBlob->Release();
	vertexShaderBlob->Release();
}



