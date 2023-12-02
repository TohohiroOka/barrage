#pragma once

#include "BaseOnStageDirection.h"

class Boss1OnStage : public BaseOnStageDirection
{
public:
	Boss1OnStage() = default;
	~Boss1OnStage() = default;


	void Init() override;

	void Update() override;

	void Draw() override;

	void SetBase3DCamera();

	void StartDirection();

	bool GetIsDirectEnd() { return camera->GetIsCameraMoveEnd(); }

	void Reset();

private:
	//�����A�j���[�V�����p
	int frame = 0;
	//�A�j���[�V�����Đ�����
	const int TEXTANIM_MAXFRAME = 15;
	bool isDisplayBossText = false;
};