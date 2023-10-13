#pragma once
#include "engine/Object/2d/Sprite.h"

/// <summary>
/// ゲージ
/// </summary>
class Gauge
{
public: //メンバ変数
	Gauge(const DirectX::XMFLOAT2& position, float length, int maxGaugeNum, int gaugeNum, const DirectX::XMFLOAT4& color);
	~Gauge();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ゲージの長さ変更
	/// </summary>
	/// <param name="decreaseNum">減少量</param>
	/// <param name="isDecreaseDiffMode">ゲージの減少量を表示するか</param>
	void ChangeLength(int newGaugeNum, bool isDecreaseDiffMode);

	bool GetIsDecreaseDiffMode() { return isDecreaseDiffMode; }

private: //メンバ関数	
	/// <summary>
	/// 減少量を短くしていく
	/// </summary>
	void ChangeDecreaseDiffLength();

private: //静的メンバ変数
	//太さ
	static const float thickness;

private: //メンバ変数
	std::unique_ptr<Sprite> gaugeFrame;
	std::unique_ptr<Sprite> gaugeBar;
	std::unique_ptr<Sprite> gaugeDecreaseDiff;

	float maxLength;
	float length;
	int maxGaugeNum;
	int gaugeNum;

	int decreaseStartLagTimer = 0;
	bool isDecreaseDiffMode = false;
};