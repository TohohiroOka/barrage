#pragma once
#include "CharSprite.h"
#include "system/GameInputManager.h"

/// <summary>
/// 1文字として扱う行動入力スプライト
/// </summary>
class CharActionInputSprite : public CharSprite
{
public: //メンバ関数
	CharActionInputSprite(GameInputManager::InputAction inputAction, const DirectX::XMFLOAT2& pos, float scale, bool isDraw = true);
	~CharActionInputSprite();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private: //メンバ変数
	//行動名
	GameInputManager::InputAction inputAction;
	//大きさ
	float scale;
};