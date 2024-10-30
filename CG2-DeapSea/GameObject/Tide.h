#pragma once
#include"Objects/Object3d.h"
#include"Commons/Object3dCommon.h"
#include"Objects/Particle.h"

class Tide
{
public:
	~Tide();
	void Initialize();
	void Update();
	void Draw();

private:
	Object3d* object3d;
	Particle* particle;

};

