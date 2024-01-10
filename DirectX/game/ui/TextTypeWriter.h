#pragma once
#include "TextCreator.h"
#include "Math/Timer.h"

/// <summary>
/// タイプライター(1文字ずつ描画するやつ)
/// </summary>
class TextTypeWriter
{
public: //メンバ関数
	TextTypeWriter(const std::wstring& text, const DirectX::XMFLOAT2& leftTopPos, float scale, int writeWaitFrame = 1);
	~TextTypeWriter();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//getter
	bool GetIsAllWrite() { return text->GetCharSprite(text->GetTextLength() - 1)->GetIsDraw(); }

private: //メンバ関数
	/// <summary>
	/// 表示管理
	/// </summary>
	void Write();

	/// <summary>
	/// 一気に全文字表示
	/// </summary>
	void AllWrite();

private: //静的メンバ変数
	//「、」の待機フレーム数
	static const int commaWaitFrame = 10;
	//「。」の待機フレーム数
	static const int periodWaitFrame = 20;

private: //メンバ変数
	//文章
	std::unique_ptr<TextCreator> text;
	//表示用用タイマー
	std::unique_ptr<Engine::Timer> timer;
	//1文字表示に待機するフレーム数
	int writeWaitFrame = 0;
	//通常文字表示に待機するフレーム数
	int normalWriteWaitFrame = 0;
	//表示文字数カウント
	int writeCount = 0;
};