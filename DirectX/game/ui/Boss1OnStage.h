#pragma once

#include "BaseOnStageDirection.h"

class Boss1OnStage : public BaseOnStageDirection
{
public:

	void Init() override;

	void Update() override;

	void Draw() override;

	void SetBase3DCamera();

	void StartDirection();

	bool GetIsDirectEnd() { return camera.GetIsCameraMoveEnd(); }

	void Reset();

private:
	//�����A�j���[�V�����p
	int frame = 0;
	//�A�j���[�V�����Đ�����
	const int TEXTANIM_MAXFRAME = 15;
	bool isDisplayBossText = false;

	//�������^�[�{�b�N�X
	std::unique_ptr<Sprite> letterBoxUnder;
};