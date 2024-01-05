#pragma once
#include "PlayerActionBase.h"

/// <summary>
/// プレイヤージャンプ行動
/// </summary>
class PlayerActionJump : public PlayerActionBase
{
public: //メンバ関数
	PlayerActionJump(Player* player);
	~PlayerActionJump();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private: //静的メンバ変数
	//ジャンプ力
	static float jumpPower;
};