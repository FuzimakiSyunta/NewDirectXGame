#pragma once

#include "Audio.h"
#include "DebugCamera.h"
#include "DirectXCommon.h"
#include "Enemy.h"
#include "EnemyBullet.h"
#include "Input.h"
#include "Model.h"
#include "Player.h"
#include "RailCamera.h"
#include "SafeDelete.h"
#include "Skydome.h"
#include "Sprite.h"
#include "ViewProjection.h"
#include "WorldTransform.h"
#include<sstream>

/// <summary>
/// ゲームシーン
/// </summary>

class GameScene {

public: // メンバ関数
	/// <summary>
	/// コンストクラタ
	/// </summary>
	GameScene();

	/// <summary>
	/// デストラクタ
	/// </summary>
	~GameScene();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 衝突判定と応答
	/// </summary>
	void CheakAllCollisions();

	/// <summary>
	/// 敵弾を追加しよう
	/// </summary>
	void AddEnemyBullet(EnemyBullet* enemyBullet);
	/// <summary>
	/// 敵データ読み込み
	/// </summary>
	void LoadEnemyPopData();
	/// <summary>
	/// 敵コマンド更新
	/// </summary>
	void UpdateEnemyPopCommands();
	
	//// 敵弾リストの取得
	//const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

private: // メンバ変数
	DirectXCommon* dxCommon_ = nullptr;
	Input* input_ = nullptr;
	Audio* audio_ = nullptr;
	uint32_t textureHandle_ = 0;
	uint32_t CleartextureHandle_ = 0u;
	Model* model_ = nullptr;

	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
	Player* player_ = nullptr;
	
	
	//Enemy* enemy_ = nullptr;
	// 敵
	std::list<Enemy*> enemys_;
	//// Enemyの生きてるかフラグ
	Enemy* isEnemy_ = nullptr;
	EnemyBullet* enemybullet_ = nullptr;
	// 弾
	std::list<EnemyBullet*> bullets_;
	Skydome* skydome_ = nullptr;
	Model* modelSkydome_ = nullptr;
	// レールカメラをゲームシーンに持たせる
	RailCmamera* railCamera_ = nullptr;
	Vector3 worldPos;
	Vector3 rotate;
	// デバッグカメラ有効
	bool isDebugCameraActive_ = false;
	// デバッグカメラ
	DebugCamera* debugCamera_ = nullptr;
	// 敵発生コマンド
	std::stringstream enemyPopCommands;

	// 待機時間
	bool isTime = true;
	int waitingTime = 120;
};
