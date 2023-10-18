#pragma once
#include "../BaseBoss.h"
#include "Object/3d/Object3d.h"

class Boss1 : public BaseBoss
{
private:

	enum class Action {
		move1,
		nearAttack,
		bullet1,
		size,
	};

public:
	Boss1();
	~Boss1(){};

	void Update() override;

	void Draw() override;

	void FrameReset() override;

	void SetAction() override;

private:
	std::unique_ptr<Model> model;
	std::unique_ptr<Object3d> boss;

};

