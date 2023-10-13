#pragma once
#include "../Object/3d/Object3d.h"
#include "BaseAction.h"
#include "Math/Vector3.h"
#include "ui/Gauge.h"

class BaseBoss
{
public:
	BaseBoss() {};
	virtual ~BaseBoss(){};

	virtual void Initialize();

	virtual void Update();

	virtual void Draw() = 0;

	virtual void SetAction() = 0;

	virtual void Damage(int damageNum);

	void Collider();

	Object3d* GetCenter() { return center.get(); }
	void SetMoveVec(const Vector3& _moveVec) { moveVec = _moveVec; }
	BaseAction* GetBaseAction() { return action.get(); }

protected:

	//中心点（描画無し）
	std::unique_ptr<Object3d> center;

	//移動距離
	Vector3 moveVec;

	//最大体力
	int maxHP;
	//体力
	int HP;
	//体力ゲージ
	std::unique_ptr<Gauge> hpGauge;

	//行動
	std::unique_ptr<BaseAction> action;
	//アクション中
	bool isAction;
	//アクションの番号
	int actionNumber;
};

