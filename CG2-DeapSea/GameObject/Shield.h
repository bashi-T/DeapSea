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
	void Update(Vector3 playerPos, Vector3 whalePos);
	void Draw();

	void SetIsDead(bool isdead) { isDead = isdead; }
	const bool& IsDead() const { return isDead; }

	void SetRadius(float r) { collision.radius = r; }
	float GetRadius() { return collision.radius; }
	Sphere GetCollision() { return collision; }

private:
	std::unique_ptr<Particle> particle_;
	ModelManager* modelManager_ = nullptr;

	bool isDead = false;
	Sphere collision{};

	float rotateSpeed = 0.0f;
	std::string shieldModel = "axis/axis.obj";
	std::string shieldSkin = "Resource/colorbabble.png";
};