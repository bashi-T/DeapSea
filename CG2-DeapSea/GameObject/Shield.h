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

	void SetTranslate(Vector3 translate) { object3d_->SetTranslate(translate); }
	void SetRadius(float r) { collision.radius = r; }
	float GetRadius() { return collision.radius; }
	Vector3 GetTranslate() { return object3d_->GetTranslate(); }
	Sphere GetCollision() { return collision; }

private:
	std::unique_ptr< Object3d> object3d_;
	std::vector < std::unique_ptr<Particle>> particles_;
	ModelManager* modelManager_ = nullptr;

	bool isDead = false;
	Sphere collision{};

	std::string shieldModel = "axis/axis.obj";
	std::string shieldSkin = "Resource/colorbabble.png";
};