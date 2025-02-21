#pragma once
#include"BaseScene.h"
#include"TitleScene.h"

#include"Input/Input.h"
#include"Objects/Object3d.h"
#include"Commons/Object3dCommon.h"
#include"Objects/Particle.h"
#include"Sprites/Sprite.h"
#include"Commons/SpriteCommon.h"
#include"GameObject/Whale.h"
#include"GameObject/Player.h"
#include"GameObject/Sand.h"

namespace MyEngine
{
	class ClearScene :public BaseScene
	{
	public:
		void Initialize() override;
	    std::unique_ptr<BaseScene> Update() override;
		void Draw() override;

	private:
	    std::unique_ptr<BaseScene> nextScene = nullptr;
	    Camera* camera_ = nullptr;
		std::unique_ptr<Sprite> sprite_;
		std::unique_ptr<Player> player_;
		std::unique_ptr<Whale> whale_;
		std::unique_ptr<Sand> sand_;

	};
}

