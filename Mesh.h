#pragma once
#include<cassert>
#include"Debug.h"
#include<dxcapi.h>

#pragma comment(lib,"dxcompiler.lib")

class Mesh
{
public:
	void MakePSO(ID3D12Device* device);
	void ResetDXC();
	IDxcBlob* CompileShader
	(
		const std::wstring& filePath,
		const wchar_t* profile,
		IDxcUtils* dxcUtils,
		IDxcCompiler3* dxcCompiler,
		IDxcIncludeHandler* includeHandler
	);

private:
	Debug* debug_ = nullptr;
	HRESULT hr = NULL;

	IDxcUtils* dxcUtils = nullptr;
	IDxcCompiler3* dxcCompiler = nullptr;
	IDxcIncludeHandler* includeHandler = nullptr;


};

