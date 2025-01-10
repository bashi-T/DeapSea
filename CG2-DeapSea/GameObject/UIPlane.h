#pragma once
#include"Objects/Object3d.h"
#include"Commons/Object3dCommon.h"

using namespace MyEngine;
class UIPlane
{
public:
	~UIPlane();
	void Initialize(std::string plane, std::string skin);
	void Update();
	void Draw();

	Vector3 GetTranslate() { return object3d_->GetTranslate(); }
	void SetTranslate(Vector3 translate) { object3d_->SetTranslate(translate); }
	Vector3 GetScale() { return object3d_->GetScale(); }
	void SetScale(Vector3 scale) { object3d_->SetScale(scale); }
	Vector3 GetRotate() { return object3d_->GetRotate(); }
	void SetRotate(Vector3 rotate) { object3d_->SetRotate(rotate); }
	Vector4 GetColor() { return model->GetMaterial()->color; }
	void SetColor(Vector4 color);
private:
	std::unique_ptr<Object3d> object3d_;
	ModelManager* modelManager_ = nullptr;

	std::string planeModel;
	std::string planeSkin;
	Model* model = nullptr;
};