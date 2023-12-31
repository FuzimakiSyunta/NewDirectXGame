#include "GameScene.h"
#include "AxisIndicator.h"
#include "ImGuiManager.h"
#include "TextureManager.h"
#include <cassert>

GameScene::GameScene() {}

GameScene::~GameScene() {
	delete model_;
	delete player_;
	delete debugCamera_;
	delete modelSkydome_;
	delete railCamera_;
}

void GameScene::Initialize() {

	dxCommon_ = DirectXCommon::GetInstance();
	input_ = Input::GetInstance();
	audio_ = Audio::GetInstance();
	textureHandle_ = TextureManager::Load("Cubeplayer.png");
	model_ = Model::Create();
	worldTransform_.Initialize();
	viewProjection_.Initialize();
	player_ = new Player();
	Vector3 playerPosition(0, 0, -1);
	player_->Initialize(model_, textureHandle_, playerPosition);

	// 敵キャラの作成
	enemy_ = new Enemy();
	// 敵キャラの初期化
	enemy_->Initialize(model_, textureHandle_);

	modelSkydome_ = Model::CreateFromOBJ("skydome", true);
	skydome_ = new Skydome();
	skydome_->Initialize(modelSkydome_, textureHandle_);

	// レールカメラ
	railCamera_ = new RailCamera();
	railCamera_->Initialize(worldPos, rotate);

	debugCamera_ = new DebugCamera(100, 100);

	//// 自キャラとレールカメラの親子関係を結ぶ
	//player_->Setparent(&railCamera_->GetWorldTransform());

	AxisIndicator::GetInstance()->SetVisible(true);
	AxisIndicator::GetInstance()->SetTargetViewProjection(&viewProjection_);
	// 敵キャラに自キャラのアドレスを渡す
	enemy_->SetPlayer(player_);
}

void GameScene::Update() {
	player_->Update();
	enemy_->Update();
	skydome_->Update();
	GameScene::CheakAllCollisions();
	debugCamera_->Update();
	railCamera_->Update();
#ifdef _DEBUG
	if (input_->TriggerKey(DIK_P)) {
		isDebugCameraActive_ = !isDebugCameraActive_;
	}
#endif;

	// カメラの処理
	if (isDebugCameraActive_) {
		debugCamera_->Update();
		viewProjection_.matView = debugCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = debugCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の更新
		viewProjection_.TransferMatrix();
	} else {
		// ビュープロジェクション行列の更新と転送
		viewProjection_.matView = railCamera_->GetViewProjection().matView;
		viewProjection_.matProjection = railCamera_->GetViewProjection().matProjection;
		// ビュープロジェクション行列の更新
		viewProjection_.TransferMatrix();
	}
}

void GameScene::Draw() {

	// コマンドリストの取得
	ID3D12GraphicsCommandList* commandList = dxCommon_->GetCommandList();

#pragma region 背景スプライト描画
	// 背景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに背景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();
	// 深度バッファクリア
	dxCommon_->ClearDepthBuffer();
#pragma endregion

#pragma region 3Dオブジェクト描画
	// 3Dオブジェクト描画前処理
	Model::PreDraw(commandList);

	/// <summary>
	/// ここに3Dオブジェクトの描画処理を追加できる
	/// </summary>
	player_->Draw(viewProjection_);
	enemy_->Draw(viewProjection_);
	skydome_->Draw(viewProjection_);

	// 3Dオブジェクト描画後処理
	Model::PostDraw();
#pragma endregion

#pragma region 前景スプライト描画
	// 前景スプライト描画前処理
	Sprite::PreDraw(commandList);

	/// <summary>
	/// ここに前景スプライトの描画処理を追加できる
	/// </summary>

	// スプライト描画後処理
	Sprite::PostDraw();

#pragma endregion
}

void GameScene::CheakAllCollisions() {
	// 判定対象AとBの座標
	Vector3 posA, posB, posC, posD;

	// 2間点の距離(自キャラと敵弾の当たり判定)
	float posAB;

	// 自キャラの半径
	float playerRadius = 40.0f;
	// 敵弾の半径
	float enemyBulletRadius = 30.0f;

	// 自弾リストの取得
	const std::list<PlayerBullet*>& playerBullets = player_->GetBullets();
	// 敵弾リストの取得
	const std::list<EnemyBullet*>& enemyBullets = enemy_->GetBullets();

#pragma region 自キャラと敵弾の当たり判定
	// 自キャラのワールド座標
	posA = player_->GetWorldPosition();

	// 自キャラと敵弾すべての当たり判定
	for (EnemyBullet* bullet : enemyBullets) {
		// 敵弾の座標
		posB = bullet->GetWorldPosition();
		// AとBの距離を求める
		posAB = (posB.x - posA.x) * (posB.x - posA.x) + (posB.y - posA.y) * (posB.y - posA.y) +
		        (posB.z - posA.z) * (posB.z - posA.z);
		// 球と球の当たり判定
		if (posAB <= (playerRadius + enemyBulletRadius) * (playerRadius + enemyBulletRadius)) {
			// 自キャラの衝突時コールバックを呼び出す
			player_->OnCollision();
			// 敵弾の衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵キャラの当たり判定
	// 自弾の半径
	float playerBulletRadius = 3.0f;
	// 敵キャラの半径
	float enemyRadius = 30.0f;

	// 敵キャラのワールド座標
	posC = enemy_->GetWorldPosition();

	// 2間点の距離(自弾と敵キャラの当たり判定)
	float posCD;

	// 自弾と敵キャラの当たり判定
	for (PlayerBullet* bullet : playerBullets) {
		// 自弾の座標
		posD = bullet->GetWorldPosition();
		// AとBの距離を求める
		posCD = (posC.x - posD.x) * (posC.x - posD.x) + (posC.y - posD.y) * (posC.y - posD.y) +
		        (posC.z - posD.z) * (posC.z - posD.z);

		// 球と球の当たり判定
		if (posCD <= (playerBulletRadius + enemyRadius) * (playerBulletRadius + enemyRadius)) {
			// 自弾の衝突時コールバックを呼び出す
			enemy_->OnCollision();
			// 敵キャラの衝突時コールバックを呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 自弾と敵弾の当たり判定
	float posBD;
	for (PlayerBullet* playerbullet : playerBullets) {
		for (EnemyBullet* enemybullet : enemyBullets) {
			posBD = (posB.x - posD.x) * (posB.x - posD.x) + (posB.y - posD.y) * (posB.y - posD.y) +
			        (posB.z - posD.z) * (posB.z - posD.z);
			// 球と球の当たり判定
			if (posBD <= (playerBulletRadius + enemyBulletRadius) *
			                 (playerBulletRadius + enemyBulletRadius)) {
				// 自弾の衝突時コールバックを呼び出す
				enemybullet->OnCollision();
				// 敵キャラの衝突時コールバックを呼び出す
				playerbullet->OnCollision();
			}
		}
	}
#pragma endregion
}
void GameScene::AddEnemyBullet(EnemyBullet* enemyBullet) {
	// リストに登録する
}
