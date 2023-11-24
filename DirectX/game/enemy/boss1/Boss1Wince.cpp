#include "Boss1Wince.h"
#include "../boss1/Boss1Model.h"
#include "../BaseBoss.h"
#include "GameHelper.h"
#include "Math/Easing/Easing.h"

Boss1Wince::Boss1Wince()
{
	//アニメーションセット
	boss->GetBaseModel()->AnimationReset();
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::standBy));

	timer = std::make_unique<Engine::Timer>();

	hitTimer = std::make_unique<Engine::Timer>();
}

void Boss1Wince::Update()
{
	timer->Update();
	if (*timer.get() > 20) {
		isEnd = true;
	}
}
