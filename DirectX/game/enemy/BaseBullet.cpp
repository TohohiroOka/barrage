#include "BaseBullet.h"

void BaseBullet::Update()
{
	//タイマー加算
	timer->Update();
	bulletEffect.Update();

	for (auto& i : instanceObject) {
		if (i->GetInstanceDrawNum() == 0) { continue; }
		i->Update();
	}

	if (predictionLine) {
		predictionLine->Update();
	}

	if (!isCollision) {
		hitTimer->Update();
		//判定を取らない時間以上になったら戻す
		if (*hitTimer.get() > 2) {
			isCollision = true;
		}
	}
}

void BaseBullet::Draw()
{
	for (auto& i : instanceObject) {
		if (i->GetInstanceDrawNum() == 0) { continue; }
		i->Draw();
	}

	bulletEffect.Draw();

	if (predictionLine) {
		predictionLine->Draw();
	}
}

void BaseBullet::FrameReset()
{
	for (auto& i : instanceObject) {
		if (i->GetInstanceDrawNum() == 0) { continue; }
		i->FrameReset();
	}
	if (predictionLine) {
		predictionLine->FrameReset();
	}
}