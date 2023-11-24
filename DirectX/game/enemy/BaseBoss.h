#pragma once
#include "../Object/3d/Object3d.h"
#include "BaseAction.h"
#include "BaseBossModel.h"
#include "Math/Vector3.h"
#include "ui/Gauge.h"

class BaseBoss
{
public:
	BaseBoss() {};
	virtual ~BaseBoss(){};

	virtual void Initialize();

	virtual void Update();

	virtual void Draw();

	virtual void DrawLightView();

	virtual void DrawSprite();

	virtual void FrameReset() { bossModel->RrameReset(); }

	virtual void SetAction() = 0;

	virtual void Damage(int damageNum);

	void Collider();

	Base3D* GetCenter() { return bossModel->GetObjectInst(); }
	float GetHitScale() { return hitScale; }
	void SetMoveVec(const Vector3& _moveVec) { moveVec = _moveVec; }
	BaseAction* GetBaseAction() { return action.get(); }
	void SetTargetPos(const Vector3& _targetPos) { targetPos = _targetPos; }
	Vector3 GetTargetPos() { return targetPos; }
	float GetLength() { return (targetPos - Vector3(bossModel->GetObjectInst()->GetPosition())).length(); }
	BaseBossModel* GetBaseModel() { return bossModel.get(); }
	bool GetBossIsAlive() { return HP <= 0; }

protected:

	//ターゲット座標
	Vector3 targetPos;

	//ボスのモデル情報
	std::unique_ptr<BaseBossModel> bossModel;

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
	//移動行動を行ったか
	bool isMove;

	//判定用の大きさ
	float hitScale;
};

