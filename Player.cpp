#include "Player.h"
#include <cassert>
#define _USE_MATH_DEFINES
#include "ImGuiManager.h"
#include "keisan.h"
#include <math.h>
#include"WinApp.h"


void Player::Initialize(Model* model, uint32_t textureHndle, Vector3 playerPotision) {
	assert(model);
	model_ = model;
	textureHandle_ = textureHndle;
	worldTransform_.Initialize();
	input_ = Input::GetInstance();
	worldTransform_.scale_ = {3.0f, 3.0f, 3.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 30.0f};
	worldTransform3DReticle_.Initialize();
	uint32_t textureReticle = TextureManager::Load("./Resources/Reticle.png");
	ReticlePos.x = 760;
	ReticlePos.y = 320;
	sprite2DReticle_ =
	    Sprite::Create(textureReticle, ReticlePos, {1.0f, 1.0f, 1.0f, 1.0f}, {0.5f, 0.5f});
	//// 解放
	//for (PlayerBullet* bullet : bullets_) {
	//	delete bullet;
	//}
}

void Player::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection, textureHandle_);
	// if (bullet_) {
	//	bullet_->Draw(viewProjection);
	// }
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(viewProjection);
	}
}
void Player::DrawUI() { sprite2DReticle_->Draw(); }

void Player::Update(ViewProjection& viewProjection) {
	{   //2-15
		POINT mousePosition;
		// マウス座標を取得
		GetCursorPos(&mousePosition);
		// クライアントエリア座標に変換する
		HWND hwnd = WinApp::GetInstance()->GetHwnd();
		ScreenToClient(hwnd, &mousePosition);

		Vector2 Reticle;
		Reticle.x = float(ReticlePos.x);
		Reticle.y = float(ReticlePos.y);
		sprite2DReticle_->SetPosition(Reticle);
		//ビュープロジェクションビューポート行列
		Matrix4x4 matVPV = Multiply(
		    viewProjection.matView,
		    Multiply(
		        viewProjection.matProjection,
		        MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1)));
		Matrix4x4 matInveresVPV = Inverse(matVPV);

		Vector3 PosNear = Vector3(float(ReticlePos.x), float(ReticlePos.y), 0);
		Vector3 PosFar = Vector3(float(ReticlePos.x), float(ReticlePos.y), 1);
		// スクリーン座標系からワールド座標系へ
		PosNear = Transform(PosNear, matInveresVPV);
		PosFar = Transform(PosFar, matInveresVPV);
		// マウスレイの方向
		Vector3 mouseDirection = Add(PosFar, PosNear);
		mouseDirection = Normalize(mouseDirection);
		// カメラから標準オブジェクトの距離
		const float kDistanceTestobject = 50.0f;
		worldTransform3DReticle_.translation_.x =
		    PosNear.x + mouseDirection.x * kDistanceTestobject;
		worldTransform3DReticle_.translation_.y =
		    PosNear.y + mouseDirection.y * kDistanceTestobject;
		worldTransform3DReticle_.translation_.z =
		    PosNear.z + mouseDirection.z * kDistanceTestobject;
		//
		worldTransform3DReticle_.UpdateMatrix();
		worldTransform3DReticle_.TransferMatrix();
		// 画面の座標を表示
		ImGui::Begin("Player");
		ImGui::Text("2DReticle:(%f,%f)", ReticlePos.x, ReticlePos.y);
		ImGui::Text("Near:(%+.2,%+.2,%+2f)", PosNear.x, PosNear.y, PosNear.z);
		ImGui::Text("Far:(%+.2,%+.2,%+2f)", PosFar.x, PosFar.y, PosFar.z);
		ImGui::Text(
		    "3DRaticle:(%+.2,%+.2,%+2f)", worldTransform3DReticle_.translation_.x,
		    worldTransform3DReticle_.translation_.y, worldTransform3DReticle_.translation_.z);
		ImGui::End();
		//2-14
		 const float kDistancePlayerTo3DRetocle = 50.0f;
		 Vector3 offset = {0, 0, 1.0f};
		 offset = TransformNormal(offset, worldTransform_.matWorld_);
		 offset = Normalize(offset);
		 offset.x *= kDistancePlayerTo3DRetocle;
		 offset.y *= kDistancePlayerTo3DRetocle;
		 offset.z *= kDistancePlayerTo3DRetocle;
		// 3Dレティクル
		 Vector3 Pos;
		 Pos.x = worldTransform_.matWorld_.m[3][0];
		 Pos.y = worldTransform_.matWorld_.m[3][1];
		 Pos.z = worldTransform_.matWorld_.m[3][2];
		 worldTransform3DReticle_.translation_.x = offset.x + Pos.x;
		 worldTransform3DReticle_.translation_.y = offset.y + Pos.y;
		 worldTransform3DReticle_.translation_.z = offset.z + Pos.z;
		 worldTransform3DReticle_.UpdateMatrix();
		 worldTransform3DReticle_.TransferMatrix();

		//3Dレティクルのワールド座標から2Dレティクルのスクリーン座標を計算
		 Vector3 positionReticle = GetWorldPosition();

		//
		 Matrix4x4 matViewport =
		     MakeViewportMatrix(0, 0, WinApp::kWindowWidth, WinApp::kWindowHeight, 0, 1);
		//
		 Matrix4x4 matViewProjectionViewport =
		     Multiply(viewProjection.matView, Multiply(viewProjection.matProjection,
		     matViewport));
		 positionReticle = Transform(positionReticle, matViewProjectionViewport);
		 sprite2DReticle_->SetPosition(Vector2(positionReticle.x, positionReticle.y));
	}

	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	Vector3 move = {0, 0, 0};
	const float kCharacterSpeed = 0.2f;

	if (input_->PushKey(DIK_LEFT)) {

		move.x -= kCharacterSpeed;
	} else if (input_->PushKey(DIK_RIGHT)) {

		move.x += kCharacterSpeed;
	}
	if (input_->PushKey(DIK_UP)) {

		move.y += kCharacterSpeed;
	} else if (input_->PushKey(DIK_DOWN)) {

		move.y -= kCharacterSpeed;
	}

	const float kRotSpeed = 0.02f;

	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y += kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	}

	float playerPos[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};

	// 画面の座標を表示
	ImGui::Begin("Player");
	ImGui::SliderFloat3("playerPos", playerPos, -28.0f, 28.0f);
	ImGui::End();

	worldTransform_.translation_.x = playerPos[0];
	worldTransform_.translation_.y = playerPos[1];
	worldTransform_.translation_.z = playerPos[2];

	worldTransform_.translation_ = Add(worldTransform_.translation_, move);

	// 移動限界座標
	const float kMoveLimitX = 28;
	const float kMoveLimitY = 14;

	// 範囲を超えない処理
	worldTransform_.translation_.x = max(worldTransform_.translation_.x, -kMoveLimitX);
	worldTransform_.translation_.x = min(worldTransform_.translation_.x, +kMoveLimitX);
	worldTransform_.translation_.y = max(worldTransform_.translation_.y, -kMoveLimitY);
	worldTransform_.translation_.y = min(worldTransform_.translation_.y, +kMoveLimitY);

	worldTransform_.UpdateMatrix();

	// 攻撃処理
	Attack();
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}
}

