#pragma once
class BaseBoss;

class BaseAction
{
public:
	BaseAction(){};
	~BaseAction(){};

	virtual void Update() = 0;

	virtual void Draw() = 0;

	virtual bool End() = 0;

	static void SetBossPtr(BaseBoss* _boss) { boss = _boss; }

protected:

	static BaseBoss* boss;
};

