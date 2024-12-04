#pragma once
#include"Input/Input.h"
#include"Objects/Object3d.h"
#include"Commons/Object3dCommon.h"
#include"Objects/Particle.h"
#include"PlayerBullet.h"

using namespace MyEngine;

class Player
{
public:
	~Player();
	void Initialize();
	void Update();
	void Draw();
	void Shot();
	void OnCollision();

	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets()const { return pBullets; }
	Vector3 GetTranslate() { return object3d->GetTranslate(); }
	const AABB& GetAABBCollision() const{ return pCollision; }
	//Sphere GetSphereCollision() { return pCollision; }
	const bool& GetIsHit()const { return isHit; }
	const bool& GetIsMovable()const { return isMovable; }
	const uint32_t& GetIsHitTimer()const { return hitTimer; }
	const Vector3& GetMoveVector()const { return moveVector; }

	void SetTranslate(Vector3 translate) { object3d->SetTranslate(translate); }
	void SetIsHit(bool isHit_) { isHit = isHit_; }
	void SetIsShot(bool isShot_) { isShot = isShot_; }
	void SetIsMovable(bool isMovable_) { isMovable = isMovable_; }

private:
	std::unique_ptr<Object3d> object3d;
	std::list<std::unique_ptr<PlayerBullet>> pBullets;
	AABB pCollision = {};
	Vector3 moveVector = {};

	uint32_t shotInterval = 0;
	bool isShot = false;
	bool isHit = false;
	bool isMovable = false;
	uint32_t hitTimer = 0;
	float angle_ = 0.0f;
	std::string playerModel = "human/walk.gltf";
	const std::string& playerSkin = "Resource/world.png";

	const float collisionwidth = 2.0f;
};