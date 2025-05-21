#pragma once
#include"Math/CGVector.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

namespace MyEngine
{
	class FileLoader
	{
	public:
		void LoadVectorFile(Vector3 vector, std::stringstream filename);
	};
} // namespace MyEngine