#pragma once
#include "PlayerActionBaseAttack.h"

/// <summary>
/// プレイヤー弱攻撃行動
/// </summary>
class PlayerActionLightAttack : public PlayerActionBaseAttack
{
public: //メンバ関数
	PlayerActionLightAttack(Player* player);
	~PlayerActionLightAttack();

private: //メンバ関数
	/// <summary>
	/// 次の攻撃を入力
	/// </summary>
	void NextAttack() override;
};