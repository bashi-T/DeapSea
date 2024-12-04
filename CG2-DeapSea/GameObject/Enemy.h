#pragma once
#include"Input/Input.h"
#include"Objects/Object3d.h"
#include"Commons/Object3dCommon.h"
#include"Objects/Particle.h"
#include"EnemyBullet.h"
#include"Player.h"
#include"Whale.h"

using namespace MyEngine;

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
		void SubtractLife() { life--; }

		bool IsDead()const { return isDead; }
		const std::list<std::unique_ptr<EnemyBullet>>& GetBullets()const { return eBullets; }
		Vector3 GetTranslate() { return object3d->GetTranslate(); }
		AABB GetCollision() { return eCollision; }
		int GetSort() { return enemySort; }
		int Getlife() { return life; }

	private:
		std::unique_ptr< Object3d> object3d;
		std::list<std::unique_ptr<EnemyBullet>> eBullets;
		Player* player_;
		Whale* whale_;

		uint32_t shotInterval = 0;
		bool isShot = false;
		bool isDead = false;
		AABB eCollision;
		Vector3 enemyVector;
		int enemySort;
		int moveInterval = 0;
		int life;
		int escapeTime = 0;
	};

