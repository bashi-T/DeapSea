#include "Debug.h"

void Debug::log(const std::string& message)
{
	OutputDebugStringA(message.c_str());
}
