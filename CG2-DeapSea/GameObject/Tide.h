#pragma once
#include"Objects/Object3d.h"
#include"Commons/Object3dCommon.h"
#include"Commons/ParticleCommon.h"
#include"Objects/Particle.h"

using namespace MyEngine;

class Tide
{
public:
	~Tide();
	void Initialize();
	void Update();
	void Draw();

	void SetTranslate(Vector3 translate) { tideTranslate=translate; }

	AABB GetCollision() { return pCollision; }
	Vector3 GetTranslate() { return tideTranslate; }
	Vector3 GetTideVector() { return tideVector; }

private:
	//std::unique_ptr< Object3d> object3d;
	std::vector<std::unique_ptr<Particle>> particles;
	AABB pCollision;

	Vector3 tideVector = { 0.125f,0.0f,0.0f };
	Vector3 tideTranslate;
};
