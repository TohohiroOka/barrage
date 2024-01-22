#pragma once
#include "ui/CharActionInputSprite.h"

/// <summary>
/// 選択ボタンが押せる状況を教えるUI(「A:入る」など)
/// </summary>
class PressSelectButtonUI
{
public: //メンバ関数
	PressSelectButtonUI();
	~PressSelectButtonUI();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

private: //静的メンバ変数
	//座標
	static const DirectX::XMFLOAT2 position;

private: //メンバ変数
	std::unique_ptr<CharActionInputSprite> buttonSprite;
	std::unique_ptr<Sprite> textSprite;
};