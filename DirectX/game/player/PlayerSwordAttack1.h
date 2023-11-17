#pragma once
#include "game/player/BasePlayerAttack.h"
#include "engine/Object/3d/Base3D.h"
#include "Object/3d/Object3d.h"
#include "Math/Timer.h"
#include <functional>

/// <summary>
/// プレイヤー剣攻撃1
/// </summary>
class PlayerSwordAttack1 : public BasePlayerAttack
{
public: //ステート
	enum State
	{
		ATTACK1,
		ATTACK2,
		ATTACK3,
		NONE,
	};

public: //メンバ関数
	PlayerSwordAttack1(std::function<DirectX::XMFLOAT3()> getSwordPos);
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
	/// 光源視点での描画
	/// </summary>
	void DrawLightView() override;

	/// <summary>
	/// 次の攻撃に遷移
	/// </summary>
	bool NextAttack(int endurance) override;

	/// <summary>
	/// 攻撃が当たった場合の処理
	/// </summary>
	void AttackCollision() override;

private: //メンバ関数
	void AttackAction1();
	void AttackAction2();
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
	std::unique_ptr<Engine::Timer> timer = 0;
	//各行動の動き
	std::vector<std::function<void()>> func_;

	//剣の座標取得用関数ポインタ
	std::function<DirectX::XMFLOAT3()> getSwordPos_;

	//連続攻撃回数
	int attackNum = 0;

	//攻撃判定可視化用オブジェクト
	std::unique_ptr<Model> model;
	std::unique_ptr<Object3d> object;
};