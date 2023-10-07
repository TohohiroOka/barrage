#pragma once
#include "../Object/3d/Object3d.h"
#include "BaseAction.h"
#include <functional>

class BaseBoss
{
public:

	virtual void Initialize();

	virtual void Update();

	virtual void Draw() = 0;

	virtual void SetAction() = 0;

	Object3d* GetCenter() { return center.get(); }

protected:

	//���S�_�i�`�斳���j
	std::unique_ptr<Object3d> center;

	//�s��
	std::unique_ptr<BaseAction> action;
	//�A�N�V������
	bool isAction;
	//�A�N�V�����̔ԍ�
	int actionNumber;
};

