#include "ModelManager.h"

namespace MyEngine
{

	ModelManager* ModelManager::GetInstance()
	{
		if (instance == NULL)
		{
			instance = std::make_unique<ModelManager>();
		}
		return instance.get();
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

	void ModelManager::LoadModel(std::string& modelFilePath, const std::string& textureFilePath, bool isLighting)
	{
	    if (fileNames.contains(textureFilePath))
		{
			//読み込まれているテクスチャに対応したモデルをあてがう
		    modelFilePath = fileNames[textureFilePath];
	    }
		else if (models.contains(modelFilePath))//同じモデルを別テクスチャで使うとき
		{
		    sameModelNum++;
			std::string num = std::to_string(sameModelNum);
			std::string newModelFilePath = num + modelFilePath;
		    modelFilePaths.push_back(newModelFilePath);
			//モデル生成とファイル読み込み、初期化
			std::unique_ptr<Model> model = std::make_unique<Model>();
		    model->ModelInitialize(modelFilePath, textureFilePath, isLighting);
			//モデル名を変更
		    modelFilePath = newModelFilePath;
			//モデルとテクスチャをmapコンテナに格納
		    models[modelFilePath] = {std::move(model), textureFilePath};
		    fileNames[textureFilePath] = modelFilePath;
		}
		else
		{
			//モデル生成とファイル読み込み、初期化
			std::unique_ptr<Model> model = std::make_unique<Model>();
			model->ModelInitialize(modelFilePath, textureFilePath, isLighting);
		    // モデルとテクスチャをmapコンテナに格納
		    models[modelFilePath] = {std::move(model), textureFilePath};
		    fileNames[textureFilePath] = modelFilePath;
		}
	}

	void ModelManager::LoadAnimationModel(std::string& modelFilePath, const std::string& TextureFilePath, bool isLighting)//処理に問題の可能性あり
	{
	    if (fileNames.contains(TextureFilePath))
		{
		    modelFilePath = fileNames[TextureFilePath];
	    }
		else if (models.contains(modelFilePath)) // 同じモデルを別テクスチャで使うとき
	    {
		    sameModelNum++;
		    std::string num = std::to_string(sameModelNum);
		    std::string newModelFilePath = num + modelFilePath;
		    modelFilePaths.push_back(newModelFilePath);
		    // モデル生成とファイル読み込み、初期化
		    std::unique_ptr<Model> model = std::make_unique<Model>();
		    model->AnimationInitialize(modelFilePath, TextureFilePath, isLighting);
		    // モデル名を変更
		    modelFilePath = newModelFilePath;
		    // モデルとテクスチャをmapコンテナに格納
		    models[modelFilePath] = {std::move(model), TextureFilePath};
		    fileNames[TextureFilePath] = modelFilePath;
	    } else {
		    // モデル生成とファイル読み込み、初期化
		    std::unique_ptr<Model> model = std::make_unique<Model>();
		    model->AnimationInitialize(modelFilePath, TextureFilePath, isLighting);
		    // モデルとテクスチャをmapコンテナに格納
		    models[modelFilePath] = {std::move(model), TextureFilePath};
		    fileNames[TextureFilePath] = modelFilePath;
	    }
    }

	void ModelManager::LoadSkeltonAnimation(std::string& modelFilePath, const std::string& TextureFilePath,bool isLighting)
	{
	    if (fileNames.contains(TextureFilePath))
		{
		    modelFilePath = fileNames[TextureFilePath];
	    }
		else if (models.contains(modelFilePath)) // 同じモデルを別テクスチャで使うとき
	    {
		    sameModelNum++;
		    std::string num = std::to_string(sameModelNum);
		    std::string newModelFilePath = num + modelFilePath;
		    modelFilePaths.push_back(newModelFilePath);
		    // モデル生成とファイル読み込み、初期化
		    std::unique_ptr<Model> model = std::make_unique<Model>();
		    model->SkeltonInitialize(modelFilePath, TextureFilePath, isLighting);
		    // モデル名を変更
		    modelFilePath = newModelFilePath;
		    // モデルとテクスチャをmapコンテナに格納
		    models[modelFilePath] = {std::move(model), TextureFilePath};
		    fileNames[TextureFilePath] = modelFilePath;
	    } else {
		    // モデル生成とファイル読み込み、初期化
		    std::unique_ptr<Model> model = std::make_unique<Model>();
		    model->SkeltonInitialize(modelFilePath, TextureFilePath, isLighting);
		    // モデルとテクスチャをmapコンテナに格納
		    models[modelFilePath] = {std::move(model), TextureFilePath};
		    fileNames[TextureFilePath] = modelFilePath;
	    }
    }

	void ModelManager::EraseModel(const std::string& modelFilePath, const std::string& textureFilePath)//モデル使いまわし用の解放処理
	{
	    models.erase(modelFilePath);
		textureManager_->EraseTexture(textureFilePath);
	    fileNames.erase(textureFilePath);
	}

	Model* ModelManager::FindModel(const std::string& filePath)
	{
		if (models.contains(filePath))
		{
		    return models[filePath].model_.get();
		}
		return nullptr;
	}
}