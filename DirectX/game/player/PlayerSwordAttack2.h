#pragma once
#include "game/player/BasePlayerAttack.h"
#include "engine/Object/3d/Base3D.h"
#include "Object/3d/Object3d.h"
#include "Math/Timer.h"
#include <functional>

/// <summary>
/// プレイヤー剣攻撃2
/// </summary>
class PlayerSwordAttack2 : public BasePlayerAttack
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
	PlayerSwordAttack2(Player* player);
	~PlayerSwordAttack2();

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
	bool NextAttack() override;

	/// <summary>
	/// 攻撃が当たった場合の処理
	/// </summary>
	void AttackCollision() override;

private: //メンバ関数
	//攻撃挙動
	void AttackAction1();
	void AttackAction2();
	void AttackAction3();

	/// <summary>
	/// プレイヤーを攻撃に合わせて動かす
	/// </summary>
	void MovePlayer(int moveTime);

	/// <summary>
	/// プレイヤーを回転入力で回す
	/// </summary>
	void PlayerChangeRotate();

public: //静的メンバ変数 (変更しやすいようにpublic)
	//最大連続攻撃回数
	static const int maxAttackNum = 3;
	//この攻撃の持久力使用料
	static int attackUseEnduranceNum;
	//この攻撃の攻撃力
	static int attackPower;

	//1回目の攻撃
	//攻撃にかかる時間
	static const int attackTime1 =55;
	//先行入力を開始する時間
	static const int actionChangeStartTime1 = 45;
	//衝突判定が有効に切り替わる時間
	static const int collisionValidStartTime1 = 40;

	//2回目の攻撃
	//攻撃にかかる時間
	static const int attackTime2 = 55;
	//先行入力を開始する時間
	static const int actionChangeStartTime2 = 45;
	//衝突判定が有効に切り替わる時間
	static const int collisionValidStartTime2 = 35;

	//3回目の攻撃
	//攻撃にかかる時間
	static const int attackTime3 = 55;
	//先行入力を開始する時間
	static const int actionChangeStartTime3 = 45;
	//衝突判定が有効に切り替わる時間
	static const int collisionValidStartTime3 = 40;

	//色
	static const DirectX::XMFLOAT4 attackColor;
	static const DirectX::XMFLOAT4 nonAttackColor;
	//攻撃開始時の移動スピード最高値
	static const float attackStartMoveSpeedMax;
	//攻撃開始時の移動スピード最低値
	static const float attackStartMoveSpeedMin;

private: //メンバ変数
	//行動
	State state = State::NONE;
	//行動用タイマー
	std::unique_ptr<Engine::Timer> timer = 0;
	//各行動の動き
	std::vector<std::function<void()>> func_;

	//連続攻撃回数
	int attackNum = 0;

	//攻撃がヒットしたか
	bool isHitAttack = false;

	//攻撃開始時の移動スピード
	float attackStartMoveSpeed;

	//攻撃判定可視化用オブジェクト
	std::unique_ptr<Model> model;
	std::unique_ptr<Object3d> object;
};