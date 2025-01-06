#pragma once
#include"Objects/Object3d.h"
#include"Commons/Object3dCommon.h"
namespace MyEngine
{
	class SkyDome
	{
	public:
		~SkyDome();
		void Initialize();
		void Update();
		void Draw();

	private:
		std::shared_ptr< Object3d> object3d_;
		ModelManager* modelManager_;
	};
}

