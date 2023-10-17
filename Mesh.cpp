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

	transformMatrix = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
	cameraTransform = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,-15.0f} };
	projectionMatrix = MakePerspectiveFovMatrix(0.45f, float(width) / float(height), 0.1f, 100.0f);
	cameraMatrix = MakeAffineMatrix(cameraTransform.scale, cameraTransform.rotate, cameraTransform.translate);

	transformationMatrixResourceSprite = CreateBufferResource(sizeof(Matrix4x4));
	transformationMatrixResourceSprite->Map(
		0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSprite));
	*transformationMatrixDataSprite = MakeIdentity4x4();
	projectionMatrixSprite = MakeOrthographicMatrix(0.0f, 0.0f, float(width), float(height), 0.0f, 100.0f);
	transformMatrixSprite = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };

	transformationMatrixResourceSphere = CreateBufferResource(sizeof(Matrix4x4));
	transformationMatrixResourceSphere->Map(
		0, nullptr, reinterpret_cast<void**>(&transformationMatrixDataSphere));
	projectionMatrixSphere = MakePerspectiveFovMatrix(0.45f, float(width) / float(height), 0.1f, 100.0f);
	*transformationMatrixDataSphere = MakeIdentity4x4();
	transformMatrixSphere = { {1.0f,1.0f,1.0f},{0.0f,0.0f,0.0f},{0.0f,0.0f,0.0f} };
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
		inputElementDescs[2].SemanticName = "NORMAL";
		inputElementDescs[2].SemanticIndex = 0;
		inputElementDescs[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
		inputElementDescs[2].AlignedByteOffset = D3D12_APPEND_ALIGNED_ELEMENT;

		inputLayoutDesc.pInputElementDescs = inputElementDescs;
		inputLayoutDesc.NumElements = _countof(inputElementDescs);
	
		blendDesc.RenderTarget[0].RenderTargetWriteMask =
			D3D12_COLOR_WRITE_ENABLE_ALL;
	
		rasterizerDesc.CullMode = D3D12_CULL_MODE_NONE;
		rasterizerDesc.FillMode = D3D12_FILL_MODE_SOLID;

		vertexShaderBlob = CompileShader(L"Object3d.VS.hlsl", L"vs_6_0",
			dxcUtils, dxcCompiler, includeHandler);
		assert(vertexShaderBlob != nullptr);
	
		pixelShaderBlob = CompileShader(L"Object3d.PS.hlsl", L"ps_6_0",
			dxcUtils, dxcCompiler, includeHandler);
		assert(pixelShaderBlob != nullptr);
	
		depthStencilDesc.DepthEnable = true;
		depthStencilDesc.DepthWriteMask = D3D12_DEPTH_WRITE_MASK_ALL;
		depthStencilDesc.DepthFunc = D3D12_COMPARISON_FUNC_LESS_EQUAL;

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

		graphicsPipelineStateDesc.DepthStencilState = depthStencilDesc;
		graphicsPipelineStateDesc.DSVFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;

		hr = DX12Common::GetInstance()->GetDevice()->
			CreateGraphicsPipelineState(
			&graphicsPipelineStateDesc,
			IID_PPV_ARGS(&graphicsPipelineState));
		assert(SUCCEEDED(hr));
}

void Mesh::Update()
{
	    vertexResource = CreateBufferResource(sizeof(VertexData) * 3);
		vertexResourceSprite = CreateBufferResource(sizeof(VertexData) * 6);
		vertexResourceSphere = CreateBufferResource(sizeof(VertexData) * 6*16*16);
		MakeVertexBufferView();
		materialResource = CreateBufferResource(sizeof(Material));
		materialResourceSprite = CreateBufferResource(sizeof(Material));
		materialResourceSphere = CreateBufferResource(sizeof(Material));
		wvpResource=CreateBufferResource(sizeof(Matrix4x4));
}

ID3D12Resource* Mesh::CreateBufferResource(size_t sizeInBytes)
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

	vertexBufferViewSprite.BufferLocation = vertexResourceSprite->GetGPUVirtualAddress();
	vertexBufferViewSprite.SizeInBytes = sizeof(VertexData) * 6;
	vertexBufferViewSprite.StrideInBytes = sizeof(VertexData);

	vertexBufferViewSphere.BufferLocation = vertexResourceSphere->GetGPUVirtualAddress();
	vertexBufferViewSphere.SizeInBytes = sizeof(VertexData) * 6*16*16;
	vertexBufferViewSphere.StrideInBytes = sizeof(VertexData);
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
	Material* materialData = nullptr;
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	wvpResource->Map(0, nullptr, reinterpret_cast<void**>(&wvpData));

	materialData[0].color = color;
	*wvpData = MakeIdentity4x4();

	transformMatrix.rotate.y += 0.02f;
	Matrix4x4 worldMatrix = MakeAffineMatrix(transformMatrix.scale, transformMatrix.rotate, transformMatrix.translate);
	viewMatrix = Inverse(cameraMatrix);
	worldViewProjectionMatrix = Multiply(worldMatrix, Multiply(viewMatrix, projectionMatrix));
	*wvpData = worldViewProjectionMatrix;

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

