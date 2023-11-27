#pragma once
#include "PlayerActionBase.h"

/// <summary>
/// プレイヤー死亡行動
/// </summary>
class PlayerActionDead : public PlayerActionBase
{
public: //メンバ関数
	PlayerActionDead(Player* player);
	~PlayerActionDead();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override {};
};