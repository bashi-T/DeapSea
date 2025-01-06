#pragma once
#include"Input/Input.h"
#include"Objects/Object3d.h"
#include"Commons/Object3dCommon.h"
#include"Objects/Particle.h"

using namespace MyEngine;

class PlayerBullet
{
public:
	~PlayerBullet();
	void Initialize(Vector3 firstPos, Matrix4x4 angle);
	void Update();
	void Draw();
	void OnCollision();
	bool IsDead()const { return isDead; }

	void SetTranslate(Vector3 translate);

	Vector3 GetTranslate() { return object3d_->GetTranslate(); }
	Sphere GetCollision() { return playerBulletCollision; }

private:
	std::unique_ptr<Object3d> object3d_;
	ModelManager* modelManager_;

	std::string bulletModel = "AnimatedCube/AnimatedCube.gltf";
	const std::string& bulletSkin = "Resource/AnimatedCube/AnimatedCube_BaseColor.png";
	Vector3 bulletSpeed = { 0.0f,0.0f,3.0f };
	static const int32_t kLifeTime = 60 * 2;
	int32_t deathTimer = kLifeTime;
	bool isDead = false;
	Sphere playerBulletCollision{};
};