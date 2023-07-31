#include "Mesh.h"

Mesh::~Mesh()
{
	delete dxcUtils;
	delete dxcCompiler;
	delete includeHandler;
	delete rootSignature;
	delete graphicsPipelineState;
}

void Mesh::Initialize(int32_t width, int32_t height, int NumTriangle) {
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
	MakeVertexResource(NumTriangle);
	MakeVertexBufferView(NumTriangle);
	MakeMaterialResource(NumTriangle);
	//InputDataTriangle(NumTriangle);
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
	IDxcUtils* dxcUtils,
    IDxcCompiler3* dxcCompiler,
	IDxcIncludeHandler* includeHandler
	)
{
	debug_->Log(debug_->ConvertString(std::format(
		L"Begin CompileShader,path{},\n", filePath, profile)));
	IDxcBlobEncoding* shaderSource = nullptr;
	hr = dxcUtils->LoadFile(filePath.c_str(), nullptr, &shaderSource);
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
	hr = dxcCompiler->Compile(
		&shaderSourceBuffer,
		arguments,
		_countof(arguments),
		includeHandler,
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
	
		rootParameters[0].ParameterType = D3D12_ROOT_PARAMETER_TYPE_CBV;
	    rootParameters[0].ShaderVisibility = D3D12_SHADER_VISIBILITY_PIXEL;
	    rootParameters[0].Descriptor.ShaderRegister = 0;
	    descriptionRootSignature_.pParameters = rootParameters;
	    descriptionRootSignature_.NumParameters = _countof(rootParameters);



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
	
		hr = DX12Common::GetInstance()->GetDevice()->CreateGraphicsPipelineState(
			&graphicsPipelineStateDesc,
			IID_PPV_ARGS(&graphicsPipelineState));
		assert(SUCCEEDED(hr));
}

void Mesh::Update()
{
	    //imgui_->Update();
	    //ImGui::Begin("color");
	    //ImGui::ColorEdit4("color", (float*)&color);
	    //ImGui::End();
	    //ImGui::Render();
}

void Mesh::MakeVertexResource(int NumTriangle)
{
	    D3D12_HEAP_PROPERTIES uploadHeapProperties{};

	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	    D3D12_RESOURCE_DESC vertexResourceDesc{};

	vertexResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;

	vertexResourceDesc.Width = sizeof(Vector4) * NumTriangle*3;

	vertexResourceDesc.Height = 1;
	vertexResourceDesc.DepthOrArraySize = 1;
	vertexResourceDesc.MipLevels = 1;
	vertexResourceDesc.SampleDesc.Count = 1;

	vertexResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	hr = DX12Common::GetInstance()->GetDevice()->CreateCommittedResource(
		&uploadHeapProperties, D3D12_HEAP_FLAG_NONE,
		&vertexResourceDesc, D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr, IID_PPV_ARGS(&vertexResource));
	assert(SUCCEEDED(hr));
}

void Mesh::MakeMaterialResource(int NumTriangle)
{
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};

	uploadHeapProperties.Type = D3D12_HEAP_TYPE_UPLOAD;
	D3D12_RESOURCE_DESC materialResourceDesc{};

	materialResourceDesc.Dimension = D3D12_RESOURCE_DIMENSION_BUFFER;
	
	materialResourceDesc.Width = sizeof(Vector4) * NumTriangle * 3;
	
	materialResourceDesc.Height = 1;
	materialResourceDesc.DepthOrArraySize = 1;
	materialResourceDesc.MipLevels = 1;
	materialResourceDesc.SampleDesc.Count = 1;
	
	materialResourceDesc.Layout = D3D12_TEXTURE_LAYOUT_ROW_MAJOR;

	hr = DX12Common::GetInstance()->GetDevice()->CreateCommittedResource(
	    &uploadHeapProperties, D3D12_HEAP_FLAG_NONE, &materialResourceDesc,
	    D3D12_RESOURCE_STATE_GENERIC_READ, nullptr, IID_PPV_ARGS(&materialResource));
	assert(SUCCEEDED(hr));
}

void Mesh::MakeVertexBufferView(int NumTriangle)
{
	vertexBufferView.BufferLocation = vertexResource->GetGPUVirtualAddress();
	vertexBufferView.SizeInBytes = sizeof(Vector4) * NumTriangle*3;
	vertexBufferView.StrideInBytes = sizeof(Vector4);
}

