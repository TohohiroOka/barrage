#pragma once
#include "PlayerActionBase.h"
#include "Math/Vector3.h"

/// <summary>
/// プレイヤーノックバック行動
/// </summary>
class PlayerActionKnockback : public PlayerActionBase
{
public: //メンバ関数
	PlayerActionKnockback(Player* player, const Vector3& knockbackVec, int knockbackTime, int knockbackPower);
	~PlayerActionKnockback();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private: //メンバ関数
	/// <summary>
	/// ノックバック
	/// </summary>
	void Knockback();

private: //メンバ変数
	//ノックバックベクトル
	Vector3 knockbackVec;
	//ノックバックにかかる時間
	int knockbackTime;
	//ノックバックタイマー
	int knockbackTimer = 0;
	//ノックバックの強さ
	float knockbackPower;
};