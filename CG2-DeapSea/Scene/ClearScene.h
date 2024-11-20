#pragma once
#include"Input/Input.h"
#include"SceneManager.h"
#include"Objects/Object3d.h"
#include"Commons/Object3dCommon.h"
#include"Objects/Particle.h"
#include"Sprites/Sprite.h"
#include"Commons/SpriteCommon.h"
#include"GameObject/Whale.h"
#include"GameObject/Player.h"
#include"GameObject/Sand.h"

class ClearScene :public SceneManager
{
public:
	//~ClearScene();
	void Init() override;
	void Update() override;
	void Draw() override;
	void Finalize() override;

private:
	std::unique_ptr<Sprite> sprite;
	std::unique_ptr<Player> player_;
	std::unique_ptr<Whale> whale_;
	std::unique_ptr<Sand> sand_;

};

