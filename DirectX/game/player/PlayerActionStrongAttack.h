#pragma once
#pragma once
#include "PlayerActionBaseAttack.h"

/// <summary>
/// プレイヤー強攻撃行動
/// </summary>
class PlayerActionStrongAttack : public PlayerActionBaseAttack
{
public: //メンバ関数
	PlayerActionStrongAttack(Player* player);
	~PlayerActionStrongAttack();

private: //メンバ関数
	/// <summary>
	/// 次の攻撃を入力
	/// </summary>
	void NextAttack() override;
};