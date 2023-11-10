#include "Boss1.h"
#include "GameHelper.h"

#include "Boss1Model.h"
#include "Boss1NearAttack1.h"
#include "Boss1Move1.h"
#include "Boss1Move2.h"
#include "Boss1Bullet1.h"
#include "Boss1Bullet2.h"
#include "Boss1Bullet3.h"

Boss1::Boss1()
{
	maxHP = 1000;

	BaseBoss::Initialize();

	bossModel = std::make_unique<Boss1Model>();
	//action = std::make_unique<Boss1Bullet3>();

	BaseAction::SetBossPtr(this);

	action = std::make_unique<Boss1Move2>();
}

void Boss1::Update()
{
	//�A�N�V�������I��莟�掟�ɍs��
	if (action->End()) {
		SetAction();
	}

	BaseBoss::Update();
}

void Boss1::FrameReset()
{
	action->FrameReset();
	BaseBoss::FrameReset();
}

void Boss1::SetAction()
{
	//������
	action->~BaseAction();
	action = nullptr;

	Vector3 pos = bossModel->GetObjectInst()->GetPosition();
	Vector3 dist = targetPos - pos;

	//action = std::make_unique<Boss1Bullet3>();
	//return;

	//�v���C���[���������ɂ���
	if (dist.length() > 300.0f) {
		bool actionRand = RandomInt(1);
		//�O��ړ��Ȃ�K���U�����s��
		//�U���s��
		if (isMove|| actionRand) {
			isMove = false;
			actionNumber = RandomInt(int(LongAction::middle) + 1, int(LongAction::size) - 1);
			if (actionNumber == int(LongAction::bullet1)) {
				action = std::make_unique<Boss1Bullet1>();
			}else if (actionNumber == int(LongAction::bullet2)) {
				action = std::make_unique<Boss1Bullet2>();
			}
		}
		//�ړ��s��
		else {
			isMove = true;
			actionNumber = RandomInt(0, int(LongAction::middle) - 1);
			if (actionNumber == int(LongAction::move1)) {
				action = std::make_unique<Boss1Move1>();
			}
		}
	}
	//�v���C���[���������ɂ���
	else if (dist.length() > 100.0f) {
		bool actionRand = RandomInt(1);
		//�O��ړ��Ȃ�K���U�����s��
		//�U���s��
		if (isMove || actionRand) {
			isMove = false;
			actionNumber = RandomInt(int(MediumAction::middle) + 1, int(MediumAction::size) - 1);
			if (actionNumber == int(MediumAction::bullet3)) {
				action = std::make_unique<Boss1Bullet3>();
			}
		}
		//�ړ��s��
		else {
			isMove = true;
			actionNumber =RandomInt(0, int(MediumAction::middle) - 1);
			if (actionNumber == int(MediumAction::move1)) {
				action = std::make_unique<Boss1Move1>();
			}
		}
	}
	//�v���C���[���ߋ����ɂ���
	else {
		bool actionRand = RandomInt(1);
		//�O��ړ��Ȃ�K���U�����s��
		//�U���s��
		if (isMove || actionRand) {
			isMove = false;
			actionNumber = RandomInt(int(ShortAction::middle) + 1, int(ShortAction::others) - 1);
			if (actionNumber == int(ShortAction::attack1)) {
				action = std::make_unique<Boss1NearAttack1>();
			}
		}
		//�ړ��s��
		else {
			SetShortMoveAction();
		}
	}
}

void Boss1::SetShortMoveAction()
{
	isMove = true;

	//�ǂƂ̋������߂������ꍇ�̓���
	const float l_size = GameHelper::Instance()->GetStageSize();
	const float l_specifiedValue = 20.0f;
	if (targetPos.x >= l_size - l_specifiedValue || targetPos.z >= l_size - l_specifiedValue ||
		targetPos.x <= l_specifiedValue || targetPos.z <= l_specifiedValue) {
		action = std::make_unique<Boss1Move2>();
		return;
	}

	//���̑��̈ړ�
	actionNumber = RandomInt(0, int(ShortAction::middle) - 1);
	if (actionNumber == int(ShortAction::move1)) {
		action = std::make_unique<Boss1Move1>();
	}

}
