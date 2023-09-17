#include"Skydome.h"
#include <cassert>


void Skydome::Initialize(Model* model, uint32_t textureHandle) { 
	assert(model);
	model_ = model;
	textureHandle_ = textureHandle;

	worldtransform_.Initialize();
	// x,y,z方向のスケーリングを設定
	worldtransform_.scale_ = {1.0f, 1.0f, 1.0f};
	// x,y,z方向の回転を設定
	worldtransform_.rotation_ = {0.0f, 0.0f, 0.0f};
	// x,y,z方向
	worldtransform_.translation_ = {0.0f, 0.0f, 0.0f};
}

void Skydome::Update()
{


}

void Skydome::Draw(ViewProjection& viewProjection_) 
{
	model_->Draw(worldtransform_, viewProjection_);
}
