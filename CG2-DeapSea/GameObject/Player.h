#pragma once
#include"Input/Input.h"
#include"Objects/Object3d.h"
#include"Commons/Object3dCommon.h"
#include"Objects/Particle.h"
#include"PlayerBullet.h"
#include"Shield.h"

using namespace MyEngine;

class Player
{
public:
	~Player();
	void Initialize(Camera* camera);
	void Update(Vector3 whalePos);
	void Draw();
	void OnCollision();
	void SetMaxPosition(float pos,float maxPos);

	const std::list<std::unique_ptr<PlayerBullet>>& GetBullets() const { return pBullets; }
	const std::unique_ptr<Shield>& GetShield() const { return shield_; }
	Vector3 GetRotate() { return object3d_->GetRotate(); }
	Vector3 GetTranslate() { return object3d_->GetTranslate(); }
	Matrix4x4 GetWorldMatrix() { return object3d_->GetObjectMatrix(); }
	const AABB& GetAABBCollision() const{ return collision; }
	const bool& GetIsHit()const { return isHit; }
	const bool& GetIsMovable()const { return isMovable; }
	const uint32_t& GetIsHitTimer()const { return hitTimer; }
	const Vector3& GetMoveVector()const { return moveVector; }

	void SetRotate(Vector3 rotate) { object3d_->SetRotate(rotate); }
	void SetTranslate(Vector3 translate) { object3d_->SetTranslate(translate); }
	void SetIsHit(bool isHit_) { isHit = isHit_; }
	void SetIsShot(bool isShot_) { isShot = isShot_; }
	void SetIsMovable(bool isMovable_) { isMovable = isMovable_; }

private:
	void Shot();
	void Move();
	void Guard(Vector3 whalePos);

	std::unique_ptr<Object3d> object3d_;
	ModelManager* modelManager_ = nullptr;
	Camera* camera_ = nullptr;

	std::list<std::unique_ptr<PlayerBullet>> pBullets;
	std::unique_ptr<Shield> shield_;
	AABB collision = {};
	Vector3 moveVector = {};

	uint32_t shotInterval = 0;
	bool isShot = false;
	bool isGuard = false;
	bool isHit = false;
	bool isMovable = false;
	int shieldTimer = 0;
	uint32_t shieldCoolTimer = 0;
	uint32_t hitTimer = 0;
	float angle_ = 0.0f;
	std::string playerModel = "human/walk.gltf";
	const std::string& playerSkin = "Resource/AnimatedCube/AnimatedCube_BaseColor.png";

	const float collisionwidth = 2.0f;
	const float moveSpeed = 0.15f;
};