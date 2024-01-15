#pragma once
#include "CharSprite.h"
#include "system/GameInputManager.h"

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

private: //メンバ関数
	/// <summary>
	/// 行動入力コマンドを解析
	/// </summary>
	/// <param name="command">コマンド</param>
	/// <returns>行動入力</returns>
	GameInputManager::InputAction ActionInputCommand(const std::wstring& command);

private: //メンバ変数
	//文字スプライト
	std::vector<std::unique_ptr<CharSprite>> charSprites;
};