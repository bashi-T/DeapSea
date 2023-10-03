#pragma once
#include<string>
#include<windows.h>
#include<format>
#include<d3d12.h>
#include<dxgidebug.h>
#include<dxgi1_3.h>
#pragma comment(lib,"d3d12.lib")
#pragma comment(lib,"dxguid.lib")

class Debug
{
public:
	void Log(const std::string& message);
	std::string ConvertString(const std::wstring& str);
	std::wstring ConvertString(const std::string& str);
	void DebugLayer();
	void InfoQueue(ID3D12Device* device);
	void ReportLiveObject();
	ID3D12Debug1* GetDebugController() { return debugController; }
		
	
private:
	IDXGIDebug1* debug;
	ID3D12Debug1* debugController = nullptr;
};

