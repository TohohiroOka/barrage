#pragma once
#include "../BaseBoss.h"
#include "Object/3d/Object3d.h"

class Boss1 : public BaseBoss
{
private:
	
	//遠距離行動
	enum class LongAction {
		move3,
		middle,//移動と攻撃の間
		bullet1,
		bullet2,
		size,
	};

	//中距離行動
	enum class MediumAction {
		move3,
		middle,//移動と攻撃の間
		bullet1,
		bullet2,
		bullet4,
		size,
	};

	//近距離行動
	enum class ShortAction {
		move1,
		middle,//移動と攻撃の間
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

	//breakを行った回数
	int breakNum;

	//breakするhp
	std::array<float, 2> breakHp;
};

