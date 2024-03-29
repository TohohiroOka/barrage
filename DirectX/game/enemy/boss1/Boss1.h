#pragma once
#include "../BaseBoss.h"
#include "Object/3d/Object3d.h"

class Boss1 : public BaseBoss
{
private:
	
	//ฃsฎ
	enum class LongAction {
		move3,
		middle,//ฺฎฦUฬิ
		bullet1,
		bullet2,
		size,
	};

	//ฃsฎ
	enum class MediumAction {
		move3,
		middle,//ฺฎฦUฬิ
		bullet1,
		bullet2,
		bullet4,
		size,
	};

	//฿ฃsฎ
	enum class ShortAction {
		move1,
		middle,//ฺฎฦUฬิ
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

	//break๐sมฝ๑
	int breakNum;

	//breakท้hp
	std::array<float, 2> breakHp;
};

