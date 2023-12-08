#pragma once
#include "PlayerActionBase.h"
#include "BasePlayerAttack.h"
#include <memory>

/// <summary>
/// プレイヤー攻撃行動基底クラス
/// </summary>
class PlayerActionBaseAttack : public PlayerActionBase
{
public: //メンバ関数
	PlayerActionBaseAttack(Player* player);
	~PlayerActionBaseAttack();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

protected: //メンバ関数
	/// <summary>
	/// 次の攻撃を入力
	/// </summary>
	virtual void NextAttack() = 0;
};