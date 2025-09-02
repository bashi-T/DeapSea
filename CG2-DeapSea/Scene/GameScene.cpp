#include "GameScene.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

namespace MyEngine
{
	void GameScene::Initialize()
	{
	}

	std::unique_ptr<BaseScene> GameScene::Update()
	{
		if (isGameOverScene == true)
		{
		    return std::make_unique<GameOverScene>();
		}
		else if (isGameClearScene == true)
		{
		    return std::make_unique<ClearScene>();
		}
		else
		{
		    return nullptr;
		}
	}

	void GameScene::Draw()
	{
	}

	void GameScene::CheckAllCollisions()
	{
	}

    void GameScene::DirectGameStart() 
	{
    }

    void GameScene::DirectGameOver()
	{
    }

    void GameScene::DirectGameClear()
	{
    }

	void GameScene::LoadEnemyPopData(std::string filePath, int32_t fileNum)
	{
		std::ifstream file;
		file.open(filePath);
		assert(file.is_open());

		enemyPopCommands[fileNum] << file.rdbuf();
		file.close();
	}

	void GameScene::UpdateEnemyPopCommands(int32_t fileNum)
	{
		if (isWait)
		{
			WaitTimer--;
			if (WaitTimer <= 0)
			{
				isWait = false;
			}
			return;
		}
		std::string line;
		while (getline(enemyPopCommands[fileNum], line))
		{
			std::istringstream line_stream(line);
			std::string word;
			getline(line_stream, word, ',');

			if (word.find("//") == 0)
			{
				continue;
			}

			if (word.find("POP") == 0)
			{
				//x座標
				getline(line_stream, word, ',');
				float x = (float)std::atof(word.c_str());

				//y座標
				getline(line_stream, word, ',');
				float y = (float)std::atof(word.c_str());

				//z座標
				getline(line_stream, word, ',');
				float z = (float)std::atof(word.c_str()) + whale_->GetTranslate().z;

				std::random_device seedGenerator;
				std::mt19937 randomEngine(seedGenerator());
			    std::uniform_real_distribution<float> enemySort(0.0f, (float)(SceneManager::GetInstance()->GetStageNumber() + 1));
				std::unique_ptr<Enemy> enemy_ = std::make_unique<Enemy>();
				enemy_->SetSort((int)enemySort(randomEngine));
				enemy_->Initialize(player_.get(), whale_.get(), enemy_->GetSort());
			    if (SceneManager::GetInstance()->GetStageNumber() == 0)
				{
					enemy_->SetIsPractice(true);
				}
				enemy_->SetTranslate({ x,y,z });
				enemy_->SetEnemyVector(whale_->GetTranslate());
				enemys_.push_back(std::move(enemy_));
			}
			else if (word.find("WAIT") == 0)
			{
				getline(line_stream, word, ',');
				int32_t waitTime = atoi(word.c_str());
				isWait = true;
				WaitTimer = waitTime;
				break;
			}
			else if (word.find("END") == 0)
			{
				gameEnd = true;
				word.clear();
				line_stream.clear();
				line.clear();
			}
		}
	}
}