void Mesh::InputDataSprite(
	Vector4 LeftTop,
	Vector4 RightTop,
	Vector4 RightBottom,
	Vector4 LeftBottom,
	Vector4 color,
	Vector2 coordLeftTop,
	Vector2 coordRightTop,
	Vector2 coordRightBottom,
	Vector2 coordLeftBottom)
{
	VertexData* vertexDataSprite = nullptr;
	vertexResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSprite));
	Material* materialDataSprite = nullptr;
	materialResourceSprite->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSprite));

	Matrix4x4 worldMatrix = MakeAffineMatrix(transformMatrixSprite.scale, transformMatrixSprite.rotate, transformMatrixSprite.translate);
	viewMatrixSprite = MakeIdentity4x4();
	worldViewProjectionMatrixSprite = Multiply(worldMatrix, Multiply(viewMatrixSprite, projectionMatrixSprite));
	*transformationMatrixDataSprite = worldViewProjectionMatrixSprite;
	
	materialDataSprite[0].color = color;
	materialDataSprite[0].enableLighting = false;

	vertexDataSprite[0].position = LeftTop;
	vertexDataSprite[1].position = RightTop;
	vertexDataSprite[2].position = RightBottom;
	vertexDataSprite[0].texcoord = coordLeftTop;
	vertexDataSprite[1].texcoord = coordRightTop;
	vertexDataSprite[2].texcoord = coordRightBottom;

	vertexDataSprite[3].position = LeftTop;
	vertexDataSprite[4].position = RightBottom;
	vertexDataSprite[5].position = LeftBottom;
	vertexDataSprite[3].texcoord = coordLeftTop;
	vertexDataSprite[4].texcoord = coordRightBottom;
	vertexDataSprite[5].texcoord = coordLeftBottom;
}

void Mesh::InputDataSphere(
	Vector4 LeftTop,
	Vector4 RightTop,
	Vector4 RightBottom,
	Vector4 LeftBottom,
	Vector4 color,
	Vector2 coordLeftTop,
	Vector2 coordRightTop,
	Vector2 coordRightBottom,
	Vector2 coordLeftBottom,
	uint32_t count)
{
	VertexData* vertexDataSphere = nullptr;
	transformMatrixSphere.rotate.y -= 0.0002f;
	transformMatrixSphere.rotate.z = -23.4 / 360.0f;
	vertexResourceSphere->Map(0, nullptr, reinterpret_cast<void**>(&vertexDataSphere));
	Material* materialDataSphere = nullptr;
	materialResourceSphere->Map(0, nullptr, reinterpret_cast<void**>(&materialDataSphere));
	Matrix4x4 worldMatrix = MakeAffineMatrix(transformMatrixSphere.scale, transformMatrixSphere.rotate, transformMatrixSphere.translate);
	viewMatrixSphere = Inverse(cameraMatrix);
	worldViewProjectionMatrixSphere = Multiply(worldMatrix, Multiply(viewMatrixSphere, projectionMatrixSphere));
	*transformationMatrixDataSphere = worldViewProjectionMatrixSphere;
	
	materialDataSphere[0].color = color;

	vertexDataSphere[count * 6].position = LeftTop;
	vertexDataSphere[count * 6 + 1].position = RightTop;
	vertexDataSphere[count * 6 + 2].position = RightBottom;
	vertexDataSphere[count * 6].texcoord = coordLeftTop;
	vertexDataSphere[count * 6 + 1].texcoord = coordRightTop;
	vertexDataSphere[count * 6 + 2].texcoord = coordRightBottom;
	vertexDataSphere[count * 6].normal.x = vertexDataSphere[count * 6].position.x;
	vertexDataSphere[count * 6].normal.y = vertexDataSphere[count * 6].position.y;
	vertexDataSphere[count * 6].normal.z = vertexDataSphere[count * 6].position.z;
	vertexDataSphere[count * 6 + 1].normal.x = vertexDataSphere[count * 6 + 1].position.x;
	vertexDataSphere[count * 6 + 1].normal.y = vertexDataSphere[count * 6 + 1].position.y;
	vertexDataSphere[count * 6 + 1].normal.z = vertexDataSphere[count * 6 + 1].position.z;
	vertexDataSphere[count * 6 + 2].normal.x = vertexDataSphere[count * 6 + 2].position.x;
	vertexDataSphere[count * 6 + 2].normal.y = vertexDataSphere[count * 6 + 2].position.y;
	vertexDataSphere[count * 6 + 2].normal.z = vertexDataSphere[count * 6 + 2].position.z;

	vertexDataSphere[count * 6 + 3].position = LeftTop;
	vertexDataSphere[count * 6 + 4].position = RightBottom;
	vertexDataSphere[count * 6 + 5].position = LeftBottom;
	vertexDataSphere[count * 6 + 3].texcoord = coordLeftTop;
	vertexDataSphere[count * 6 + 4].texcoord = coordRightBottom;
	vertexDataSphere[count * 6 + 5].texcoord = coordLeftBottom;
	vertexDataSphere[count * 6 + 3].normal.x = vertexDataSphere[count * 6 + 3].position.x;
	vertexDataSphere[count * 6 + 3].normal.y = vertexDataSphere[count * 6 + 3].position.y;
	vertexDataSphere[count * 6 + 3].normal.z = vertexDataSphere[count * 6 + 3].position.z;
	vertexDataSphere[count * 6 + 4].normal.x = vertexDataSphere[count * 6 + 4].position.x;
	vertexDataSphere[count * 6 + 4].normal.y = vertexDataSphere[count * 6 + 4].position.y;
	vertexDataSphere[count * 6 + 4].normal.z = vertexDataSphere[count * 6 + 4].position.z;
	vertexDataSphere[count * 6 + 5].normal.x = vertexDataSphere[count * 6 + 5].position.x;
	vertexDataSphere[count * 6 + 5].normal.y = vertexDataSphere[count * 6 + 5].position.y;
	vertexDataSphere[count * 6 + 5].normal.z = vertexDataSphere[count * 6 + 5].position.z;

}

