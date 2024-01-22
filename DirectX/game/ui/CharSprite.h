#pragma once
#include "Object/2d/Sprite.h"

/// <summary>
/// 文字スプライト
/// </summary>
class CharSprite
{
public: //メンバ関数
	CharSprite(const std::wstring& character, const DirectX::XMFLOAT2& pos, float scale, bool isDraw = true);
	~CharSprite();

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 文字変更
	/// </summary>
	/// <param name="character">文字</param>
	void ChangeCharacter(const std::wstring& character);

	//getter
	Sprite* GetSprite() { return charSprite.get(); }
	bool GetIsDraw() { return isDraw; }
	const std::wstring& GetCharacter() { return character; }

	//setter
	void SetIsDraw(bool isDraw) { this->isDraw = isDraw; }

private: //メンバ関数
	/// <summary>
	/// 文字テキストテクスチャ読み込み
	/// </summary>
	/// <param name="character">文字</param>
	void LoadCharTexture(const std::string& character);

	/// <summary>
	/// wstring型の文字列をstring型に変換
	/// </summary>
	/// <param name="oWString">変換対象の文字列</param>
	/// <returns></returns>
	std::string WStringToString(const std::wstring& oWString);

public: //静的メンバ変数(取得しやすいようにpublic)
	//文字テクスチャの大きさ
	static const float charTextureSize;

protected: //メンバ変数
	//文字スプライト
	std::unique_ptr<Sprite> charSprite;
	//文字
	std::wstring character;
	//大きさ
	float scale;
	//描画するか
	bool isDraw = true;
};