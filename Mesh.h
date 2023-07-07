#pragma once
#include<cassert>
#include"Debug.h"
#include<dxcapi.h>
#include<d3d12.h>
#include<dxgi1_6.h>

#pragma comment(lib,"dxcompiler.lib")
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class Mesh
{
public:
	void Initialize(int32_t width, int32_t height);
	static void ResetDXC();

	static IDxcBlob* CompileShader(
		const std::wstring& filePath,
		const wchar_t* profile,
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);
	static void MakePSO(ID3D12Device* device);
	static void MakeVertexResource(ID3D12Device* device);
	static void MakeVertexBufferView();
	static void InputData(struct Vector4* vertexData);
	static void Draw(ID3D12GraphicsCommandList* commandList);
	static void MeshRelease();

	static ID3D12Resource* GetVertexResource() { return vertexResource; }
	static struct Vector4* GetVertexData() { return vertexData; }
private:
	static inline Debug* debug_;
	static inline HRESULT hr = NULL;

	static inline IDxcUtils* dxcUtils = nullptr;
	static inline IDxcCompiler3* dxcCompiler = nullptr;
	static inline IDxcIncludeHandler* includeHandler = nullptr;
	static inline ID3D12RootSignature* rootSignature = nullptr;
	static inline ID3D12PipelineState* graphicsPipelineState=NULL;
	static inline ID3DBlob* signatureBlob = nullptr;
	static inline ID3DBlob* errorBlob = nullptr;
	static inline IDxcBlob* pixelShaderBlob = nullptr;
	static inline IDxcBlob* vertexShaderBlob = nullptr;
	static inline D3D12_VERTEX_BUFFER_VIEW vertexBufferView{};
	static inline ID3D12Resource* vertexResource = nullptr;
	static inline D3D12_VIEWPORT viewport{};
	static inline D3D12_RECT scissorRect{};
	static inline D3D12_HEAP_PROPERTIES uploadHeapProperties{};
	static inline D3D12_RESOURCE_DESC vertexResourceDesc{};
	static inline D3D12_ROOT_SIGNATURE_DESC descriptionRootSignature{};
	static inline D3D12_INPUT_ELEMENT_DESC inputElementDescs[1] = {};
	static inline D3D12_INPUT_LAYOUT_DESC inputLayoutDesc{};
	static inline D3D12_BLEND_DESC blendDesc{};
	static inline D3D12_RASTERIZER_DESC rasterizerDesc{};
	static inline D3D12_GRAPHICS_PIPELINE_STATE_DESC graphicsPipelineStateDesc{};
	static inline struct Vector4* vertexData=nullptr;
};
struct Vector4 final {
	float x;
	float y;
	float z;
	float w;
};
