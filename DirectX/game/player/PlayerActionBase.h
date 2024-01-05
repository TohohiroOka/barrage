#pragma once
#include "PlayerData.h"

/// <summary>
/// プレイヤークラスの前方宣言
/// </summary>
class Player;

/// <summary>
/// プレイヤーの行動基底クラス
/// </summary>
class PlayerActionBase
{
public:
	PlayerActionBase(Player* player);
	~PlayerActionBase();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	//getter
	bool GetIsActionEnd() { return isActionEnd; }

protected: //メンバ関数
	/// <summary>
	/// ジャンプできる状態か
	/// </summary>
	/// <returns></returns>
	bool JumpStart();

	/// <summary>
	/// 弱攻撃開始できる状態か
	/// </summary>
	/// <returns></returns>
	bool LightAttackStart();

	/// <summary>
	/// 強攻撃開始できる状態か
	/// </summary>
	/// <returns></returns>
	bool StrongAttackStart();

	/// <summary>
	/// 回避開始できる状態か
	/// </summary>
	/// <returns></returns>
	bool AvoidStart();

	/// <summary>
	/// ブリンク開始できる状態か
	/// </summary>
	/// <returns></returns>
	bool BlinkStart();

protected: //メンバ変数
	//行動させる対象のプレイヤー
	Player* player;
	//現在の行動を終了するか
	bool isActionEnd = false;
	//行動終了をした場合に次にどの行動をするか
	PlayerActionName nextAction;
};