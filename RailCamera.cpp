#include"RailCamera.h"
#include "ImGuiManager.h"
#include"Keisan.h"


Vector3 RailCamera::GetWorldTransform() {
	// ワールド座標をいれる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得
	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}


void RailCamera::Initialize(const Vector3& worldPos, const Vector3& rotation) {
	worldTransform_.translation_ = worldPos;
	worldTransform_.rotation_ = rotation;
	viewProjection_.Initialize();
}


void RailCamera::Update()
{

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCaracterSpeed = 0.3f;
	// キャラクターの回転速さ
	const float KRotspeed = 0.02f;

	

	// 座標移動(ベクトルの加算)
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	// 座標移動(ベクトルの加算)
	worldTransform_.rotation_ = Add(worldTransform_.rotation_, move);

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	

	// ImGuiで値を入力する変数
	float inputFloat3[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};

	float inputrotation_[3] = {
	    worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z};



	// キャラクターの座標を画面表示する処理
	ImGui::Begin("Camera");
	// float3入力ボックス
	ImGui::SliderFloat3("InputFloat3", inputFloat3,-30.0f,30.0f);
	// float3スライダー
	ImGui::SliderFloat3("SliderFloat3", inputrotation_, -30.0f, 30.0f);

	ImGui::End();
	worldTransform_.translation_.x = inputFloat3[0];
	worldTransform_.translation_.y = inputFloat3[1];
	worldTransform_.translation_.z = inputFloat3[2];

	worldTransform_.rotation_.x = inputrotation_[0];
	worldTransform_.rotation_.y = inputrotation_[1];
	worldTransform_.rotation_.z = inputrotation_[2];
}