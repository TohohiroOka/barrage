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

	//�^�[�Q�b�g���W
	Vector3 targetPos;

	//�{�X�̃��f�����
	std::unique_ptr<BaseBossModel> bossModel;

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
	//�ړ��s�����s������
	bool isMove;

	//����p�̑傫��
	float hitScale;
};

