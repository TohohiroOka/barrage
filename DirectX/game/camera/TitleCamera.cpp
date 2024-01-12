#include "TitleCamera.h"
#include "Math/Easing/Easing.h"

using namespace DirectX;

TitleCamera::TitleCamera()
	: GameCamera()
{
	//各状態の内容をセット
	func.emplace_back([this] { return NormalAction(); });
	func.emplace_back([this] { return MovePortalFrontAction(); });
	func.emplace_back([this] { return ZoomPortalAction(); });
	func.emplace_back([this] { return StayAction(); });

	//タイマークラス
	easeTimer = std::make_unique<Engine::Timer>();
}

TitleCamera::~TitleCamera()
{
}

void TitleCamera::Update()
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

void TitleCamera::ChangePhase(TitleCameraPhase phase)
{
	//フェーズを更新
	cameraPhase = phase;
	//イージング前座標をセット
	easeBeforePos = position;
	//タイマーリセット
	easeTimer->Reset();

	//フェーズごとに必要な情報をセット
	if (phase == TitleCameraPhase::MOVE_PORTAL_FRONT) {
		Vector3 portalDistance = { 0, 10, -50 };
		easeAfterPos = portalPos + portalDistance;
	}
	else if (phase == TitleCameraPhase::ZOOM_PORTAL) {
		Vector3 portalDistance = { 0, 0, -15 };
		easeAfterPos = portalPos + portalDistance;
	}

	//回転イージング情報をセット
	easeBeforeRota = rotation;
	easeAfterRota = VelocityRotate(portalPos - easeAfterPos);
	Rotate360(easeAfterRota.x);
	Rotate360(easeAfterRota.y);
	Rotate360(easeAfterRota.z);
	//y軸回転角が180以上の場合は逆回転になるように調整
	if (easeBeforeRota.y >= 180) {
		easeBeforeRota.y -= 360;
	}

	isPhaseActionEnd = false;
}

void TitleCamera::NormalAction()
{
	//ロックオン入力を受け付ける
	LockonInput();

	//トランスフォーム更新
	UpdateTransform();
}

void TitleCamera::MovePortalFrontAction()
{
	//行動が終了していたら抜ける
	if (isPhaseActionEnd) { return; }

	const float moveTime = 90;
	easeTimer->Update();
	const float time = *easeTimer.get() / moveTime;
	position.x = Easing::InOutQuart(easeBeforePos.x, easeAfterPos.x, time);
	position.y = Easing::InOutQuart(easeBeforePos.y, easeAfterPos.y, time);
	position.z = Easing::InOutQuart(easeBeforePos.z, easeAfterPos.z, time);

	rotation.x = Easing::InOutQuart(easeBeforeRota.x, easeAfterRota.x, time);
	rotation.y = Easing::InOutQuart(easeBeforeRota.y, easeAfterRota.y, time);
	rotation.z = Easing::InOutQuart(easeBeforeRota.z, easeAfterRota.z, time);

	if (*easeTimer.get() >= moveTime) {
		isPhaseActionEnd = true;
	}
}

void TitleCamera::ZoomPortalAction()
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

void TitleCamera::StayAction()
{
	//行動が終了していたら抜ける
	if (isPhaseActionEnd) { return; }

	const float stayTime = 45;
	easeTimer->Update();

	if (*easeTimer.get() >= stayTime) {
		isPhaseActionEnd = true;
	}
}
