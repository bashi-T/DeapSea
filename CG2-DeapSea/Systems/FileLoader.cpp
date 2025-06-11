#include "FileLoader.h"
//namespace MyEngine {
void /*FileLoader::*/LoadVectorFile(Vector3 vector, std::stringstream filename)
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
void /*FileLoader::*/LoadVectorFile(Vector4 vector, std::stringstream filename)
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

			// a座標
			getline(line_stream, word, ',');
			float a = (float)std::atof(word.c_str());

			vector = {x, y, z, a};
		}
	}
}
Vector3 /*FileLoader::*/ LoadVector3File(std::string filePath)
{
	std::ifstream file;
	file.open(filePath);
	assert(file.is_open());

	//enemyPopCommands[fileNum] << file.rdbuf();
	//file.close();

	float x = 0.0f, y = 0.0f, z = 0.0f;
	std::string line;
	while (getline(file, line))
	{
		std::istringstream line_stream(line);
		std::string word;
		getline(line_stream, word, ',');
		if (word.find("VECTOR") == 0)
		{
			// x座標
			getline(line_stream, word, ',');
			x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			z = (float)std::atof(word.c_str());
		}
	}
	return {x, y, z};
}
Vector4/* FileLoader::*/LoadVector4File(std::string filename)
{
	std::ifstream file;
	file.open(filename);
	assert(file.is_open());

	float x = 0.0f, y = 0.0f, z = 0.0f, a = 0.0f;
	std::string line;
	while (getline(file, line))
	{
		std::istringstream line_stream(line);
		std::string word;
		getline(line_stream, word, ',');
		if (word.find("VECTOR") == 0)
		{
			// x座標
			getline(line_stream, word, ',');
			x = (float)std::atof(word.c_str());

			// y座標
			getline(line_stream, word, ',');
			y = (float)std::atof(word.c_str());

			// z座標
			getline(line_stream, word, ',');
			z = (float)std::atof(word.c_str());

			// a座標
			getline(line_stream, word, ',');
			a = (float)std::atof(word.c_str());
		}
	}
	return {x, y, z, a};
}
//} // namespace MyEngine
