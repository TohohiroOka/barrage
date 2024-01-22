#pragma once
#include "CharSprite.h"

/// <summary>
/// 変動する数字テキスト
/// </summary>
class NumberText
{
public: //メンバ関数
	NumberText();
	~NumberText();

	/// <summary>
	/// 数字テキストとして使用する文字スプライトを追加
	/// </summary>
	/// <param name="charSprite">文字スプライト</param>
	void InsertCharSprite(CharSprite* charSprite);

	/// <summary>
	/// 文字スプライト追加を終了する
	/// </summary>
	void InsertEnd();

	/// <summary>
	/// 数字変更
	/// </summary>
	/// <param name="number"></param>
	void ChangeNumber(int number);

	//getter
	int GetNumber() { return number; }


private: //メンバ変数
	//数字テキストとして使用する文字スプライト群
	std::vector<CharSprite*> charSprites;
	//テキストとして表示する数字
	int number;
};