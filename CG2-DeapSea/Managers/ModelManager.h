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
	private:
		ModelManager() = default;
		~ModelManager() = default;
		ModelManager(ModelManager&) = delete;
		ModelManager& operator=(ModelManager&) = delete;

		static ModelManager* instance;
		ModelCommon* modelCommon_ = nullptr;

		std::map<std::string, std::unique_ptr<Model>>models;
		std::vector<std::string>modelFilePaths;
		int sameModelNum = 0;

	public:
		static ModelManager* GetInstance();

		void Finalize();
		void Initialize(DX12Common* dxCommon);
		void LoadModel(std::string& filePath, const std::string& TextureFilePath, bool isLighting);
		void LoadAnimationModel(std::string& filePath, const std::string& TextureFilePath, bool isLighting);
		void LoadSkeltonAnimation(std::string& filePath, const std::string& TextureFilePath, SRVManager* srvManager, bool isLighting);
		void EraseModel(const std::string& filePath, const std::string& textureFilePath);

		Model* FindModel(const std::string& filePath);

		ModelCommon* GetModelCommon() { return modelCommon_; }

	};
}