void Player::Rotate() {
	const float kRotSpeed = 0.02f;
	if (input_->PushKey(DIK_A)) {
		worldTransform_.rotation_.y -= kRotSpeed;
	} else if (input_->PushKey(DIK_D)) {
		worldTransform_.rotation_.x += kRotSpeed;
	}
}

void Player::Attack() {
	if (input_->TriggerKey(DIK_SPACE)) {

		const float kBulletSpeed = 1.0f;
		Vector3 velocity(0, 0, kBulletSpeed);
		// 向き
		velocity = TransformNormal(velocity, worldTransform_.matWorld_);
		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		velocity.x = worldTransform3DReticle_.translation_.x - worldTransform_.translation_.x;
		velocity.y = worldTransform3DReticle_.translation_.y - worldTransform_.translation_.y;
		velocity.z = worldTransform3DReticle_.translation_.z - worldTransform_.translation_.z;
		velocity = Normalize(velocity);
		velocity.x *= kBulletSpeed;
		velocity.y *= kBulletSpeed;
		velocity.z *= kBulletSpeed;
		newBullet->Initialize(model_, GetWorldPosition(), velocity);
		// 弾を登録する
		// bullet_ = newBullet;
		bullets_.push_back(newBullet);
	}
}

Vector3 Player::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}


void Player::OnCollision() {}

// 親子関係を結ぶ
void Player::Setparent(const WorldTransform* parent) { worldTransform_.parent_ = parent; }

Player::~Player() 
{ delete sprite2DReticle_; }
