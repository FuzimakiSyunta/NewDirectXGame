#pragma once
#include "Input.h"
#include "Model.h"
#include "ViewProjection.h"
#include "WorldTransform.h"

class RailCmamera {
public:
	void Initialize(const Vector3& worldPos, const Vector3& position);
	void Update();

	ViewProjection& GetViewProjection() { return viewProjection_; }
	WorldTransform& GetWorldTransform() { return worldTransform_; };

private:
	WorldTransform worldTransform_;
	ViewProjection viewProjection_;
};