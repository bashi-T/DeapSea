#pragma once

#include<string>
#include<map>
#include<memory>
#include"Objects/Model.h"
#include "Commons/DX12Common.h"

namespace MyEngine
{
	class ModelManager
	{
	public:
		static std::shared_ptr<ModelManager> GetInstance();

		void Finalize();
		void Initialize();
		void LoadModel(std::string& filePath, const std::string& TextureFilePath, bool isLighting);
		void LoadAnimationModel(std::string& filePath, const std::string& TextureFilePath, bool isLighting);
		void LoadSkeltonAnimation(std::string& filePath, const std::string& TextureFilePath, bool isLighting);
		void EraseModel(const std::string& filePath, const std::string& textureFilePath);

		Model* FindModel(const std::string& filePath);

		std::shared_ptr<ModelCommon> GetModelCommon() { return modelCommon_; }

		~ModelManager() = default;
		ModelManager(ModelManager&) = delete;
		ModelManager& operator=(ModelManager&) = delete;
	protected:
		ModelManager() = default;
	private:

		static inline std::weak_ptr<ModelManager> instance;
		std::shared_ptr<ModelCommon> modelCommon_ = nullptr;
		std::shared_ptr<DX12Common> dx12Common_ = nullptr;
		std::shared_ptr<TextureManager> textureManager_ = nullptr;

		std::map<std::string, std::unique_ptr<Model>>models;
		std::vector<std::string>modelFilePaths;
		int32_t sameModelNum = 0;

	};
}

