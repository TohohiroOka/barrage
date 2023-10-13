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

	//���S�_�i�`�斳���j
	std::unique_ptr<Object3d> center;

	//�ړ�����
	Vector3 moveVec;

	//�ő�̗�
	int maxHP;
	//�̗�
	int HP;
	//�̗̓Q�[�W
	std::unique_ptr<Gauge> hpGauge;

	//�s��
	std::unique_ptr<BaseAction> action;
	//�A�N�V������
	bool isAction;
	//�A�N�V�����̔ԍ�
	int actionNumber;
};

