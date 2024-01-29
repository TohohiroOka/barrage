#pragma once
#include "Object/2d/Sprite.h"

/// <summary>
/// チュートリアル用ポーズ画面
/// </summary>
class TutorialPause
{
public: //メンバ関数
	TutorialPause();
	~TutorialPause();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ポーズ画面開始
	/// </summary>
	void PauseStart();

	/// <summary>
	/// ポーズ画面終了
	/// </summary>
	void PauseEnd();

	//getter
	bool GetIsPauseDraw() { return isPauseDraw; }


private: //メンバ変数
	//仮置き背景用黒スプライト
	std::unique_ptr<Sprite> backSprite;
	//ポーズ画面表示中か
	bool isPauseDraw = false;
};