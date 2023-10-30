#include "Boss1Move2.h"
#include "../BaseBoss.h"
#include "GameHelper.h"
#include "Math/Easing/Easing.h"

Boss1Move2::Boss1Move2()
{
	boss->GetBaseModel()->SetAnimation(1);

	using namespace DirectX;
	XMFLOAT3 target = boss->GetTargetPos();
	startPos = boss->GetCenter()->GetPosition();

	float stageSize = GameHelper::Instance()->GetStageSize() / 2.0f;

	//íÜêSÇ©ÇÁÇÃç∑
	XMFLOAT2 random = { RandomFloat(100.0f) - 50.0f,RandomFloat(100.0f) - 50.0f };

	endPos = { stageSize + random.x,10.0f,stageSize + random.y };

	state = State::up;

	timer = std::make_unique<Engine::Timer>();

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
	const Vector3 pos = Vector3(boss->GetCenter()->GetPosition());

	float moveY = Easing::OutBack(startPos.y, 100.0f, rate) - pos.y;

	boss->SetMoveVec({ 0.0f,moveY,0.0f });

	if (rate < 1.0f) { return; }

	state = State::side;
	timer->Reset();
	startPos = boss->GetCenter()->GetPosition();
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
