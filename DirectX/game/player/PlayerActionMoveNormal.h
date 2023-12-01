#pragma once
#include "PlayerActionBase.h"

/// <summary>
/// プレイヤー通常移動行動
/// </summary>
class PlayerActionMoveNormal : public PlayerActionBase
{
public: //メンバ関数
	PlayerActionMoveNormal(Player* player);
	~PlayerActionMoveNormal();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private: //メンバ関数
	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// ダッシュ
	/// </summary>
	void Dash();

	/// <summary>
	/// 次の行動を開始
	/// </summary>
	void NextActionStart();

public: //静的メンバ変数 (変更しやすいようにpublic)
	//最大移動スピード
	static float moveSpeedMax;
	//最大ダッシュスピード
	static float dashSpeedMax;

private: //メンバ変数
	//ダッシュしているか
	bool isDash = false;
	bool isDashStart = true;
};