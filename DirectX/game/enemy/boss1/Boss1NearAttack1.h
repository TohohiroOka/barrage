#pragma once
#include "../game/enemy/BaseAction.h"

class Boss1NearAttack1 : public BaseAction
{
public:
	Boss1NearAttack1();
	~Boss1NearAttack1() {};

	void Update() override;

	void Draw() override;

	bool End() override;

private:
};

