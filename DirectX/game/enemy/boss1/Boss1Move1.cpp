#include "Boss1Move1.h"
#include "../BaseBoss.h"
#include "Math/Easing/Easing.h"
#include "GameHelper.h"

Boss1Move1::Boss1Move1(DirectX::XMFLOAT3 _pos)
{
	startPos = boss->GetCenter()->GetPosition();
	endPos = _pos + startPos;
	timer = 0;
}

void Boss1Move1::Update()
{
	Vector3 pos = Vector3(boss->GetCenter()->GetPosition());
	const float maxTimer = 50.0f;
	const float rate = timer / maxTimer;
	pos.x = Easing::OutCubic(startPos.x, endPos.x, rate) - pos.x;
	pos.y = Easing::OutCubic(startPos.y, endPos.y, rate) - pos.y;
	pos.z = Easing::OutCubic(startPos.z, endPos.z, rate) - pos.z;

	boss->SetMoveVec(pos);

	timer++;

	if (rate < 1.0f) { return; }
	isEnd = true;
}