void Mesh::DrawSprite(
	Vector4 LeftTop,
	Vector4 RightTop,
	Vector4 RightBottom,
	Vector4 LeftBottom,
	Vector4 color,
	Vector2 coordLeftTop,
	Vector2 coordRightTop,
	Vector2 coordRightBottom,
	Vector2 coordLeftBottom)
{
	InputDataSprite(
		LeftTop,
		RightTop,
		RightBottom,
		LeftBottom,
		color,
		coordLeftTop,
		coordRightTop,
		coordRightBottom,
		coordLeftBottom);
	DX12Common::GetInstance()->GetCommandList()->
		RSSetViewports(1, &viewport);

	DX12Common::GetInstance()->GetCommandList()->
		RSSetScissorRects(1, &scissorRect);

	DX12Common::GetInstance()->GetCommandList()->
		SetPipelineState(graphicsPipelineState);

	DX12Common::GetInstance()->GetCommandList()->
		SetGraphicsRootSignature(rootSignature);

	DX12Common::GetInstance()->GetCommandList()->
		IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	DX12Common::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(0,
			materialResourceSprite->GetGPUVirtualAddress());

	DX12Common::GetInstance()->GetCommandList()->
		SetGraphicsRootConstantBufferView(1,
			transformationMatrixResourceSprite->GetGPUVirtualAddress());

	D3D12_CPU_DESCRIPTOR_HANDLE rtv = DX12Common::GetInstance()->
		GetRtvHandles(DX12Common::GetInstance()->GetBackBufferIndex());
	D3D12_CPU_DESCRIPTOR_HANDLE dsv = DX12Common::GetInstance()->GetDsvHandle();

	DX12Common::GetInstance()->GetCommandList()->
		OMSetRenderTargets(
			1,
			&rtv,
			false,
			&dsv);

	DX12Common::GetInstance()->GetCommandList()->
		SetGraphicsRootDescriptorTable(2,
			DX12Common::GetInstance()->GetTextureSrvHandleGPU());

	DX12Common::GetInstance()->GetCommandList()->
		IASetVertexBuffers(0, 1, &vertexBufferViewSprite);

	DX12Common::GetInstance()->GetCommandList()->
		DrawInstanced(6, 1, 0, 0);
}

void Mesh::DrawTriangle(
	Vector4 Top,
	Vector4 Right,
	Vector4 Left,
	Vector4 color,
	Vector2 coordTop,
	Vector2 coordRight,
	Vector2 coordLeft,
	bool useWorldMap)
{
	InputDataTriangle(
		Top,
		Right,
		Left,
		color,
		coordTop,
		coordRight,
		coordLeft);
	DX12Common::GetInstance()->GetCommandList()->
		RSSetViewports(1, &viewport);

	DX12Common::GetInstance()->GetCommandList()->
		RSSetScissorRects(1, &scissorRect);

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

	D3D12_CPU_DESCRIPTOR_HANDLE rtv = DX12Common::GetInstance()->
		GetRtvHandles(DX12Common::GetInstance()->GetBackBufferIndex());
	D3D12_CPU_DESCRIPTOR_HANDLE dsv = DX12Common::GetInstance()->GetDsvHandle();

	DX12Common::GetInstance()->GetCommandList()->
		OMSetRenderTargets(
			1,
			&rtv,
			false,
			&dsv);

	DX12Common::GetInstance()->GetCommandList()->
		SetGraphicsRootDescriptorTable(
			2,
			useWorldMap ?
			DX12Common::GetInstance()->GetTextureSrvHandleGPU2() :
			DX12Common::GetInstance()->GetTextureSrvHandleGPU());

	DX12Common::GetInstance()->GetCommandList()->
		DrawInstanced(3, 1, 0, 0);
}

