#include "Boss1Move1.h"
#include "../BaseBoss.h"
#include "Math/Easing/Easing.h"
#include "GameHelper.h"

Boss1Move1::Boss1Move1(const DirectX::XMFLOAT3& _pos)
{
	using namespace DirectX;
	startPos = boss->GetCenter()->GetPosition();
	XMFLOAT3 pos=_pos;
	if (pos.x == 0.0f && pos.y == 0.0f && pos.z == 0.0f) {
		pos.x = RandomFloat(50.0f);
		pos.y = 0.0f;
		pos.z = RandomFloat(50.0f);
	}

	endPos = pos + startPos;
	timer = std::make_unique<Engine::Timer>();
}

void Boss1Move1::Update()
{
	Vector3 pos = Vector3(boss->GetCenter()->GetPosition());
	const float maxTimer = 50.0f;
	const float rate = *timer.get() / maxTimer;
	pos.x = Easing::OutCubic(startPos.x, endPos.x, rate) - pos.x;
	pos.y = Easing::OutCubic(startPos.y, endPos.y, rate) - pos.y;
	pos.z = Easing::OutCubic(startPos.z, endPos.z, rate) - pos.z;

	boss->SetMoveVec(pos);

	timer->Update();

	if (rate < 1.0f) { return; }
	isEnd = true;
}
