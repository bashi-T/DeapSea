#pragma once
#include"Objects/Object3d.h"
#include"Commons/Object3dCommon.h"
namespace MyEngine
{
	class SkyDome
	{
	public:
		~SkyDome();
		void Initialize(Camera* camera);
		void Update();
		void Draw();

	private:
		std::unique_ptr<Object3d> object3d_;
		ModelManager* modelManager_;
		Camera* camera_;
	};
}

