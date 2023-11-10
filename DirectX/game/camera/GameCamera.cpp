#include "GameCamera.h"
#include "system/GameInputManager.h"
#include "WindowApp.h"
#include "GameHelper.h"
#include "Math/Easing/Easing.h"

using namespace DirectX;

Player* GameCamera::player = nullptr;
const float GameCamera::rotateXMin = -45;
const float GameCamera::rotateXMax = 70;
const float GameCamera::rotateXBase = 7;
const float GameCamera::rotateCenterDistanceNormal = 15.0f;
const Vector3 GameCamera::targetDistanceNormal = { 0, 0, 0 };

GameCamera::GameCamera() :
	Camera(true)
{
	//回転の中心との距離をセット
	rotateCenterDistance = rotateCenterDistanceNormal;
	//ターゲットの中心からずらす距離をセット
	targetDistance = targetDistanceNormal;

	//初期の回転角をセット
	rotation.x = rotateXBase;

	//タイマークラス
	lockonChangeRotaTimer = std::make_unique<Engine::Timer>();
}

GameCamera::~GameCamera()
{
}

void GameCamera::Update()
{
	//ロックオン入力を受け付ける
	LockonInput();

	//トランスフォーム更新
	UpdateTransform();

	//カメラ座標回転角を反映して更新
	//平行移動行列の計算
	const XMMATRIX matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//ワールド行列を更新
	UpdateMatWorld(matTrans);
	//視点、注視点を更新
	UpdateEyeTarget();
	Camera::Update();
}

void GameCamera::Lockon(Base3D* lockonTarget)
{
	if (!lockonTarget) { return; }

	this->lockonTarget = lockonTarget;

	easeBeforeRota = rotation;
	lockonChangeRotaTimer->Reset();
	isLockonEndRotate = false;

	isLockon = true;
}

void GameCamera::UpdateMatWorld(const XMMATRIX& matTrans)
{
	//回転　
	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	//子である自機用のワールド行列の合成
	matWorld = XMMatrixIdentity();	//変形をリセット
	matWorld *= matRot;		//ワールド行列に回転を反映
	matWorld *= matTrans;	//ワールド行列に平行移動を反映
}

void GameCamera::UpdateEyeTarget()
{
	//視点をワールド座標に設定
	eye = { matWorld.r[3].m128_f32[0], matWorld.r[3].m128_f32[1], matWorld.r[3].m128_f32[2] };
	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	//カメラの回転を反映させる
	forward = MatrixTransformDirection(forward, matWorld);
	//視点から前方に進んだ位置を注視点に設定
	target = eye + forward;
	//天地が反転してもいいように上方向ベクトルも回転させる
	Vector3 baseUp(0, 1, 0);
	up = MatrixTransformDirection(baseUp, matWorld);
}

void GameCamera::UpdateTransform()
{
	if (!isLockon) {
		if (isLockonEndRotate) {
			LockonEndRotate();
		}
		else {
			UpdateRotate();
		}
	}
	else {
		UpdateLockonRotate();
	}
	UpdatePosition();

	PositionCollision();
}

void GameCamera::UpdateRotate()
{
	DirectInput* input = DirectInput::GetInstance();
	//視界移動
	const float rotSpeed = 2.0f;
	XMFLOAT2 rotNum = { 0,0 };//回転量

	//キー入力
	{
		if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::CameraLeftRota).key)) { rotNum.y -= rotSpeed; }//右入力
		if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::CameraRightRota).key)) { rotNum.y += rotSpeed; }//左入力
		if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::CameraDownRota).key)) { rotNum.x += rotSpeed; }//下入力
		if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::CameraUpRota).key)) { rotNum.x -= rotSpeed; }//上入力
	}
	//コントローラー入力
	{
		//ある程度スティックを傾けないと判定しない
		const float moveStickIncline = 0.3f;
		const XMFLOAT2 padIncline = XInputManager::GetInstance()->GetPadRStickIncline();
		if (fabsf(padIncline.x) >= moveStickIncline || fabsf(padIncline.y) >= moveStickIncline) {
			const XMFLOAT2 padIncline = GameInputManager::GetPadRStickIncline();
			const float stickRadian = GameInputManager::GetPadRStickRadian();
			rotNum.y += cosf(stickRadian) * fabsf(padIncline.x) * rotSpeed;
			rotNum.x += sinf(stickRadian) * fabsf(padIncline.y) * rotSpeed;
		}
	}

	//入力設定がリバースモードなら回転量を逆にする
	if (GameInputManager::GetIsCameraRotaYReverse()) { rotNum.y = -rotNum.y; }
	if (GameInputManager::GetIsCameraRotaXReverse()) { rotNum.x = -rotNum.x; }

	//回転量を加算して回転させる
	rotation.y += rotNum.y;
	rotation.x += rotNum.x;

	//上下方向の角度制限
	rotation.x = max(rotation.x, rotateXMin);
	rotation.x = min(rotation.x, rotateXMax);

	//横回転の回転角を0～360以内に収まるようにする
	if (rotation.y > 360) { rotation.y -= 360; }
	if (rotation.y < 0) { rotation.y += 360; }
}

