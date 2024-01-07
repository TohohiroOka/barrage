#pragma once
#include "CharSprite.h"

/// <summary>
/// テキスト制作システム
/// </summary>
class TextCreator
{
public: //メンバ関数
	TextCreator(const std::wstring& text, const DirectX::XMFLOAT2& leftTopPos, float scale, bool isStartDraw = true);
	~TextCreator();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//getter
	int GetTextLength() { return (int)charSprites.size(); }
	CharSprite* GetCharSprite(int num) { return charSprites[num].get(); }

private: //メンバ変数
	//文字スプライト
	std::vector<std::unique_ptr<CharSprite>> charSprites;
};