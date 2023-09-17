#include "WorldTransform.h"
#include "ViewProjection.h"
#include"Input.h"

///レールカメラ
class RailCamera {
public:
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const Vector3& worldPos, const Vector3& rotation);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	const ViewProjection& GetViewProjection() { return viewProjection_; }

	// ワールド座標を取得
	Vector3 GetWorldTransform();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	// ビュープロジェクション
	ViewProjection viewProjection_;
	// キーボード入力
	Input* input_ = nullptr;

};