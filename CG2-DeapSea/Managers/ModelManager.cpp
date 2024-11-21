#include "ModelManager.h"

namespace MyEngine
{
	ModelManager* ModelManager::instance = nullptr;

	ModelManager* ModelManager::GetInstance()
	{
		if (instance == NULL)
		{
			instance = new ModelManager;
		}
		return instance;
	}

	void ModelManager::Finalize()
	{
		modelCommon_->Finalize();
		delete instance;
		instance = NULL;
	}

	void ModelManager::Initialize(DX12Common* dxCommon)
	{
		modelCommon_ = ModelCommon::GetInstance();
		modelCommon_->Initialize(dxCommon);
		sameModelNum = 0;
	}

	void ModelManager::LoadModel(std::string& filePath, const std::string& TextureFilePath, bool isLighting)
	{
		if (models.contains(filePath) && TextureManager::GetInstance()->GetTextureData().contains(TextureFilePath))
		{
			return;
		}
		else if (models.contains(filePath))
		{
			std::string num = std::to_string(sameModelNum);
			std::string modelFilePath = num + filePath;
			modelFilePaths.push_back(modelFilePath);
			//モデル生成とファイル読み込み、初期化
			std::unique_ptr<Model> model = std::make_unique<Model>();
			model->ModelInitialize(modelCommon_, filePath, TextureFilePath, isLighting);
			//モデルをmapコンテナに格納
			models.insert(std::make_pair(modelFilePath, std::move(model)));
			filePath = modelFilePath;
			sameModelNum++;
		}
		else
		{
			//モデル生成とファイル読み込み、初期化
			std::unique_ptr<Model> model = std::make_unique<Model>();
			model->ModelInitialize(modelCommon_, filePath, TextureFilePath, isLighting);
			//モデルをmapコンテナに格納
			models.insert(std::make_pair(filePath, std::move(model)));
		}
	}

	void ModelManager::LoadAnimationModel(std::string& filePath, const std::string& TextureFilePath, bool isLighting)//処理に問題の可能性あり
	{
		if (models.contains(filePath) && TextureManager::GetInstance()->GetTextureData().contains(TextureFilePath))
		{
			return;
		}
		else if (models.contains(filePath))
		{
			std::string num = std::to_string(sameModelNum);
			std::string modelFilePath = num + filePath;
			modelFilePaths.push_back(modelFilePath);
			//モデル生成とファイル読み込み、初期化
			std::unique_ptr<Model> model = std::make_unique<Model>();
			model->AnimationInitialize(modelCommon_, filePath, TextureFilePath, isLighting);
			//モデルをmapコンテナに格納
			models.insert(std::make_pair(modelFilePath, std::move(model)));
			filePath = modelFilePath;
			sameModelNum++;
		}
		else
		{
			//モデル生成とファイル読み込み、初期化
			std::unique_ptr<Model> model = std::make_unique<Model>();
			model->AnimationInitialize(modelCommon_, filePath, TextureFilePath, isLighting);
			//モデルをmapコンテナに格納
			models.insert(std::make_pair(filePath, std::move(model)));
		}
	}

	void ModelManager::LoadSkeltonAnimation(std::string& filePath, const std::string& TextureFilePath, SRVManager* srvManager, bool isLighting)
	{
		if (models.contains(filePath) && TextureManager::GetInstance()->GetTextureData().contains(TextureFilePath))
		{
			return;
		}
		else if (models.contains(filePath))
		{
			std::string num = std::to_string(sameModelNum);
			std::string modelFilePath = num + filePath;
			modelFilePaths.push_back(modelFilePath);
			//モデル生成とファイル読み込み、初期化
			std::unique_ptr<Model> model = std::make_unique<Model>();
			model->SkeltonInitialize(modelCommon_, filePath, TextureFilePath, srvManager, isLighting);
			//モデルをmapコンテナに格納
			models.insert(std::make_pair(modelFilePath, std::move(model)));
			filePath = modelFilePath;
			sameModelNum++;
		}
		else
		{
			//モデル生成とファイル読み込み、初期化
			std::unique_ptr<Model> model = std::make_unique<Model>();
			model->SkeltonInitialize(modelCommon_, filePath, TextureFilePath, srvManager, isLighting);
			//モデルをmapコンテナに格納
			models.insert(std::make_pair(filePath, std::move(model)));
		}
	}

	void ModelManager::EraseModel(const std::string& filePath, const std::string& textureFilePath)//モデル使いまわし用の解放処理
	{
		models.erase(filePath);
		TextureManager::GetInstance()->EraseTexture(textureFilePath);
	}

	Model* ModelManager::FindModel(const std::string& filePath)
	{
		if (models.contains(filePath))
		{
			return models.at(filePath).get();
		}
		return nullptr;
	}
}