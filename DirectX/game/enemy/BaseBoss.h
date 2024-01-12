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

	virtual void FrameReset() {
		bossModel->RrameReset();
		if (breakAction) {
			breakAction->FrameReset();
		}
	}

	virtual void SetAction() = 0;

	virtual void Damage(int damageNum);

	void Collider();

	void SetPlayerDirection();

	Base3D* GetCenter() { return bossModel->GetObjectInst(); }
	float GetHitScale() { return hitScale; }
	BaseAction* GetBaseAction() { return action.get(); }
	Vector3 GetTargetPos() { return targetPos; }
	float GetLength() { return (targetPos - Vector3(bossModel->GetPosition())).length(); }
	BaseBossModel* GetBaseModel() { return bossModel.get(); }
	bool GetBossIsAlive() { return HP <= 0; }
	bool GetIsWince() { return isWince; }
	bool GetIsBreak() { return isBreak; }
	void SetTargetPos(const Vector3& _targetPos) { targetPos = _targetPos; }
	BaseAction* GetBaseAction2() { 
		if (breakAction) {
			return breakAction.get();
		} else {
			return nullptr;
		}
	}


protected:

	//�^�[�Q�b�g���W
	Vector3 targetPos;

	//�{�X�̃��f�����
	std::unique_ptr<BaseBossModel> bossModel;

	//�ő�̗�
	int maxHP;
	//�̗�
	int HP;
	//�̗̓Q�[�W
	std::unique_ptr<Gauge> hpGauge;

	//�s�����f�t���O
	bool isBreak;

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

	//���ݒl
	float winceValue;
	//����
	bool isWince;
	//���݃A�N�V����
	std::unique_ptr<BaseAction> winceAction;

	//hp����艻�ɂȂ������̍s��
	std::unique_ptr<BaseAction> breakAction;
};

