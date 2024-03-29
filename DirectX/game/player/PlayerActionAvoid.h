#pragma once
#include "PlayerActionBase.h"
#include "Math/Timer.h"

/// <summary>
/// プレイヤー回避行動
/// </summary>
class PlayerActionAvoid : public PlayerActionBase
{
public: //メンバ関数
	PlayerActionAvoid(Player* player);
	~PlayerActionAvoid();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private: //メンバ関数
	/// <summary>
	///	回避
	/// </summary>
	void Avoid();

private: //静的メンバ変数
	//回避にかかる時間
	static const int avoidTime = 36;
	//先行入力を開始する時間
	static const int actionChangeStartTime = 15;
	//回避開始時の移動スピード
	static const float avoidStartMoveSpeed;
	//回避終了時の移動スピード
	static const float avoidEndMoveSpeed;

private: //メンバ変数
	//回避用タイマー
	std::unique_ptr<Engine::Timer> avoidTimer;
};