#pragma once
#include "PlayerActionBase.h"
#include "Math/Timer.h"

/// <summary>
/// プレイヤーブリンク行動
/// </summary>
class PlayerActionBlink : public PlayerActionBase
{
public: //メンバ関数
	PlayerActionBlink(Player* player);
	~PlayerActionBlink();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private: //メンバ関数
	/// <summary>
	///	回避
	/// </summary>
	void Blink();

private: //静的メンバ変数
	//ブリンクにかかる時間
	static const int blinkTime = 20;
	//先行入力を開始する時間
	static const int actionChangeStartTime = 15;

private: //メンバ変数
	//ブリンク用タイマー
	std::unique_ptr<Engine::Timer> blinkTimer;
	//ブリンク開始可能か
	bool isBlinkStart = true;
};