#pragma once
#include "Object/3d/Fbx.h"
#include "effect/SlashEffect.h"

/// <summary>
/// プレイヤー用剣
/// </summary>
class PlayerSword
{
public: //メンバ関数
	PlayerSword(Fbx* parent);
	~PlayerSword();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// エフェクト描画
	/// </summary>
	void DrawEffect();

	//getter
	SlashEffect* GetSlashEffect() { return slashEffect.get(); }

private: //メンバ変数
	//剣を所持する親オブジェクト
	Fbx* parent = nullptr;
	//剣のモデル
	std::unique_ptr<Model> swordModel;
	//斬撃演出
	std::unique_ptr<SlashEffect> slashEffect;
};