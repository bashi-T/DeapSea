#pragma once
#include"Objects/Object3d.h"
#include"Commons/Object3dCommon.h"
#include"Commons/ParticleCommon.h"
#include"Objects/Particle.h"

using namespace MyEngine;

class Shield
{
public:
	~Shield();
	void Initialize();
	void Update();
	void Draw();

private:
	std::unique_ptr< Object3d> object3d;
	std::vector < std::unique_ptr<Particle>> particles;
};