#include "BlenderModel.h"

namespace MyEngine
{
	void BlenderModel::Initialize(std::string objectName)
	{
		const std::string fullPath = std::string("Resource/levels/") + objectName;
		std::ifstream file;
		file.open(fullPath);
		if (file.fail())
		{
			assert(0);
		}

		// 正しいレベルデータファイルかチェック
		file >> desrialized;
		assert(desrialized.is_object());
		assert(desrialized.contains("name"));
		assert(desrialized["name"].is_string());
		// ”name”を文字列として取得
		std::string name = desrialized["name"].get<std::string>();
		assert(name.compare("scene") == 0);

		LoadLevelData();
	
		int i = 0;
		for (auto& objectData : levelData->objects)
		{
			Model* model = nullptr;
			decltype(models)::iterator it = models.find(objectData.fileName);
			if (it != models.end())
			{
				model = it->second;
			}

			Object3d* newObject = new Object3d;
			std::string objectFilePath = std::string("levels/") + objectData.fileName + ".obj";
			newObject->Initialize();
			ModelManager::GetInstance()->LoadModel(objectFilePath, "Resource/uvChecker.png",true);
			newObject->SetModel(objectFilePath);

			newObject->SetTranslate(objectData.transration);
			newObject->SetRotate(objectData.rotation);
			newObject->SetScale(objectData.scaling);
			objects.push_back(newObject);
			i++;
		}
	}

	void BlenderModel::Update()
	{
		for (Object3d* object : objects)
		{
			object->Update(Camera::GetInstance());
		}
	}

	void BlenderModel::Draw()
	{
		for (Object3d* object : objects)
		{
			object->Draw(ModelManager::GetInstance()->GetModelCommon());
		}
	}

	void BlenderModel::Finalize() {}

	void BlenderModel::LoadLevelData()
	{
		for (nlohmann::json& object : desrialized["objects"])
		{
			assert(object.contains("type"));
			std::string type = object["type"].get<std::string>();
			if (type.compare("MESH") == 0)
			{
				levelData->objects.emplace_back(LevelData::ObjectData{});
				LevelData::ObjectData& objectData = levelData->objects.back();
				if (object.contains("file_name"))
				{
					objectData.fileName = object["file_name"];
				}
				nlohmann::json& transform = object["transform"];
		
				objectData.transration.x = (float)transform["translation"][0];
				objectData.transration.y = (float)transform["translation"][2];
				objectData.transration.z = (float)transform["translation"][1];

				objectData.rotation.x = -(float)transform["rotation"][0] * (theta_ / 180);
				objectData.rotation.y = -(float)transform["rotation"][2] * (theta_ / 180);
				objectData.rotation.z = -(float)transform["rotation"][1] * (theta_ / 180);

				objectData.scaling.x = (float)transform["scaling"][0];
				objectData.scaling.y = (float)transform["scaling"][2];
				objectData.scaling.z = (float)transform["scaling"][1];
			}
			if (object.contains("children")) // 再帰処理する
			{
				LoadLevelData();
			}
		}
	}
} // namespace MyEngine
