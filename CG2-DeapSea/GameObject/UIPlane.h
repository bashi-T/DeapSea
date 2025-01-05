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

	Vector3 GetTranslate() { return object3d->GetTranslate(); }
	void SetTranslate(Vector3 translate) { object3d->SetTranslate(translate); }
	Vector3 GetScale() { return object3d->GetScale(); }
	void SetScale(Vector3 scale) { object3d->SetScale(scale); }
	Vector3 GetRotate() { return object3d->GetRotate(); }
	void SetRotate(Vector3 rotate) { object3d->SetRotate(rotate); }
	Vector4 GetColor() { return model->GetMaterial()->color; }
	void SetColor(Vector4 color);
private:
	std::unique_ptr< Object3d> object3d;
	std::shared_ptr<ModelManager>modelManager_;

	std::string planeModel;
	std::string planeSkin;
	Model* model = nullptr;
};