void Mesh::InputDataTriangle(Vector4 Top,Vector4 Right,Vector4 Left,Vector4 color)
{
	Vector4* vertexData = nullptr;
	vertexResource->Map(0, nullptr, reinterpret_cast<void**>(&vertexData));
	Vector4* materialData = nullptr;
	materialResource->Map(0, nullptr, reinterpret_cast<void**>(&materialData));
	
	*materialData = color;

	vertexData[0] = Left; // { -1.0f,-1.0f,0.0f,1.0f };//左下
	vertexData[1] = Top;  // { -0.75f,-0.6f,0.0f,1.0f };//上
	vertexData[2] = Right;// { -0.5f,-1.0f,0.0f,1.0f };//右下

	//vertexData[3] = { -0.5f,-1.0f,0.0f,1.0f };//左下
	//vertexData[4] = { -0.25f,-0.7f,0.0f,1.0f };//上
	//vertexData[5] = { 0.0f,-1.0f,0.0f,1.0f };//右下

	//vertexData[6] = { 0.0f,-1.0f,0.0f,1.0f };//左下
	//vertexData[7] = { 0.25f,-0.8f,0.0f,1.0f };//上
	//vertexData[8] = { 0.5f,-1.0f,0.0f,1.0f };//右下

	//vertexData[9] = { 0.5f,-1.0f,0.0f,1.0f };//左下
	//vertexData[10] = { 0.75f,-0.9f,0.0f,1.0f };//上
	//vertexData[11] = { 1.0f,-1.0f,0.0f,1.0f };//右下

	//vertexData[12] = { -0.6f,-0.5f,0.0f,1.0f };//左下
	//vertexData[13] = { -0.45f,0.0f,0.0f,1.0f };//上
	//vertexData[14] = { -0.3f,-0.5f,0.0f,1.0f };//右下

	//vertexData[15] = { 0.05f,-0.5f,0.0f,1.0f };//左下
	//vertexData[16] = { 0.25f,0.0f,0.0f,1.0f };//上
	//vertexData[17] = { 0.55f,-0.5f,0.0f,1.0f };//右下

	//vertexData[18] = { -0.9f,0.0f,0.0f,1.0f };//左下
	//vertexData[19] = { -0.45f,0.5f,0.0f,1.0f };//上
	//vertexData[20] = { 0.0f,0.0f,0.0f,1.0f };//右下

	//vertexData[21] = { -0.0f,0.0f,0.0f,1.0f };//左下
	//vertexData[22] = { 0.25f,0.5f,0.0f,1.0f };//上
	//vertexData[23] = { 0.5f,0.0f,0.0f,1.0f };//右下

	//vertexData[24] = { -0.25f,0.5f,0.0f,1.0f };//左下
	//vertexData[25] = { -0.25f,1.0f,0.0f,1.0f };//上
	//vertexData[26] = { 0.0f,0.5f,0.0f,1.0f };//右下

	//vertexData[27] = { 0.0f,0.5f,0.0f,1.0f };//左下
	//vertexData[28] = { 0.25f,1.0f,0.0f,1.0f };//上
	//vertexData[29] = { 0.4f,0.5f,0.0f,1.0f };//右下
}

void Mesh::DrawTriangle(int NumTriangle) {
	float clearColor[] =
	{
		0.1f, 0.25f,0.5f,1.0f
	};
	DX12Common::GetInstance()->GetCommandList()->ClearRenderTargetView(
	    DX12Common::GetInstance()->
		GetRtvHandles(DX12Common::GetInstance()->GetBackBufferIndex()),
	    clearColor, 0, nullptr);
}

void Mesh::Draw(Vector4 Top, Vector4 Right, Vector4 Left, Vector4 color) {
	DX12Common::GetInstance()->GetCommandList()->
		RSSetViewports(1, &viewport);

	DX12Common::GetInstance()->GetCommandList()->
		RSSetScissorRects(1, &scissorRect);

		InputDataTriangle( Top, Right, Left,color);

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
		DrawInstanced(3, 1, 0, 0);
}
    

void Mesh::MeshRelease()
{
	vertexResource->Release();
	materialResource->Release();
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



