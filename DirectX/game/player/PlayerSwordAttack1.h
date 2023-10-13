#pragma once
#include "game/player/BasePlayerAttack.h"
#include "engine/Object/3d/Base3D.h"
#include "Object/3d/Object3d.h"
#include <functional>

/// <summary>
/// プレイヤー剣攻撃1
/// </summary>
class PlayerSwordAttack1 : public BasePlayerAttack
{
public: //ステート
	enum State
	{
		NONATTACK,
		ATTACK,
	};

public: //メンバ関数
	PlayerSwordAttack1(Base3D* attacker);
	~PlayerSwordAttack1();

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 描画
	/// </summary>
	void Draw() override;

private: //メンバ関数
	void NonAttackAction();
	void AttackAction();

private: //メンバ変数
	//行動
	State state = State::NONATTACK;
	//行動用タイマー
	float timer = 0;
	//各行動の動き
	std::vector<std::function<void()>> func_;

	//攻撃判定可視化用オブジェクト
	std::unique_ptr<Model> model;
	std::unique_ptr<Object3d> object;
};