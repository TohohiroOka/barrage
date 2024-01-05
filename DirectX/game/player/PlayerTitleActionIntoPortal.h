#pragma once
#include "PlayerActionBase.h"
#include "Math/Vector3.h"
#include "Math/Timer.h"

/// <summary>
/// プレイヤータイトルシーン用ポータルに入る行動
/// </summary>
class PlayerTitleActionIntoPortal : public PlayerActionBase
{
public: //メンバ関数
	PlayerTitleActionIntoPortal(Player* player, const Vector3& portalPos);
	~PlayerTitleActionIntoPortal();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private: //メンバ関数
	/// <summary>
	/// ポータルに入る
	/// </summary>
	void IntoPortal();

private: //メンバ変数
	//イージング前座標
	Vector3 easeBeforePos;
	//イージング後座標
	Vector3 easeAfterPos;
	//イージング前回転角
	Vector3 easeBeforeRota;
	//イージング後回転角
	Vector3 easeAfterRota;
	//イージング前大きさ
	DirectX::XMFLOAT3 easeBeforeScale;
	//イージング後大きさ
	DirectX::XMFLOAT3 easeAfterScale;
	//ポータルに入る用タイマー
	std::unique_ptr<Engine::Timer> intoPortalTimer;
	//入ったか
	bool isInPortal = false;
};