#pragma once
class BaseBoss;

class BaseAction
{
public:
	virtual ~BaseAction(){};

	virtual void Update() = 0;

	virtual void Draw() = 0;

	bool End() { return isEnd; }

	static void SetBossPtr(BaseBoss* _boss) { boss = _boss; }

protected:

	//�{�X�{�̃N���X
	static BaseBoss* boss;
	//�s���I��
	bool isEnd = false;
};