void Mesh::DrawSphere(const Sphere& sphere_,
	Vector4 color, bool useWorldMap)
{
	const uint32_t kSubdivision = 16;
	float pi = 3.141592f;
	const float kLonevery = 2.0f / kSubdivision * pi;
	const float kLatevery = pi / kSubdivision;
	uint32_t sphereCount = -1;

	for (uint32_t latIndex = 0; latIndex < kSubdivision; ++latIndex)
	{
		float lat = -pi / 2.0f + kLatevery * latIndex;//theta
		float latB = pi / kSubdivision;

		for (uint32_t lonIndex = 0; lonIndex < kSubdivision; ++lonIndex)
		{
			sphereCount++;
			float lon = lonIndex * kLonevery;//phi
			float lonC = 2 * pi / kSubdivision;

			Vector4 a =
			{
				sphere_.center.x + sphere_.radius * cos(lat) * cos(lon),
				sphere_.center.y + sphere_.radius * sin(lat),
				sphere_.center.z + sphere_.radius * cos(lat) * sin(lon),
				1.0f
			};
			Vector4 b =
			{
				sphere_.center.x + sphere_.radius * cos(lat + latB) * cos(lon),
				sphere_.center.y + sphere_.radius * sin(lat + latB),
				sphere_.center.z + sphere_.radius * cos(lat + latB) * sin(lon),
				1.0f
			};
			Vector4 c =
			{
				sphere_.center.x + sphere_.radius * cos(lat) * cos(lon + lonC),
				sphere_.center.y + sphere_.radius * sin(lat),
				sphere_.center.z + sphere_.radius * cos(lat) * sin(lon + lonC),
				1.0f
			};
			Vector4 d =
			{
				sphere_.center.x + sphere_.radius * cos(lat + latB) * cos(lon + lonC),
				sphere_.center.y + sphere_.radius * sin(lat + latB),
				sphere_.center.z + sphere_.radius * cos(lat + latB) * sin(lon + lonC),
				1.0f
			};
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
				b,
				d,
				c,
				a,
				color,
				texcoordB,
				texcoordD,
				texcoordC,
				texcoordA,
				sphereCount);

			DX12Common::GetInstance()->GetCommandList()->
				RSSetViewports(1, &viewport);

			DX12Common::GetInstance()->GetCommandList()->
				RSSetScissorRects(1, &scissorRect);

			DX12Common::GetInstance()->GetCommandList()->
				SetPipelineState(graphicsPipelineState);

			DX12Common::GetInstance()->GetCommandList()->
				SetGraphicsRootSignature(rootSignature);

			DX12Common::GetInstance()->GetCommandList()->
				IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

			DX12Common::GetInstance()->GetCommandList()->
				SetGraphicsRootConstantBufferView(0,
					materialResourceSphere->GetGPUVirtualAddress());

			DX12Common::GetInstance()->GetCommandList()->
				SetGraphicsRootConstantBufferView(1,
					transformationMatrixResourceSphere->GetGPUVirtualAddress());

			D3D12_CPU_DESCRIPTOR_HANDLE rtv = DX12Common::GetInstance()->
				GetRtvHandles(DX12Common::GetInstance()->GetBackBufferIndex());
			D3D12_CPU_DESCRIPTOR_HANDLE dsv = DX12Common::GetInstance()->GetDsvHandle();

			DX12Common::GetInstance()->GetCommandList()->
				OMSetRenderTargets(
					1,
					&rtv,
					false,
					&dsv);

			DX12Common::GetInstance()->GetCommandList()->
				SetGraphicsRootDescriptorTable(
					2,
					useWorldMap ?
					DX12Common::GetInstance()->GetTextureSrvHandleGPU2() :
					DX12Common::GetInstance()->GetTextureSrvHandleGPU());

			DX12Common::GetInstance()->GetCommandList()->
				IASetVertexBuffers(0, 1, &vertexBufferViewSphere);

			DX12Common::GetInstance()->GetCommandList()->
				DrawInstanced(6 * kSubdivision * kSubdivision, 1, 0, 0);

		}
	}
}

void Mesh::MeshRelease()
{
	vertexResource->Release();
	vertexResourceSprite->Release();
	vertexResourceSphere->Release();
	materialResource->Release();
	materialResourceSprite->Release();
	materialResourceSphere->Release();
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



