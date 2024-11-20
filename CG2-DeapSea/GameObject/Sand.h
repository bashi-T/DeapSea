#pragma once
#include"Objects/Object3d.h"
#include"Commons/Object3dCommon.h"
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
};

