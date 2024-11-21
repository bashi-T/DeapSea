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
		Object3d* object3d;

	};
}

