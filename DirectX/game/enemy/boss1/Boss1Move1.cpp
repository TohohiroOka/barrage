#include "Boss1Move1.h"
#include "../boss1/Boss1Model.h"
#include "../BaseBoss.h"
#include "Math/Easing/Easing.h"
#include "GameHelper.h"

Boss1Move1::Boss1Move1()
{
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::back_end));
	boss->GetBaseModel()->AnimationReset();
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::back_start));
	boss->GetBaseModel()->AnimationReset();
	boss->GetBaseModel()->SetIsRoop(false);

	Vector3 target = boss->GetTargetPos();
	Vector3 bossPos = boss->GetCenter()->GetPosition();

	float angle = GetAngle({ target.x,target.z }, { bossPos.x,bossPos.z });
	boss->GetCenter()->SetRotation({ 0.0f,angle ,0.0f });

	moveV = { bossPos.x - target.x,0.0f,bossPos.x - target.x };
	moveV= moveV.normalize();

	moveTime = std::make_unique<Engine::Timer>();
	hitTimer = std::make_unique<Engine::Timer>();
}

void Boss1Move1::Update()
{
	if (boss->GetIsWince()) {
		return;
		isEnd = true;
	}

	const float maxTimer = 30.0f;
	moveTime->Update();

	float rate = *moveTime.get() / maxTimer;
	float i = 0;
	if (rate < 0.5) {
		i = Easing::InSine(0, 5, rate * 2.0f);
	} else {
		boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::back_end));
		i = Easing::OutSine(5, 0, (rate - 0.5f) * 2.0f);
	}

	boss->SetMoveVec(moveV * i);

	if (*moveTime.get() <= maxTimer) { return; }
	isEnd = true;
}