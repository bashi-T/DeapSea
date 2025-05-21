#include "FileLoader.h"
namespace MyEngine {
void FileLoader::LoadVectorFile(Vector3 vector, std::stringstream filename)
{
		std::string line;
		while (getline(filename, line))
		{
			std::istringstream line_stream(line);
			std::string word;
			getline(line_stream, word, ',');
		    if (word.find("VECTOR") == 0)
			{
			    // x座標
			    getline(line_stream, word, ',');
			    float x = (float)std::atof(word.c_str());

			    // y座標
			    getline(line_stream, word, ',');
			    float y = (float)std::atof(word.c_str());

			    // z座標
			    getline(line_stream, word, ',');
			    float z = (float)std::atof(word.c_str());
			    vector = {x, y, z};
		    }
	    }
}
} // namespace MyEngine
