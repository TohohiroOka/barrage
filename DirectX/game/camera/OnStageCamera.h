#pragma once
#include "Camera/Camera.h"
#include "Math/Vector3.h"

#include <vector>

//�o�����o�p�̃J����
class OnStageCamera
{
public:
	enum EASEMODE
	{
		EASE_LERP,
		EASE_IN_QUAD,
		EASE_OUT_QUAD,
		EASE_IO_QUAD,
	};

	void Init();

	void Update();

	//���s�ړ��A�J����(eye)���W��start->end�Ɏ����Ă����Atarget��eye�Ƃ̑��΍��W
	void SetPan(Vector3 startEyePos, Vector3 endEyePos, Vector3 targetLocal, int frame, EASEMODE mode);

	//�����_�����̂܂܁A���_���ړ�������
	//���`���
	void SetTrack(Vector3 targetPos, Vector3 eyeStart, Vector3 eyeEnd, int frame, EASEMODE mode);

	//���_�����̂܂܁A�����_���ړ�������
	//���`���
	void SetZoom(Vector3 eyePos, Vector3 targetStartPos, Vector3 targetEndPos, int frame, EASEMODE mode);

	//���_�A�����_�̗�������`��Ԃňړ�������
	void SetMove(Vector3 eyeStart, Vector3 eyeEnd, Vector3 targetStart, Vector3 targetEnd, int frame, EASEMODE mode);

	//Set�֐��Ŏw�肵���J����������J�n����
	void StartCameraMove();

	Camera* GetCamera() { return camera.get(); }

	bool GetIsCameraMoveEnd() { return isEnd; }

	int GetUsingCameraNum() { return indexNum; }

	Vector3 GetEyePos() { return eyePos; }
	Vector3 GetTargetPos() { return targetPos; }

private:

	struct CameraMoveStatus
	{
		Vector3		eyeStart;
		Vector3		eyeEnd;
		Vector3		targetStart;
		Vector3		targetEnd;
		int			maxFrame;
		EASEMODE	mode;
	};
	std::vector<CameraMoveStatus> moveStatus;

	Vector3		eyePos;
	Vector3		targetPos;
	int			frame;

	int indexNum = 0;

	bool isEnd = false;

	std::unique_ptr<Camera> camera;
};