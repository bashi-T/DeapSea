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

namespace MyEngine
{
	class GameOverScene :public SceneManager
	{
	public:
		void Initialize() override;
		void Update() override;
		void Draw() override;
		void Finalize() override;

	private:
		std::unique_ptr<Sprite> sprite_;
		std::unique_ptr<Player> player_;
		std::unique_ptr<Whale> whale_;
		std::unique_ptr<Sand> sand_;

		std::string boneWhaleModel = "whale/BoneWhale.obj";
		std::string boneWhaleSkin = "Resource/boneColor.png";
		int floatingTime = 0;

	};
}
