#pragma once
#include "Math/Vector3.h"

/// <summary>
/// プレイヤークラスの前方宣言
/// </summary>
class Player;

/// <summary>
/// プレイヤー攻撃基底クラス
/// </summary>
class BasePlayerAttack
{
public:
	struct AttackCollisionData {
		Vector3 startPosition;
		Vector3 endPosition;
		float radius;
		int power;
	};

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 描画
	/// </summary>
	virtual void Draw() = 0;

	/// <summary>
	/// 次の攻撃に遷移
	/// </summary>
	virtual bool NextAttack() = 0;

	/// <summary>
	/// 攻撃が当たった場合の処理
	/// </summary>
	virtual void AttackCollision() = 0;

	//getter
	int GetUseEndranceNum() { return useEnduranceNum; }
	AttackCollisionData GetAttackCollisionData() { return attackCollisionData; }
	bool GetIsCollisionValid() { return isCollisionValid; }
	bool GetIsNextActionInput() { return isNextActionInput; }
	bool GetIsAttackActionEnd() { return isAttackActionEnd; }

	//setter
	void SetIsCollisionValid(bool isCollisionValid) { this->isCollisionValid = isCollisionValid; }

protected: //メンバ変数
	//プレイヤー
	Player* player;
	//連続攻撃回数
	int attackNum = 0;
	//使用する持久力
	int useEnduranceNum;
	//コリジョンデータ
	AttackCollisionData attackCollisionData;
	//衝突判定が有効か
	bool isCollisionValid = false;
	//次の行動の入力を開始するか
	bool isNextActionInput = false;
	//攻撃行動を終了するか
	bool isAttackActionEnd = false;
};