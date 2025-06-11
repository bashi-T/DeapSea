#pragma once
#include"Math/CGVector.h"
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>

//namespace MyEngine
//{
//	class FileLoader
//	{
//	public:
	    void LoadVectorFile(Vector3 vector, std::stringstream filename);
	    void LoadVectorFile(Vector4 vector, std::stringstream filename);

		Vector3 LoadVector3File(std::string filename);
	    Vector4 LoadVector4File(std::string filename);
//    };    
//} // namespace MyEngine