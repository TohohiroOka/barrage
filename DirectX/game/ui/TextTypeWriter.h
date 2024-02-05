#pragma once
#include "TextCreator.h"
#include "Math/Timer.h"

/// <summary>
/// タイプライター(1文字ずつ描画するやつ)
/// </summary>
class TextTypeWriter : public TextCreator
{
public: //メンバ関数
	TextTypeWriter(const std::wstring& text, const DirectX::XMFLOAT2& leftTopPos, float scale, int writeWaitFrame = 1);
	~TextTypeWriter();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private: //メンバ関数
	/// <summary>
	/// 表示管理
	/// </summary>
	void Write();

	/// <summary>
	/// カタカタ音管理
	/// </summary>
	void TextSound();

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
	//表示用用タイマー
	std::unique_ptr<Engine::Timer> timer;
	//1文字表示に待機するフレーム数
	int writeWaitFrame = 0;
	//通常文字表示に待機するフレーム数
	int normalWriteWaitFrame = 0;
	//表示文字数カウント
	int writeCount = 0;
	//テキスト更新と同じ入力を使用している場合、トリガー判定が通ってしまい文章が全表示されてしまうので生成した瞬間かのフラグを設置
	bool isCreateFrame = true;

	//サウンド番号
	int soundNumber = 0;
};