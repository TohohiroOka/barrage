#pragma once
#include "Camera/Camera.h"
#include "Math/Vector3.h"

#include <vector>

//出現演出用のカメラ
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

	//平行移動、カメラ(eye)座標をstart->endに持っていく、targetはeyeとの相対座標
	void SetPan(Vector3 startEyePos, Vector3 endEyePos, Vector3 targetLocal, int frame, EASEMODE mode);

	//注視点をそのまま、視点を移動させる
	//線形補間
	void SetTrack(Vector3 targetPos, Vector3 eyeStart, Vector3 eyeEnd, int frame, EASEMODE mode);

	//視点をそのまま、注視点を移動させる
	//線形補間
	void SetZoom(Vector3 eyePos, Vector3 targetStartPos, Vector3 targetEndPos, int frame, EASEMODE mode);

	//視点、注視点の両方を線形補間で移動させる
	void SetMove(Vector3 eyeStart, Vector3 eyeEnd, Vector3 targetStart, Vector3 targetEnd, int frame, EASEMODE mode);

	//Set関数で指定したカメラ操作を開始する
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