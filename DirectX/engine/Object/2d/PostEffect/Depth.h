#pragma once
#include "Object/2d/Sprite.h"

class Depth : public Sprite
{
public: //静的メンバ関数
	/// <summary>
	/// シャドウマップ生成
	/// </summary>
	/// <returns>シャドウマップ</returns>
	static std::unique_ptr<Depth> Create(const std::string& _texName);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize(const std::string& _texName);

	/// <summary>
	/// シーン描画前処理
	/// </summary>
	void PreDrawScene();

	/// <summary>
	/// シーン描画後処理
	/// </summary>
	void PostDrawScene();
};

