#pragma once
#include<d3d12.h>
#include<dxgi1_6.h>
#include<cassert>
#include"Debug.h"


#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxgi.lib")

class DX12
{
public:
	DX12();
	~DX12();
	void MakeDXGIFactory();
	void ChoseUseAdapter();
	void MakeD3D12Device();

private:
	Debug* debug_ = nullptr;
	IDXGIFactory7 * dxgiFactory = nullptr;
	HRESULT hr = NULL;
	IDXGIAdapter4* useAdapter = nullptr;
	ID3D12Device* device = nullptr;

};

