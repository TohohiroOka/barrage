#pragma once
#include "PlayerActionBase.h"
#include "BasePlayerAttack.h"
#include <memory>

/// <summary>
/// プレイヤー攻撃行動
/// </summary>
class PlayerActionAttack : public PlayerActionBase
{
public: //メンバ関数
	PlayerActionAttack(Player* player);
	~PlayerActionAttack();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private: //メンバ関数
	/// <summary>
	/// 攻撃中にの角度を変更
	/// </summary>
	void ChangeRotate();

	/// <summary>
	/// 次の攻撃を入力
	/// </summary>
	void NextAttack();
};