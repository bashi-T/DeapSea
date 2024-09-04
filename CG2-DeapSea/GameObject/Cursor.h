#pragma once
#include"Objects/Object3d.h"
#include"Commons/Object3dCommon.h"

class Cursor
{
public:
	~Cursor();
	void Initialize();
	void Update();
	void Draw();

	Vector3 GetTranslate() { return object3d->GetTranslate(); }
	void SetTranslate(Vector3 translate) { object3d->SetTranslate(translate); }
private:
	Object3d* object3d = nullptr;

};

