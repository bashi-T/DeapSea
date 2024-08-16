#pragma once
#include"Objects/Object3d.h"
#include"Commons/Object3dCommon.h"
#include<json.hpp>
#include<fstream>
#include<vector>
#include<map>
#include "Math/CGVector.h"

class BlenderModel
{
public:
	void Initialize(std::string objectName);
	void Update();
	void Draw();
	void Finalize();
	void LoadLevelData();
	Object3d* GetObject3d() { return  object3d; }
private:
	const std::string fullpath;

	struct LevelData
	{
		struct ObjectData
		{
			std::string fileName;
			Vector3 transration;
			Vector3 rotation;
			Vector3 scaling;
		};
		//オブジェクトのコンテナ
		std::vector<ObjectData>objects;
	};
	LevelData* levelData = new LevelData();
	std::map<std::string, Model*>models;

	//jsonから解凍したデータ
	nlohmann::json desrialized;
	std::vector<Object3d*> objects;
	Object3d* object3d = new Object3d;
};

