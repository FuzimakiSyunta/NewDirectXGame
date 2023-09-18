#include "RailCamera.h"
#include "ImGuiManager.h"
#include "keisan.h"

void RailCamera::Initialize(Vector3& worldPos, Vector3& rotate) {
	// ワールドトランスフォームの初期設定
	worldTransform_.translation_ = worldPos;
	worldTransform_.rotation_.x = rotate.x;
	worldTransform_.rotation_.y = rotate.y;
	worldTransform_.rotation_.z = rotate.z;
	// ビュープロジェクションの初期化
	viewProjection_.Initialize();
}

void RailCamera::Update() {

	// 移動
	Vector3 move = {0, 0, 0};
	worldTransform_.translation_ = Add(worldTransform_.translation_, move);
	// 回転
	const float kRotSpeed = 0.00f;
	worldTransform_.rotation_.y -= kRotSpeed;

	worldTransform_.matWorld_ = MakeAffineMatrix(
	    worldTransform_.scale_, worldTransform_.rotation_, worldTransform_.translation_);

	// カメラオブジェクトのワールド行列からビュー行列を計算する
	viewProjection_.matView = Inverse(worldTransform_.matWorld_);

	float translate[3] = {
	    worldTransform_.translation_.x, worldTransform_.translation_.y,
	    worldTransform_.translation_.z};
	float rotate[3] = {
	    worldTransform_.rotation_.x, worldTransform_.rotation_.y, worldTransform_.rotation_.z};

	// カメラの座標を画面表示する処理
	ImGui::Begin("Camera");
	ImGui::SliderFloat3("CameraTranslation", translate, 600.0f, -600.0f);
	ImGui::SliderFloat3("CameraTranslation", rotate, 600.0f, -600.0f);
	ImGui::End();
}

Vector3 RailCamera::GetWorldPosition() {
	Vector3 worldPos;

	worldPos.x = worldTransform_.matWorld_.m[3][0];
	worldPos.y = worldTransform_.matWorld_.m[3][1];
	worldPos.z = worldTransform_.matWorld_.m[3][2];

	return worldPos;
}

// Vector3 RailCamera::GetViewProjection() {
//	Vector3 viewProjection;
//
//	viewProjection.x = viewProjection_.matView.m[][]
//
// }