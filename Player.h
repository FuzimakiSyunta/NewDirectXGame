#pragma once

#include "Input.h"
#include "Model.h"
#include "PlayerBullet.h"
#include "WorldTransform.h"
#include <list>
#include <Sprite.h>

/// <summary>
/// 自キャラ
/// </summary>
class Player {

public:
	// デストラクタ
	~Player();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(Model* model, uint32_t textureHndle, Vector3 playerPotision);

	/// <summary>
	/// 自キャラ
	/// 更新
	/// </summary>
	void Update(ViewProjection& viewProjection);

	/// <summary>
	/// 描画
	/// </summary>
	void Draw(ViewProjection& viewProjection);

	// ワールド座標を取得
	Vector3 GetWorldPosition();

	// コールバック関数
	void OnCollision();

	// 親子関係を結ぶ
	void Setparent(const WorldTransform* parent);

	// const WorldTransform& GetWorldTransform() { return worldTransform_; }

	// 弾リストを取得
	const std::list<PlayerBullet*>& GetBullets() const { return bullets_; }
	/// <summary>
	/// UI描画
	/// </summary>
	void DrawUI();

private:
	void Rotate();

	void Attack();

private:
	WorldTransform worldTransform_;
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Player* player_ = nullptr;
	Input* input_ = nullptr;
	float inputFloat[3] = {0, 0, 0};
	// 弾
	std::list<PlayerBullet*> bullets_;
	// 3Dレティクル　
	WorldTransform worldTransform3DReticle_;
	// 2Dレティクル用スプライト
	Sprite* sprite2DReticle_ = nullptr;
	Vector2 ReticlePos = {640, 320};
	Vector3 positionReticle;
	// コントローラー
	XINPUT_STATE joyState;
};