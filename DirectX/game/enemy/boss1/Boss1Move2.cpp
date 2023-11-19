#include "Boss1Move2.h"
#include "../BaseBoss.h"
#include "GameHelper.h"
#include "Math/Easing/Easing.h"

Boss1Move2::Boss1Move2()
{
	//アニメーションセット
	boss->GetBaseModel()->AnimationReset();
	boss->GetBaseModel()->SetAnimation(0);

	Vector3 target = boss->GetTargetPos();
	startPos = boss->GetCenter()->GetPosition();

	Vector3 move = Vector3(startPos) - target;
	move = move.normalize();

	endPos = move * 100.0f;
	endPos.y = 0.0f;

	state = State::up;

	timer = std::make_unique<Engine::Timer>();

	hitTimer = std::make_unique<Engine::Timer>();

	func_.emplace_back([this] {return UpMove(); });
	func_.emplace_back([this] {return SideMove(); });
}

void Boss1Move2::Update()
{
	if (state < State::size) {
		func_[int(state)]();
	}
}

void Boss1Move2::UpMove()
{
	timer->Update();
	const float maxTimer = 50.0f;
	const float rate = *timer.get() / maxTimer;

	if (rate < 1.0f) {
		const Vector3 pos = Vector3(boss->GetCenter()->GetPosition());
		float moveY = Easing::OutBack(startPos.y, 50.0f, rate) - pos.y;
		boss->SetMoveVec({ 0.0f,moveY,0.0f });
	} else if(rate > 1.1f) {
		state = State::side;
		timer->Reset();
		startPos = boss->GetCenter()->GetPosition();
	}
}

void Boss1Move2::SideMove()
{
	timer->Update();
	const float maxTimer = 50.0f;
	const float rate = *timer.get() / maxTimer;
	Vector3 pos = Vector3(boss->GetCenter()->GetPosition());

	pos.x = Easing::OutCirc(startPos.x, endPos.x, rate) - pos.x;
	pos.y = Easing::InCubic(startPos.y, endPos.y, rate) - pos.y;
	pos.z = Easing::OutCirc(startPos.z, endPos.z, rate) - pos.z;

	boss->SetMoveVec(pos);

	if (rate < 1.0f) { return; }
	isEnd = true;
	timer->Reset();
	startPos = boss->GetCenter()->GetPosition();
}
