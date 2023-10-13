#pragma once
#include <vector>
#include "Math/Vector3.h"

class BaseBoss;

class BaseAction
{
public:
	struct AttackCollision {
		Vector3 pos;
		float radius;
	};

	virtual ~BaseAction(){};

	virtual void Update() = 0;

	virtual void Draw() = 0;

	bool End() { return isEnd; }

	static void SetBossPtr(BaseBoss* _boss) { boss = _boss; }

	virtual void GetAttackCollision(std::vector<BaseAction::AttackCollision>& _info) = 0;

protected:

	//ボス本体クラス
	static BaseBoss* boss;
	//行動終了
	bool isEnd = false;
};

