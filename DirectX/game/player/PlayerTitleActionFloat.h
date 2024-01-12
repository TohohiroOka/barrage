#pragma once
#include "PlayerActionBase.h"
#include "Math/Vector3.h"
#include "Math/Timer.h"

/// <summary>
/// プレイヤータイトルシーン用浮遊行動
/// </summary>
class PlayerTitleActionFloat : public PlayerActionBase
{
public: //メンバ関数
	PlayerTitleActionFloat(Player* player, const Vector3& portalPos);
	~PlayerTitleActionFloat();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private: //メンバ関数
	/// <summary>
	/// 浮遊
	/// </summary>
	void Float();

private: //メンバ変数
	//イージング前座標
	Vector3 easeBeforePos;
	//イージング後座標
	Vector3 easeAfterPos;
	//イージング前回転角
	Vector3 easeBeforeRota;
	//イージング後回転角
	Vector3 easeAfterRota;
	//浮遊用タイマー
	std::unique_ptr<Engine::Timer> floatTimer;
};