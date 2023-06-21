#pragma once
#include<string>
#include<windows.h>
#include<format>
#include<d3d12.h>

#pragma comment(lib,"d3d12.lib")

class Debug
{
public:
	void Log(const std::string& message);
	std::string ConvertString(const std::wstring& str);
	void DebugLayer();
	void InfoQueue(ID3D12Device* device);
private:

};

