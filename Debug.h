#pragma once
#include<string>
#include<windows.h>
#include<format>

class Debug
{
public:
	void Log(const std::string& message);
	std::string ConvertString(const std::wstring& str);
};

