#include "BaseBullet.h"

void BaseBullet::Update()
{
	//タイマー加算
	timer++;
}

void BaseBullet::Draw()
{
	instanceObject->Draw();
	predictionLine->Draw();
}

void BaseBullet::FrameReset()
{
	instanceObject->FrameReset();
	predictionLine->FrameReset();
}