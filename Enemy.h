#pragma once
#include "EnemyBullet.h"
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"

#include <list>

class Player;

class Enemy {

public: // メンバ関数
	// デストラクタ
	~Enemy();

	void Initialize(Model* model, uint32_t textureHndle);

	void Update();

	void Draw(ViewProjection& viewProjection);

	Vector3 GetWorldPosition();
	void SetPlayer(Player* player) { player_ = player; }

	// コールバック関数
	void OnCollision();

	// 弾リストを取得
	const std::list<EnemyBullet*>& GetBullets() const { return bullets_; }

private: // メンバ関数
	void Fire();
	void shot(); // メンバ関数
	void run();
	void Approach();
	static void (Enemy::*enemyMove[])(); // メンバ関数ポインタ
	// 寿命
	static const int32_t kLifeTime = 10;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;

private: // メンバ変数
	// 発射間隔
	static const int kFireInterval = 60;

	// 自キャラ
	enum class Phase {
		approach,
		Leave,
	};

	Player* player_ = nullptr;

	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	uint32_t CleartextureHandle_ = 0u;
	Enemy* enemy_ = nullptr;
	Input* input_ = nullptr;
	Phase phase_ = Phase::approach;
	// 発射タイマー
	int32_t fireTimer_ = 0;
	// 弾
	std::list<EnemyBullet*> bullets_;
};