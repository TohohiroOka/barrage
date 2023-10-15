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

	/// <summary>
	/// 光源視点での描画
	/// </summary>
	virtual void DrawLightView() = 0;

	/// <summary>
	/// 次の攻撃に遷移
	/// </summary>
	virtual bool NextAttack(int endurance) = 0;

	//getter
	int GetUseEndranceNum() { return useEnduranceNum; }
	AttackCollisionData GetAttackCollisionData() { return attackCollisionData; }
	bool GetisCollisionValid() { return isCollisionValid; }
	bool GetIsNextAttackInput() { return isNextAttackInput; }
	bool GetIsAttackActionEnd() { return isAttackActionEnd; }

protected: //メンバ変数
	//連続攻撃回数
	int attackNum = 0;
	//使用する持久力
	int useEnduranceNum;
	//コリジョンデータ
	AttackCollisionData attackCollisionData;
	//衝突判定が有効か
	bool isCollisionValid = false;
	//次の攻撃の入力を開始するか
	bool isNextAttackInput = false;
	//攻撃行動を終了するか
	bool isAttackActionEnd = false;
};