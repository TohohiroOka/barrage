#include "Boss1Wince.h"
#include "../boss1/Boss1Model.h"
#include "../BaseBoss.h"
#include "GameHelper.h"
#include "Math/Easing/Easing.h"

Boss1Wince::Boss1Wince()
{
	//アニメーションセット
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::fear));
	boss->GetBaseModel()->AnimationReset();

	timer = std::make_unique<Engine::Timer>();

	hitTimer = std::make_unique<Engine::Timer>();

	Vector3 pos = boss->GetBaseModel()->GetPosition();
	boss->GetBaseModel()->SetPosition({ pos.x,10.0f,pos.z });
}

void Boss1Wince::Update()
{
	if (!boss->GetBaseModel()->GetIsAnimationEnd()) { return; }
	isEnd = true;
}
