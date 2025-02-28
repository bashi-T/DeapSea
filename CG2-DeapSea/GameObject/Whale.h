#pragma once
#include"Input/Input.h"
#include"Objects/Object3d.h"
#include"Commons/Object3dCommon.h"
#include"Objects/Particle.h"
#include"Player.h"
#include"Commons/ParticleCommon.h"
#include"Objects/Particle.h"

using namespace MyEngine;

class Whale
{
public:
	~Whale();
	void Initialize(Player* players);
	void Update();
	void Draw();
	void SetMaxPosition(float pos, float originalPos, float maxPos);

	void ChangeModel(int32_t whaleLife);
	void OnCollision();
	void OnTideCollision(Vector3 tideVector);
	void SetTranslate(Vector3 translate);
	void SetRotate(Vector3 rotate);
	void SetLife(int32_t i) { life = i; }
	void SetColor(Vector4 color) { object3d_->SetColor(color); }
	const Vector3& GetTranslate()const { return object3d_->GetTranslate(); }
	const Vector3& GetRotate()const { return object3d_->GetRotate(); }
	const AABB& GetCollision()const { return wCollision; }
	const int32_t& GetLife()const { return life; }
	const bool& GetIsHit()const { return isHit; }
	const float& GetMaxDistance()const { return maxDistance; }
	Matrix4x4 GetWorldMatrix() { return object3d_->GetObjectMatrix(); }

	void ChangeModel(std::string shape, std::string skin);
private:
	void Move();
	
	std::unique_ptr<Object3d> object3d_;
	std::unique_ptr<Particle> particle_;
	ModelManager* modelManager_ = nullptr;

	Player* player = nullptr;
	Vector3 whaleSpeed = { 1.0f,1.0f,1.0f };
	Vector3 nowWhaleSpeed = { 0.0f,0.0f,0.0f };
	Vector3 accVector = { 0.0f,0.0f,0.0f };
	bool isDead = false;
	bool isHit = false;
	AABB wCollision = {};
	int32_t life = 4;
	uint32_t coolTimer = 0;
	const float maxDistance = 6.0f;
	const float accSpeed = 0.001f;

	std::string whaleModel = "whale/improvisedWhale2.obj";
	std::string whaleSkin = "Resource/whale/whaleSkin.png";
};