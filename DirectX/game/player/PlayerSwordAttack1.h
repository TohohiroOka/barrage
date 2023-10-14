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
		NONATTACK1,
		ATTACK1,
		NONATTACK2,
		ATTACK2,
		NONATTACK3,
		ATTACK3,
		NONE,
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

	/// <summary>
	/// 次の攻撃に遷移
	/// </summary>
	bool NextAttack(int endurance) override;

private: //メンバ関数
	void NonAttackAction1();
	void AttackAction1();
	void NonAttackAction2();
	void AttackAction2();
	void NonAttackAction3();
	void AttackAction3();

private: //静的メンバ変数
	//この攻撃の持久力使用料
	static const int attackUseEnduranceNum = 20;
	//最大連続攻撃回数
	static const int maxAttackNum = 3;
	//色
	static const DirectX::XMFLOAT4 attackColor;
	static const DirectX::XMFLOAT4 nonAttackColor;

private: //メンバ変数
	//行動
	State state = State::NONE;
	//行動用タイマー
	float timer = 0;
	//各行動の動き
	std::vector<std::function<void()>> func_;

	//連続攻撃回数
	int attackNum = 0;

	//攻撃判定可視化用オブジェクト
	std::unique_ptr<Model> model;
	std::unique_ptr<Object3d> object;

	//仮攻撃用イージング変数
	Vector3 easeBeforePos;
	Vector3 easeAfterPos;
};