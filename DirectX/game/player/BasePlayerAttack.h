#pragma once
#include "Math/Vector3.h"

/// <summary>
/// プレイヤー攻撃基底クラス
/// </summary>
class BasePlayerAttack
{
public:
	struct AttackCollisionData {
		DirectX::XMVECTOR center;
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

	//getter
	AttackCollisionData GetAttackCollisionData() { return attackCollisionData; }
	bool GetisCollisionValid() { return isCollisionValid; }
	bool GetIsNextAttackInput() { return isNextAttackInput; }
	bool GetIsAttackActionEnd() { return isAttackActionEnd; }

protected: //メンバ変数
	//コリジョンデータ
	AttackCollisionData attackCollisionData;
	//衝突判定が有効か
	bool isCollisionValid = false;
	//次の攻撃の入力を開始するか
	bool isNextAttackInput = false;
	//攻撃行動を終了するか
	bool isAttackActionEnd = false;
};