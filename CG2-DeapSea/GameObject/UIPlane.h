#pragma once
#include"Objects/Object3d.h"
#include"Commons/Object3dCommon.h"

class UIPlane
{
public:
	~UIPlane();
	void Initialize(const std::string plane,const std::string skin);
	void Update();
	void Draw();

	Vector3 GetTranslate() { return object3d->GetTranslate(); }
	void SetTranslate(Vector3 translate) { object3d->SetTranslate(translate); }
	Vector3 GetScale() { return object3d->GetScale(); }
	void SetScale(Vector3 scale) { object3d->SetScale(scale); }
private:
	Object3d* object3d = nullptr;

};

