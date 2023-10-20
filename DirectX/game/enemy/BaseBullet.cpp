#include "BaseBullet.h"

void BaseBullet::Update()
{
	//�^�C�}�[���Z
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