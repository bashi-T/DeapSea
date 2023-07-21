#pragma once
#include"DX12Common.h"
#include<dxcapi.h>

#pragma comment(lib,"dxcompiler.lib")

class Mesh
{
public:
	void Initialize(int32_t width, int32_t height);
	void ResetDXC();

	IDxcBlob* CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile,
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);

	void CreatePSO();
	void MakeVertexResource(ID3D12Device* device, int NumTriangle);
	void MakeVertexBufferView(int NumTriangle);
	void InputData(struct Vector4* vertexData);
	void InputDataChain(struct Vector4* vertexData, int32_t i);
	void Draw(ID3D12GraphicsCommandList* commandList, int NumTriangle);
	void DrawTriangle(
		int32_t numTriangle,
		int i,
		ID3D12GraphicsCommandList* commandList,
		D3D12_CPU_DESCRIPTOR_HANDLE rtvHandles[],
		UINT backBufferIndex);
	void MeshRelease();

	ID3D12Resource* GetVertexResource() { return vertexResource; }
	Vector4* GetVertexData() { return vertexData; }

private:
	Debug* debug_;
	WinApp* sWinApp;

	static inline HRESULT hr = NULL;
	
	static inline IDxcUtils* dxcUtils = nullptr;
	static inline IDxcCompiler3* dxcCompiler = nullptr;
	static inline IDxcIncludeHandler* includeHandler = nullptr;
	ID3D12RootSignature* rootSignature = nullptr;
	ID3D12PipelineState* graphicsPipelineState=NULL;
	ID3DBlob* signatureBlob = nullptr;
	ID3DBlob* errorBlob = nullptr;
	IDxcBlob* pixelShaderBlob = nullptr;
	IDxcBlob* vertexShaderBlob = nullptr;
	D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	ID3D12Resource* vertexResource = nullptr;
	static inline D3D12_VIEWPORT viewport{};
	static inline D3D12_RECT scissorRect{};
	D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	D3D12_RESOURCE_DESC vertexResourceDesc{};
	D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature_{};
	D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
	D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	D3D12_BLEND_DESC blendDesc{};
	D3D12_RASTERIZER_DESC rasterizerDesc{};
	D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	struct Vector4* vertexData=nullptr;
};
struct Vector4 final {
	float x;
	float y;
	float z;
	float w;
};
