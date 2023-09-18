#pragma once
#include "Input.h"
#include "Model.h"
#include "WorldTransform.h"

class Skydome {
public:
	void Initialize(Model* model, uint32_t textureHndle);

	void Update();

	void Draw(ViewProjection& viewProjection);

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// モデル
	Model* model_ = nullptr;
	uint32_t textureHandle_ = 0u;
	Input* input_ = nullptr;
};