void GameCamera::UpdateLockonRotate()
{
	//回転角変更にかかる時間
	const float changeRotaTime = 15;

	//プレイヤーとロックオンターゲットの角度を取得(0～360に調整)
	Vector3 moveRotaVelocity = VelocityRotate(((Vector3)lockonTarget->GetPosition() - targetDistance) - (player->GetPosition() + targetDistance));
	while (moveRotaVelocity.y < 0 || moveRotaVelocity.y > 360) {
		//横回転の回転角を0～360以内に収まるようにする
		if (moveRotaVelocity.y > 360) { moveRotaVelocity.y -= 360; }
		else if (moveRotaVelocity.y < 0) { moveRotaVelocity.y += 360; }
	}
	while (moveRotaVelocity.x < 0 || moveRotaVelocity.x > 360) {
		//縦回転の回転角を0～360以内に収まるようにする
		if (moveRotaVelocity.x > 360) { moveRotaVelocity.x -= 360; }
		else if (moveRotaVelocity.x < 0) { moveRotaVelocity.x += 360; }
	}

	//イージングで動かす場合
	if (*lockonChangeRotaTimer.get() <= changeRotaTime) {
		lockonChangeRotaTimer->Update();

		//回転をなるべく短くするために元角度を調整(例：350→10 より -10→10の方が近い)
		Vector3 adjustRota = easeBeforeRota;
		if (adjustRota.y > moveRotaVelocity.y) {
			if (adjustRota.y - moveRotaVelocity.y > 180) {
				adjustRota.y -= 360;
			}
		}
		else if (adjustRota.y < moveRotaVelocity.y) {
			if (moveRotaVelocity.y - adjustRota.y > 180) {
				adjustRota.y += 360;
			}
		}
		if (adjustRota.x > moveRotaVelocity.x) {
			if (adjustRota.x - moveRotaVelocity.x > 180) {
				adjustRota.x -= 360;
			}
		}
		else if (adjustRota.x < moveRotaVelocity.x) {
			if (moveRotaVelocity.x - adjustRota.x > 180) {
				adjustRota.x += 360;
			}
		}

		//ロックオンしたターゲットの方向をイージングで向くようにする
		const float time = *lockonChangeRotaTimer.get() / changeRotaTime;
		rotation.y = Easing::OutCubic(adjustRota.y, moveRotaVelocity.y, time);
		rotation.x = Easing::OutCubic(adjustRota.x, moveRotaVelocity.x, time);
	}
	else {
		//ロックオンしたターゲットの方向を向く
		rotation = moveRotaVelocity;
	}
}

void GameCamera::UpdatePosition()
{
	//X,Y回転角をラジアンに直す
	Vector2 radian;
	radian.x = XMConvertToRadians(rotation.x);
	radian.y = XMConvertToRadians(rotation.y);
	//アンダーフローする可能性があるので、小数点を切り捨てる
	const float divNum = 1000;
	radian.x = floorf(radian.x * divNum) / divNum;
	radian.y = floorf(radian.y * divNum) / divNum;

	//ターゲットへの追従を指定したフレームの分遅らせる
	targetPositionsKeep.push_back(player->GetPosition());
	const int frame = 3;
	if (targetPositionsKeep.size() > frame) {
		targetPositionsKeep.pop_front();
	}

	//計算結果を割り当てて座標をセット
	//Y座標はX回転角のsinを使用
	//X,Z座標はY回転角のsin,cosで計算し、X回転角(Y座標)のcosを乗算して算出
	Vector3 targetPos = targetPositionsKeep.front() + targetDistance;
	position.x = (-sinf(radian.y) * cosf(radian.x)) * rotateCenterDistance + targetPos.x;
	position.y = sinf(radian.x) * rotateCenterDistance + targetPos.y;
	position.z = (-cosf(radian.y) * cosf(radian.x)) * rotateCenterDistance + targetPos.z;
}

void GameCamera::PositionCollision()
{
	//地面のめり込みを解消する
	const float groundHeight = 1; //仮置き地面の高さ
	const float collisionHeight = groundHeight + 0.1f; //押し戻しを開始する高さ
	if (position.y < collisionHeight) {
		//めり込み量を算出
		const float sinkNum = collisionHeight - position.y;

		//高さ押し戻し
		position.y += sinkNum;

		//押し戻しの分、X,Zの同じ量移動させる
		Vector2 posXZ = { position.x, position.z };
		Vector2 targetPosXZ = { player->GetPosition().x, player->GetPosition().z };
		Vector2 vec = targetPosXZ - posXZ;
		vec.normalize();

		position.x += vec.x * sinkNum;
		position.z += vec.y * sinkNum;
	}
}

void GameCamera::LockonInput()
{
	//ロックオンターゲット検出フラグを毎フレーム戻しておく
	isLockonStart = false;

	//ロックオン入力がなければ抜ける
	if (!GameInputManager::TriggerInputAction(GameInputManager::Lockon)) { return; }

	//ロックオンしていない場合はロックオンターゲットを検出する状態にする
	if (!isLockon) {
		isLockonStart = true;
	}
	//ロックオン中ならロックオンを解除する
	else {
		lockonTarget = nullptr;
		isLockon = false;

		//ロックオン解除時に回転角を戻すか
		const float rotateLine = 5.0f; //回転角をイージングで動かすかを決定する差分
		if (rotation.x > rotateLine || rotation.x < -rotateLine) {
			easeBeforeRota = rotation;
			lockonChangeRotaTimer->Reset();

			isLockonEndRotate = true;
		}
	}
}

void GameCamera::LockonEndRotate()
{
	//回転角変更にかかる時間
	const float changeRotaTime = 15;
	lockonChangeRotaTimer->Update();
	const float time = *lockonChangeRotaTimer.get() / changeRotaTime;

	//回転をなるべく短くするために元角度を調整(例：350→10 より -10→10の方が近い)
	Vector3 adjustRota = easeBeforeRota;
	if (adjustRota.x > 180) {
		adjustRota.x -= 360;
	}

	//初期の高さにカメラをイージングで動かす
	rotation.x = Easing::OutCubic(adjustRota.x, rotateXBase, time);

	//タイマーが指定した時間になったら終了
	if (*lockonChangeRotaTimer.get() > changeRotaTime) {
		isLockonEndRotate = false;
	}
}
