#pragma once
#include"Input/Input.h"
#include"Objects/Object3d.h"
#include"Commons/Object3dCommon.h"
#include"Objects/Particle.h"
#include"EnemyBullet.h"
#include"Player.h"
#include"Whale.h"

class Enemy
{
public:
	~Enemy();
	void Initialize(Player* player, Whale* whale, int sort);
	void Update(int sort);
	void Draw(int sort);
	void Shot();
	void OnCollision();
	void SetTranslate(Vector3 translate);
	void SetEnemyVector(Vector3 translate);
	void SetSort(int i) { enemySort = i; }

	bool IsDead()const { return isDead; }
	const std::list<EnemyBullet*>& GetBullets()const { return eBullets; }
	Vector3 GetTranslate() { return object3d->GetTranslate(); }
	OBB GetCollision() { return eCollision; }
	int GetSort() { return enemySort; }
private:
	Object3d* object3d;
	std::list<EnemyBullet*> eBullets;
	Player* player_;
	Whale* whale_;

	uint32_t shotInterval = 0;
	bool isShot = false;
	bool isDead = false;
	OBB eCollision;
	Vector3 enemyVector;
	int enemySort;
	int moveInterval = 0;
	//const std::string enemyModel;
	//const std::string enemySkin;
};

