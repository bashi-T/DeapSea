#pragma once
#include <iostream>
#include <memory>

enum Scene { TITLE, INGAME, GAMEOVER, CLEAR };

class BaseScene
{
public:
	virtual void Initialize() = 0;
	virtual std::unique_ptr<BaseScene> Update() = 0;
	virtual void Draw() = 0;
	virtual ~BaseScene() = default;

private:
	//std::unique_ptr<BaseScene> nextScene = nullptr;
};
