#pragma once
#include"Input/Input.h"
#include"SceneManager.h"
#include"Sprites/Sprite.h"
#include"Commons/SpriteCommon.h"

class GameScene:public SceneManager
{
public:
	void Init() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;
private:
	//Input* input;
	//SpriteCommon* SPCommon;
	//SRVManager* srvManager;
	std::vector<Sprite*> sprites;

};

