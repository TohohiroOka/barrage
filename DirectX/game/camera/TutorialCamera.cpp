#include "TutorialCamera.h"
#include "Math/Easing/Easing.h"

using namespace DirectX;

TutorialCamera::TutorialCamera()
	: GameCamera()
{
	//各状態の内容をセット
	func.emplace_back([this] { return NormalAction(); });
	func.emplace_back([this] { return ZoomEnemyBornAction(); });
	func.emplace_back([this] { return ZoomEndReturnAction(); });

	//タイマークラス
	easeTimer = std::make_unique<Engine::Timer>();
}

TutorialCamera::~TutorialCamera()
{
}

void TutorialCamera::Update()
{
	//各状態の内容を更新
	func[int(cameraPhase)]();

	//カメラ座標回転角を反映して更新
	//平行移動行列の計算
	const XMMATRIX matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//ワールド行列を更新
	UpdateMatWorld(matTrans);
	//視点、注視点を更新
	UpdateEyeTarget();
	Camera::Update();
}

void TutorialCamera::ChangePhase(TutorialCameraPhase phase)
{
	//フェーズを更新
	cameraPhase = phase;
	//イージング前座標をセット
	easeBeforePos = position;
	//回転イージング情報をセット
	easeBeforeRota = rotation;
	//タイマーリセット
	easeTimer->Reset();

	//フェーズごとに必要な情報をセット
	if (phase == TutorialCameraPhase::ZOOM_ENEMY_BORN) {
		Vector3 distance = { 0, 10, -50 };
		easeAfterPos = zoomTargetPos + distance;
		easeAfterRota = VelocityRotate(zoomTargetPos - easeAfterPos);

		//ズーム前の情報をセット
		zoomBeforePos = position;
		zoomBeforeRota = rotation;
	}
	else if (phase == TutorialCameraPhase::ZOOM_END_RETURN) {
		easeAfterPos = zoomBeforePos;
		easeAfterRota = zoomBeforeRota;
	}

	Rotate360(easeAfterRota.x);
	Rotate360(easeAfterRota.y);
	Rotate360(easeAfterRota.z);
	//y軸回転角が180以上の場合は逆回転になるように調整
	if (fabsf(easeAfterRota.y - easeBeforeRota.y) > 180) {
		if (easeAfterRota.y > easeBeforeRota.y) { easeAfterRota.y -= 360; }
		else { easeAfterRota.y += 360; }
	}
	if (fabsf(easeAfterRota.x - easeBeforeRota.x) > 180) {
		if (easeAfterRota.x > easeBeforeRota.x) { easeAfterRota.x -= 360; }
		else { easeAfterRota.x += 360; }
	}

	isPhaseActionEnd = false;
}

void TutorialCamera::NormalAction()
{
	//ロックオン入力を受け付ける
	LockonInput();

	//トランスフォーム更新
	UpdateTransform();
}

void TutorialCamera::ZoomEnemyBornAction()
{
	//行動が終了していたら抜ける
	if (isPhaseActionEnd) { return; }

	const float zoomTime = 90;
	easeTimer->Update();
	const float time = *easeTimer.get() / zoomTime;
	position.x = Easing::InOutQuart(easeBeforePos.x, easeAfterPos.x, time);
	position.y = Easing::InOutQuart(easeBeforePos.y, easeAfterPos.y, time);
	position.z = Easing::InOutQuart(easeBeforePos.z, easeAfterPos.z, time);

	rotation.x = Easing::InOutQuart(easeBeforeRota.x, easeAfterRota.x, time);
	rotation.y = Easing::InOutQuart(easeBeforeRota.y, easeAfterRota.y, time);
	rotation.z = Easing::InOutQuart(easeBeforeRota.z, easeAfterRota.z, time);

	if (*easeTimer.get() >= zoomTime) {
		isPhaseActionEnd = true;
	}
}

void TutorialCamera::ZoomEndReturnAction()
{
	//行動が終了していたら抜ける
	if (isPhaseActionEnd) { return; }

	const float zoomTime = 100;
	easeTimer->Update();
	const float time = *easeTimer.get() / zoomTime;
	position.x = Easing::InOutQuad(easeBeforePos.x, easeAfterPos.x, time);
	position.y = Easing::InOutQuad(easeBeforePos.y, easeAfterPos.y, time);
	position.z = Easing::InOutQuad(easeBeforePos.z, easeAfterPos.z, time);

	rotation.x = Easing::InOutQuad(easeBeforeRota.x, easeAfterRota.x, time);
	rotation.y = Easing::InOutQuad(easeBeforeRota.y, easeAfterRota.y, time);
	rotation.z = Easing::InOutQuad(easeBeforeRota.z, easeAfterRota.z, time);

	if (*easeTimer.get() >= zoomTime) {
		isPhaseActionEnd = true;
	}
}
