#pragma once
#include"Objects/Object3d.h"
#include"Commons/Object3dCommon.h"
#include"Commons/ParticleCommon.h"
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
	std::vector <Particle*> particles;

};

