#pragma once
#include "../BaseBoss.h"
#include "Object/3d/Object3d.h"

class Boss1 : public BaseBoss
{
private:
	
	//�������s��
	enum class LongAction {
		move3,
		middle,//�ړ��ƍU���̊�
		bullet1,
		bullet2,
		size,
	};

	//�������s��
	enum class MediumAction {
		move3,
		middle,//�ړ��ƍU���̊�
		bullet1,
		bullet2,
		bullet4,
		size,
	};

	//�ߋ����s��
	enum class ShortAction {
		move1,
		middle,//�ړ��ƍU���̊�
		attack1,
		bullet2,
		bullet3,
		others,
		move2,
		size,
	};

public:
	Boss1();
	~Boss1(){};

	void Update() override;

	void FrameReset() override;

	void SetAction() override;

	void SetShortMoveAction();

private:

	//break���s������
	int breakNum;

	//break����hp
	std::array<float, 2> breakHp;
};

