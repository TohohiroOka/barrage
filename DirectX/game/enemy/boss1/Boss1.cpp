#include "Boss1.h"
#include "GameHelper.h"

#include "Boss1NearAttack1.h"
#include "Boss1Move1.h"
#include "Boss1Bullet1.h"
#include "Boss1Bullet2.h"
#include "Boss1Bullet3.h"

const float partsOneDist = 7.0f;
const std::array<Vector3, Boss1::partsNum> Boss1::partsDist = {
	{{partsOneDist,partsOneDist,partsOneDist},{-partsOneDist,partsOneDist,partsOneDist},
	{partsOneDist,-partsOneDist,partsOneDist},{-partsOneDist,-partsOneDist,partsOneDist},
	{partsOneDist,partsOneDist,-partsOneDist},{-partsOneDist,partsOneDist,-partsOneDist},
	{partsOneDist,-partsOneDist,-partsOneDist},{-partsOneDist,-partsOneDist,-partsOneDist}}
};

Boss1::Boss1()
{
	maxHP = 1000;

	BaseBoss::Initialize();

	model = Model::CreateFromOBJ("NormalCube");

	for (int i=0;i< partsNum;i++) {
		boss[i] = Object3d::Create(model.get());
		boss[i]->SetParent(center.get());
		boss[i]->SetPosition(partsDist[i]);
		boss[i]->SetScale({ 5.0f,5.0f ,5.0f });
	}

	center->SetPosition({ 255.0f / 2.0f,10.0f ,255.0f / 2.0f });

	BaseAction::SetBossPtr(this);

	action = std::make_unique<Boss1Move1>(Boss1Move1({ 0.0f,0.0f,50.0f }));
	//action = std::make_unique<Boss1Bullet3>();
}

void Boss1::Update()
{
	//�A�N�V�������I��莟�掟�ɍs��
	if (action->End()) {
		SetAction();
	}

	action->Update();

	BaseBoss::Update();
}

void Boss1::Draw()
{
	action->Draw();

	for (auto& i : boss) {
		i->Draw();
	}

	hpGauge->Draw();
}

void Boss1::FrameReset()
{
	action->FrameReset();
}

void Boss1::SetAction()
{
	//������
	action->~BaseAction();
	action = nullptr;

	Vector3 pos = center->GetPosition();
	Vector3 dist = targetPos - pos;

	action = std::make_unique<Boss1Bullet3>();
	return;

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
				action = std::make_unique<Boss1Move1>(Boss1Move1({ 0.0f,0.0f,50.0f }));
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
				action = std::make_unique<Boss1Move1>(Boss1Move1({ 0.0f,0.0f,50.0f }));
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
			actionNumber = RandomInt(int(ShortAction::middle) + 1, int(ShortAction::size) - 1);
			if (actionNumber == int(ShortAction::attack1)) {
				action = std::make_unique<Boss1NearAttack1>();
			}
		}
		//�ړ��s��
		else {
			isMove = true;
			actionNumber = RandomInt(0, int(ShortAction::middle) - 1);
			if (actionNumber == int(ShortAction::move1)) {
				action = std::make_unique<Boss1Move1>(Boss1Move1({ 0.0f,0.0f,50.0f }));
			}
		}
	}
}
