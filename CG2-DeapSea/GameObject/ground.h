#pragma once
#include"Objects/Object3d.h"
#include"Commons/Object3dCommon.h"

using namespace MyEngine;

class Ground
{
public:
	~Ground();
	void Initialize();
	void Update();
	void Draw();

	Vector3 GetTranslate() { return object3d_->GetTranslate(); }
	void SetTranslate(Vector3 translate) { object3d_->SetTranslate(translate); }
private:
	std::unique_ptr<Object3d> object3d_;
	ModelManager* modelManager_ = nullptr;
	std::string groundModel = "ground/newground.obj";
	const std::string groundSkin = "Resource/ganban.png";

};