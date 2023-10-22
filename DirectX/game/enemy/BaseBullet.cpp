#include "BaseBullet.h"

void BaseBullet::Update()
{
	//�^�C�}�[���Z
	timer->Update();
}

void BaseBullet::Draw()
{
	for (auto& i : instanceObject) {
		if (i->GetInstanceDrawNum() == 0) { continue; }
		i->Draw();
	}

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