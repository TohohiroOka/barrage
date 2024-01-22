#pragma once
#include "Object/2d/Sprite.h"
#include "GameHelper.h"
#include "Math/Timer.h"

/// <summary>
/// OKと表示するスプライト
/// </summary>
class OKSprite
{
public: //メンバ関数
	OKSprite();
	~OKSprite();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 描画開始
	/// </summary>
	void DrawStart();

	//getter
	bool GetIsDraw() { return isDraw; }
	bool GetIsScaleChange() { return isScaleChange; }

	//setter
	void SetIsDraw(bool isDraw) { this->isDraw = isDraw; }


private: //メンバ関数
	/// <summary>
	/// スケール変更演出
	/// </summary>
	void ScaleChange();

private: //メンバ変数
	//OKスプライト
	std::unique_ptr<Sprite> okSprite;
	//スプライトのスケール比
	float spriteScale;
	//スケール変更演出中か
	bool isScaleChange = false;
	//タイマー
	std::unique_ptr<Engine::Timer> scaleChangeTimer;
	//描画するか
	bool isDraw = false;
};