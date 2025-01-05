#include "ModelManager.h"

namespace MyEngine
{

	std::shared_ptr<ModelManager> ModelManager::GetInstance()
	{
		auto ret_ptr = instance.lock();
		if (!ret_ptr)
		{
			ret_ptr = std::shared_ptr<ModelManager>(new ModelManager{});
			instance = std::weak_ptr<ModelManager>(ret_ptr);
			return ret_ptr;
		}

		return instance.lock();
	}

	void ModelManager::Finalize()
	{
		modelCommon_->Finalize();
		instance.reset();
	}

	void ModelManager::Initialize()
	{
		modelCommon_ = ModelCommon::GetInstance();
		dx12Common_ = DX12Common::GetInstance();
		textureManager_ = TextureManager::GetInstance();

		modelCommon_->Initialize();
		sameModelNum = 0;
	}

	void ModelManager::LoadModel(std::string& filePath, const std::string& TextureFilePath, bool isLighting)
	{
		if (models.contains(filePath) && textureManager_->GetTextureData().contains(TextureFilePath))
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
			model->ModelInitialize(filePath, TextureFilePath, isLighting);
			//モデルをmapコンテナに格納
			models.insert(std::make_pair(modelFilePath, std::move(model)));
			filePath = modelFilePath;
			sameModelNum++;
		}
		else
		{
			//モデル生成とファイル読み込み、初期化
			std::unique_ptr<Model> model = std::make_unique<Model>();
			model->ModelInitialize(filePath, TextureFilePath, isLighting);
			//モデルをmapコンテナに格納
			models.insert(std::make_pair(filePath, std::move(model)));
		}
	}

	void ModelManager::LoadAnimationModel(std::string& filePath, const std::string& TextureFilePath, bool isLighting)//処理に問題の可能性あり
	{
		if (models.contains(filePath) && textureManager_->GetTextureData().contains(TextureFilePath))
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
			model->AnimationInitialize(filePath, TextureFilePath, isLighting);
			//モデルをmapコンテナに格納
			models.insert(std::make_pair(modelFilePath, std::move(model)));
			filePath = modelFilePath;
			sameModelNum++;
		}
		else
		{
			//モデル生成とファイル読み込み、初期化
			std::unique_ptr<Model> model = std::make_unique<Model>();
			model->AnimationInitialize(filePath, TextureFilePath, isLighting);
			//モデルをmapコンテナに格納
			models.insert(std::make_pair(filePath, std::move(model)));
		}
	}

	void ModelManager::LoadSkeltonAnimation(std::string& filePath, const std::string& TextureFilePath,bool isLighting)
	{
		if (models.contains(filePath) && textureManager_->GetTextureData().contains(TextureFilePath))
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
			model->SkeltonInitialize(filePath, TextureFilePath, isLighting);
			//モデルをmapコンテナに格納
			models.insert(std::make_pair(modelFilePath, std::move(model)));
			filePath = modelFilePath;
			sameModelNum++;
		}
		else
		{
			//モデル生成とファイル読み込み、初期化
			std::unique_ptr<Model> model = std::make_unique<Model>();
			model->SkeltonInitialize(filePath, TextureFilePath, isLighting);
			//モデルをmapコンテナに格納
			models.insert(std::make_pair(filePath, std::move(model)));
		}
	}

	void ModelManager::EraseModel(const std::string& filePath, const std::string& textureFilePath)//モデル使いまわし用の解放処理
	{
		models.erase(filePath);
		textureManager_->EraseTexture(textureFilePath);
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