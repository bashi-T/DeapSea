#pragma once
#include"Input/Input.h"
#include"Objects/Object3d.h"
#include"Commons/Object3dCommon.h"
#include"Objects/Particle.h"

using namespace MyEngine;

class EnemyBullet
{
public:
	~EnemyBullet();
	void Initialize(Vector3 firstPos);
	void Update();
	void Draw();
	void OnCollision();
	bool IsDead()const { return isDead; }

	void SetTranslate(Vector3 translate);
	void SetEnemyBulletVector(Vector3 translate);
	Vector3 GetTranslate() { return object3d_->GetTranslate(); }
	Sphere GetCollision() { return enemyBulletCollision; }

private:
	void Move();

	std::unique_ptr<Object3d> object3d_;
	ModelManager* modelManager_ = nullptr;

	std::string bulletModel = "AnimatedCube/AnimatedCube.gltf";
	const std::string bulletSkin = "Resource/circle.png";
	Vector3 bulletSpeed = { 0.0f,0.0f,-0.1f };
	bool isDead = false;
	Sphere enemyBulletCollision;
	Vector3 enemyBulletVector = {};
};