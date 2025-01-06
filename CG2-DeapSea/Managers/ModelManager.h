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
		static ModelManager* GetInstance();

		void Finalize();
		void Initialize();
		void LoadModel(std::string& filePath, const std::string& TextureFilePath, bool isLighting);
		void LoadAnimationModel(std::string& filePath, const std::string& TextureFilePath, bool isLighting);
		void LoadSkeltonAnimation(std::string& filePath, const std::string& TextureFilePath, bool isLighting);
		void EraseModel(const std::string& filePath, const std::string& textureFilePath);

		Model* FindModel(const std::string& filePath);

		ModelCommon* GetModelCommon() { return modelCommon_; }

		ModelManager() = default;
		~ModelManager() = default;

	private:
		ModelManager(ModelManager&) = delete;
		ModelManager& operator=(ModelManager&) = delete;

		static inline std::unique_ptr<ModelManager> instance;

		ModelCommon* modelCommon_ = nullptr;
		DX12Common* dx12Common_ = nullptr;
		TextureManager* textureManager_ = nullptr;

		std::map<std::string, std::unique_ptr<Model>>models;
		std::vector<std::string>modelFilePaths;
		int32_t sameModelNum = 0;

	};
}

