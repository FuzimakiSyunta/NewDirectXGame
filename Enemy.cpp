﻿#include "Enemy.h"
#include <cassert>
#define _USE_MATH_DEFINES
#include "ImGuiManager.h"
#include "Player.h"
#include "keisan.h"
#include <math.h>

void Enemy::Initialize(Model* model, uint32_t textureHndle) {
	assert(model);
	model_ = model;
	textureHandle_ = TextureManager::Load("Enemy.png");
	worldTransform_.Initialize();
	input_ = Input::GetInstance();
	worldTransform_.scale_ = {20.0f, 20.0f, 20.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 300.0f};
	Approach();
	// 解放
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}

	//enemyMove = &Enemy::shot; // ポインタに関数のアドレスを代入
}

void Enemy::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);

	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}

// フェーズの関数テーブル
void (Enemy::*Enemy::enemyMove[])() = {
    &Enemy::shot, // 接近
    &Enemy::run   // 離脱
};

void Enemy::Update() {

	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	Vector3 move = {0, 0, -0.2f};
	Vector3 leave = {0.6f, 0.6f, -1.0f};
	const float kCharacterSpeed = 0.2f;

	
	(this->*enemyMove[static_cast<size_t>(phase_)])(); // shotを呼び出す

	float EnemyPos[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};

	// 画面の座標を表示
	ImGui::Begin("Enemy");
	ImGui::SliderFloat3("EnemyPos", EnemyPos, 600.0f, -600.0f);
	ImGui::End();

	worldTransform_.translation_.x = EnemyPos[0];
	worldTransform_.translation_.y = EnemyPos[1];
	worldTransform_.translation_.z = EnemyPos[2];

	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	worldTransform_.TransferMatrix();

	// 接近フェーズの更新関数
	--fireTimer_;
	if (fireTimer_ < 0) {
		Fire();
		fireTimer_ = kFireInterval;
	}

	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}
}
Enemy::~Enemy() {}

void Enemy::Approach() {
	// 発射タイマーを初期化
	fireTimer_ = 30;
}

void Enemy::Fire() {
	assert(player_);
	const float kBulletSpeed = -0.3f;
	Vector3 speed = {2.0f, 2.0f, 2.0f};
	Vector3 velocity(0, 0, kBulletSpeed);

	Vector3 subVector = Subtract(player_->GetWorldPosition(), GetWorldPosition());
	subVector = Normalize(subVector);
	subVector = VectorMultiply(subVector, speed);

	velocity = subVector;

	// 向き
	// velocity = TransformNormal(velocity, worldTransform_.matWorld_);
	// 弾を生成し、初期化
	EnemyBullet* newBullet = new EnemyBullet();
	newBullet->Initialize(model_, worldTransform_.translation_, velocity);
	// 弾を登録する
	bullets_.push_back(newBullet);
}

void Enemy::shot() {
	Vector3 move = {0, 0, -0.2f};
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	if (worldTransform_.translation_.z < 100.0f) {
		phase_ = Phase::Leave;
	}
}

void Enemy::run() {
	Vector3 leave = {0.6f, 0.6f, -1.0f};
	worldTransform_.translation_ = Add(worldTransform_.translation_, leave);
}

Vector3 Enemy::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision() {}
