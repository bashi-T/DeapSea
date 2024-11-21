#pragma once
#include"Objects/Object3d.h"
#include"Commons/Object3dCommon.h"

using namespace MyEngine;

class Sand
	{
	public:
		~Sand();
		void Initialize();
		void Update();
		void Draw();

		Vector3 GetTranslate() { return object3d->GetTranslate(); }
		void SetTranslate(Vector3 translate) { object3d->SetTranslate(translate); }
	private:
		std::unique_ptr< Object3d> object3d;
		std::string groundModel = "floor/floor.obj";
		const std::string& groundSkin = "Resource/sand.png";

	};