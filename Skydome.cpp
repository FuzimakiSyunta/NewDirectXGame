#include "Skydome.h"
#include <cassert>
#define _USE_MATH_DEFINES
#include "ImGuiManager.h"
#include "keisan.h"
#include <math.h>

void Skydome::Initialize(Model* model, uint32_t textureHndle) {
	assert(model);
	model_ = model;
	worldTransform_.Initialize();
	input_ = Input::GetInstance();
	worldTransform_.scale_ = {10.0f, 10.0f, 10.0f};
	worldTransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	worldTransform_.translation_ = {0.0f, 0.0f, 0.0f};
}

void Skydome::Update() { worldTransform_.UpdateMatrix(); }

void Skydome::Draw(ViewProjection& viewProjection) {
	model_->Draw(worldTransform_, viewProjection);
}