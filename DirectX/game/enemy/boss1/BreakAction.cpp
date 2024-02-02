#include "BreakAction.h"
#include "Boss1Model.h"
#include "../game/enemy/BaseBoss.h"
#include "Math/Easing/Easing.h"
#include "GameHelper.h"

BreakAction::BreakAction()
{
	boss->SetPlayerDirection();
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::back_end));
	boss->GetBaseModel()->AnimationReset();
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::back_start));
	boss->GetBaseModel()->AnimationReset();

	Vector3 target = boss->GetTargetPos();
	Vector3 bossPos = boss->GetBaseModel()->GetPosition();

	float angle = GetAngle({ target.x,target.z }, { bossPos.x,bossPos.z });
	boss->GetCenter()->SetRotation({ 0.0f,angle ,0.0f });

	timer = std::make_unique<Engine::Timer>();

	effect = std::make_unique<Boss1DestroyEffect>(boss->GetCenter()->GetPosition());
}

void BreakAction::Update()
{
	const float maxTime = 100.0f;
	timer->Update();

	if (*timer.get() < maxTime) {
		if (*timer.get() < 80) {
			effect->AddParticle();
		}
		float al = Easing::Lerp(1.0f, 0.0f, *timer.get() / maxTime);
		boss->GetCenter()->SetColor({ 1.0f,1.0f,1.0f, al });
	} else {
		isEnd = true;
	}

	effect->Update();
}

void BreakAction::Draw()
{
	effect->Draw();
}