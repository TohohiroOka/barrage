#include "OnStageCamera.h"
#include "Math/Easing/Easing.h"

void OnStageCamera::Init()
{
	camera = Camera::Create();
	camera->SetUp({ 0,1,0 });
}

void OnStageCamera::Update()
{
	//操作が終わっているか
	if (isEnd) { return; }
	//時間更新
	frame++;
	//カメラ操作終了
	if (indexNum == moveStatus.size() - 1 && frame >= moveStatus[indexNum].maxFrame) { 
		isEnd = true; 
		return;
	}
	//カメラのフェーズ更新
	if (frame >= moveStatus[indexNum].maxFrame && indexNum < moveStatus.size()) { 
		indexNum++; 
		frame = 0;
	}


	float rate = float(frame) / float(moveStatus[indexNum].maxFrame);

	switch (moveStatus[indexNum].mode)
	{
	case EASE_LERP:
		eyePos		= lerp(moveStatus[indexNum].eyeStart, moveStatus[indexNum].eyeEnd, rate);
		targetPos	= lerp(moveStatus[indexNum].targetStart, moveStatus[indexNum].targetEnd, rate);
		break;
	case EASE_IN_QUAD:
		eyePos = easeIn(moveStatus[indexNum].eyeStart, moveStatus[indexNum].eyeEnd, rate);
		targetPos = easeIn(moveStatus[indexNum].targetStart, moveStatus[indexNum].targetEnd, rate);
		break;
	case EASE_OUT_QUAD:
		eyePos = easeOut(moveStatus[indexNum].eyeStart, moveStatus[indexNum].eyeEnd, rate);
		targetPos = easeOut(moveStatus[indexNum].targetStart, moveStatus[indexNum].targetEnd, rate);
		break;
	case EASE_IO_QUAD:
		eyePos = easeInOut(moveStatus[indexNum].eyeStart, moveStatus[indexNum].eyeEnd, rate);
		targetPos = easeInOut(moveStatus[indexNum].targetStart, moveStatus[indexNum].targetEnd, rate);
		break;

	default:
		break;
	}

	camera->SetEye(eyePos);
	camera->SetTarget(targetPos);
	camera->Update();

}

void OnStageCamera::SetPan(Vector3 startEyePos, Vector3 endEyePos, Vector3 targetLocal, int frame, EASEMODE mode)
{
	CameraMoveStatus addStatus;
	addStatus.eyeStart		= startEyePos;
	addStatus.eyeEnd		= endEyePos;
	addStatus.targetStart	= startEyePos + targetLocal;
	addStatus.targetEnd		= endEyePos + targetLocal;
	addStatus.maxFrame		= frame;
	addStatus.mode			= mode;

	moveStatus.push_back(addStatus);
}

void OnStageCamera::SetTrack(Vector3 targetPos, Vector3 eyeStart, Vector3 eyeEnd, int frame, EASEMODE mode)
{
	CameraMoveStatus addStatus;
	addStatus.eyeStart		= eyeStart;
	addStatus.eyeEnd		= eyeEnd;
	addStatus.targetStart	= targetPos;
	addStatus.targetEnd		= targetPos;
	addStatus.maxFrame		= frame;
	addStatus.mode			= mode;

	moveStatus.push_back(addStatus);
}

void OnStageCamera::SetZoom(Vector3 eyePos, Vector3 targetStartPos, Vector3 targetEndPos, int frame, EASEMODE mode)
{
	CameraMoveStatus addStatus;
	addStatus.eyeStart		= eyePos;
	addStatus.eyeEnd		= eyePos;
	addStatus.targetStart	= targetStartPos;
	addStatus.targetEnd		= targetEndPos;
	addStatus.maxFrame		= frame;
	addStatus.mode			= mode;

	moveStatus.push_back(addStatus);
}

void OnStageCamera::SetMove(Vector3 eyeStart, Vector3 eyeEnd, Vector3 targetStart, Vector3 targetEnd, int frame, EASEMODE mode)
{
	CameraMoveStatus addStatus;
	addStatus.eyeStart		= eyePos;
	addStatus.eyeEnd		= eyePos;
	addStatus.targetStart	= targetStart;
	addStatus.targetEnd		= targetEnd;
	addStatus.maxFrame		= frame;
	addStatus.mode			= mode;
}

void OnStageCamera::StartCameraMove()
{
	frame = 0;
	indexNum = 0;
	isEnd = false;
	eyePos = moveStatus[indexNum].eyeStart;
	targetPos = moveStatus[indexNum].targetStart;
}
