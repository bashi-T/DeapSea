#include "titleScene.h"
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define new ::new(_NORMAL_BLOCK, __FILE__, __LINE__)

namespace MyEngine
{
	void TitleScene::Initialize()
	{
	}

	std::unique_ptr<BaseScene> TitleScene::Update()
	{
		if (isSceneChange == true)
		{
		    return std::make_unique<GameScene>();
		}
		else
		{
		    return nullptr;
		}

	}

	void TitleScene::Draw()
	{
	}
}
