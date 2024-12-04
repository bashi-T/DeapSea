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

	void OnCollision();
	void OnTideCollision(Vector3 tideVector);
	void SetTranslate(Vector3 translate);
	void SetRotate(Vector3 rotate);
	void SetLife(int i) { life = i; }
	void SetColor(Vector4 color) { object3d->SetColor(color); }
	const Vector3& GetTranslate()const { return object3d->GetTranslate(); }
	const Vector3& GetRotate()const { return object3d->GetRotate(); }
	const AABB& GetCollision()const { return wCollision; }
	const int& GetLife()const { return life; }
	const bool& GetIsHit()const { return isHit; }
	const float& GetMaxDistance()const { return maxDistance; }
	void ChangeModel(std::string shape, std::string skin);
private:
	std::unique_ptr< Object3d> object3d;
	Player* player = nullptr;
	std::unique_ptr<Particle> particle;
	Vector3 whaleSpeed = { 0.1f,0.1f,0.1f };
	Vector3 nowWhaleSpeed = { 0.0f,0.0f,0.0f };
	Vector3 accSpeed = { 0.0f,0.0f,0.0f };
	bool isDead = false;
	bool isHit = false;
	AABB wCollision = {};
	int life = 4;
	uint32_t coolTimer = 0;
	float maxDistance = 6.0f;
	std::string whaleModel = "whale/improvisedWhale2.obj";
	std::string whaleSkin = "Resource/whaleSkin.png";
};