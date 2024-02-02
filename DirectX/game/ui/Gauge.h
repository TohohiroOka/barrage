#pragma once
#include "engine/Object/2d/Sprite.h"

/// <summary>
/// ゲージ
/// </summary>
class Gauge
{
public: //メンバ変数
	Gauge(const DirectX::XMFLOAT2& position, const std::string& frameTexName, const DirectX::XMFLOAT2& barShiftNum, int maxGaugeNum, int gaugeNum, float decreaseGaugeLengthChangeSpeed, const DirectX::XMFLOAT4& color);
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
	/// <param name="newGaugeNum">新たに設定するゲージ量</param>
	/// <param name="isDecreaseDiffMode">ゲージの減少量を表示するか</param>
	void ChangeLength(int newGaugeNum, bool isDecreaseDiffMode);

	//getter
	bool GetIsDecreaseDiffMode() { return isDecreaseDiffMode; }

private: //メンバ関数	
	/// <summary>
	/// 減少量を短くしていく
	/// </summary>
	void ChangeDecreaseDiffLength();

private: //メンバ変数
	std::unique_ptr<Sprite> gaugeFrame;
	std::unique_ptr<Sprite> gaugeBar;
	std::unique_ptr<Sprite> gaugeDecreaseDiff;

	float length;
	float thickness;
	int maxGaugeNum;
	int gaugeNum;

	int decreaseStartLagTimer = 0;
	float decreaseLengthChangeSpeed = 0;
	bool isDecreaseDiffMode = false;
};