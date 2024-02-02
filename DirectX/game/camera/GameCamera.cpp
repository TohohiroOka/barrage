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
const float GameCamera::rotateCenterDistanceNormal = 35.0f;
const Vector3 GameCamera::targetDistanceNormal = { 0, 5, 0 };

GameCamera::GameCamera() :
	Camera(true)
{
	//入力で行動できる状態にセット
	actionInput.isCameraMove = true;
	actionInput.isLockon = true;

	//回転の中心との距離をセット
	rotateCenterDistance = rotateCenterDistanceNormal;
	//ターゲットの中心からずらす距離をセット
	targetDistance = targetDistanceNormal;

	//初期の回転角をセット
	rotation.x = rotateXBase;

	//タイマークラス
	lockonChangeRotaTimer = std::make_unique<Engine::Timer>();
	lockonRotYStopTimer = std::make_unique<Engine::Timer>();
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
	isLockonRotYStop = false;

	isLockon = true;
}

void GameCamera::LockonEnd(bool isLockonEndRotateStart)
{
	//そもそもロックオンしていなければ抜ける
	if (!isLockon) { return; }

	//ロックオン解除
	lockonTarget = nullptr;
	isLockon = false;

	if (!isLockonEndRotateStart) { return; }

	//ロックオン解除時に回転角を戻すか
	const float rotateLine = 5.0f; //回転角をイージングで動かすかを決定する差分
	if (rotation.x > rotateLine || rotation.x < -rotateLine) {
		easeBeforeRota = rotation;
		lockonChangeRotaTimer->Reset();

		isLockonEndRotate = true;
	}
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
		UpdateLockonTargetDistance();
		UpdateLockonRotate();
	}
	UpdatePosition();

	PositionCollision();
}

void GameCamera::UpdateRotate()
{
	//入力を判定しない状態なら抜ける
	if (!actionInput.isCameraMove) { return; }

	DirectInput* input = DirectInput::GetInstance();
	//視界移動
	const float rotSpeed = 2.5f;
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
	Rotate360(rotation.y);
}

void GameCamera::UpdateLockonTargetDistance()
{
	//プレイヤーの速度に応じてロックオンターゲットから離す距離を算出
	const float multNum = 10;
	//プレイヤーとロックオン対象との距離を計算
	const float playerLockonTargetDistance = sqrtf((player->GetData()->pos.x - lockonTarget->GetPosition().x) * (player->GetData()->pos.x - lockonTarget->GetPosition().x) +
		(player->GetData()->pos.z - lockonTarget->GetPosition().z) * (player->GetData()->pos.z - lockonTarget->GetPosition().z));
	const Vector3 lockonTargetDistanceNum = player->GetData()->velocity * multNum * (playerLockonTargetDistance / 100);

	//プレイヤーとロックオン対象との距離が一定数以下の場合はy軸回転を止める
	const float rotYStopDistanceNum = 3.0f;
	if (fabsf(playerLockonTargetDistance) < rotYStopDistanceNum) {
		isLockonRotYStop = true; 
		lockonRotYStopTimer->Reset(); 
		easeBeforeRota.y = rotation.y;
	}

	//現在の離す距離から、算出した離す距離まで動かしていく
	const float distanceChangeSpeed = 1 * GameHelper::Instance()->GetGameSpeed();
	if (lockonTargetDistance.x < lockonTargetDistanceNum.x) {
		lockonTargetDistance.x += distanceChangeSpeed;
		lockonTargetDistance.x = min(lockonTargetDistance.x, lockonTargetDistanceNum.x);
	}
	else if (lockonTargetDistance.x > lockonTargetDistanceNum.x) {
		lockonTargetDistance.x -= distanceChangeSpeed;
		lockonTargetDistance.x = max(lockonTargetDistance.x, lockonTargetDistanceNum.x);
	}

	if (lockonTargetDistance.z < lockonTargetDistanceNum.z) {
		lockonTargetDistance.z += distanceChangeSpeed;
		lockonTargetDistance.z = min(lockonTargetDistance.z, lockonTargetDistanceNum.z);
	}
	else if (lockonTargetDistance.z > lockonTargetDistanceNum.z) {
		lockonTargetDistance.z -= distanceChangeSpeed;
		lockonTargetDistance.z = max(lockonTargetDistance.z, lockonTargetDistanceNum.z);
	}
}

void GameCamera::UpdateLockonRotate()
{
	//プレイヤーとロックオンターゲットの角度を取得(0～360に調整)
	Vector3 moveRotaVelocity = VelocityRotate((((Vector3)lockonTarget->GetPosition() + lockonTargetDistance) - targetDistance) - (player->GetData()->pos + targetDistance));
	//回転角を0～360以内に収まるようにする
	Rotate360(moveRotaVelocity.y);
	Rotate360(moveRotaVelocity.x);

	//開始時にイージングで動かす場合のタイマーを更新	
	const float  lockonStartChangeRotaTime = 15;//回転角変更にかかる時間
	float lockonStartEaseTimer;
	if (*lockonChangeRotaTimer.get() <= lockonStartChangeRotaTime) {
		lockonChangeRotaTimer->Update();
		//ロックオンしたターゲットの方向をイージングで向くようにする
		lockonStartEaseTimer = *lockonChangeRotaTimer.get() / lockonStartChangeRotaTime;
	}


	//y軸回転
	{
		//y軸回転を停止させる場合
		if (isLockonRotYStop) {
			const int stopTime = 30;
			lockonRotYStopTimer->Update();

			if (*lockonRotYStopTimer.get() >= stopTime) {
				const float rotTime = 40.0f;
				//ロックオンしたターゲットの方向をイージングで向くようにする
				const float time = (*lockonRotYStopTimer.get() - stopTime) / rotTime;

				//360から急に0に戻ってしまうのを防ぐために前フレームのプレイヤーとロックオン対象との角度との差を計算して調整
				float adjustMoveRotaVelocity = moveRotaVelocity.y;
				if (*lockonRotYStopTimer.get() > stopTime) {
					if (moveRotaVelocity.y <= 90 && oldMoveRotaVelocity >= 270) {
						adjustMoveRotaVelocity += 360;
					}
					else if (oldMoveRotaVelocity <= 90 && moveRotaVelocity.y >= 270) {
						adjustMoveRotaVelocity -= 360;
					}
				}
				oldMoveRotaVelocity = adjustMoveRotaVelocity;

				//いーじんぐで
				rotation.y = Easing::OutCubic(easeBeforeRota.y, adjustMoveRotaVelocity, time);

				if (*lockonRotYStopTimer.get() >= stopTime + rotTime) {
					isLockonRotYStop = false;
				}
			}
			else {
				//回転をなるべく短くするために元角度を調整(例：350→10 より -10→10の方が近い)
				if (easeBeforeRota.y > moveRotaVelocity.y) {
					if (easeBeforeRota.y - moveRotaVelocity.y > 180) {
						easeBeforeRota.y -= 360;
					}
				}
				else if (easeBeforeRota.y < moveRotaVelocity.y) {
					if (moveRotaVelocity.y - easeBeforeRota.y > 180) {
						easeBeforeRota.y += 360;
					}
				}
			}
		}
		//開始時にイージングで動かすで動かす場合
		else if (*lockonChangeRotaTimer.get() <= lockonStartChangeRotaTime) {
			//回転をなるべく短くするために元角度を調整(例：350→10 より -10→10の方が近い)
			LockonAdjastEaseRotate(rotation.y, easeBeforeRota.y, moveRotaVelocity.y, lockonStartEaseTimer);
		}
		//それ以外ならロックオンする
		else {
			rotation.y = moveRotaVelocity.y;
		}
	}

	//x軸回転
	{
		//開始時にイージングで動かすで動かす場合
		if (*lockonChangeRotaTimer.get() <= lockonStartChangeRotaTime) {
			//回転をなるべく短くするために元角度を調整(例：350→10 より -10→10の方が近い)
			LockonAdjastEaseRotate(rotation.x, easeBeforeRota.x, moveRotaVelocity.x, lockonStartEaseTimer);
		}
		//それ以外ならロックオンする
		else {
			rotation.x = moveRotaVelocity.x;
		}
	}
}

void GameCamera::LockonAdjastEaseRotate(float& rotation, float easeBeforeRotate, float easeAfterRotate, float easeTime)
{
	//回転をなるべく短くするために元角度を調整(例：350→10 より -10→10の方が近い)
	float adjustRota = easeBeforeRotate;
	if (adjustRota > easeAfterRotate) {
		if (adjustRota - easeAfterRotate > 180) {
			adjustRota -= 360;
		}
	}
	else if (adjustRota < easeAfterRotate) {
		if (easeAfterRotate - adjustRota > 180) {
			adjustRota += 360;
		}
	}
	rotation = Easing::OutCubic(adjustRota, easeAfterRotate, easeTime);
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
	targetPositionsKeep.push_back(player->GetData()->pos);
	const int frame = 8;
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
		Vector2 targetPosXZ = { player->GetData()->pos.x, player->GetData()->pos.z };
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

	//入力を判定しない状態なら抜ける
	if (!actionInput.isLockon) { return; }
	//ロックオン入力がなければ抜ける
	if (!GameInputManager::TriggerInputAction(GameInputManager::Lockon)) { return; }

	//ロックオンしていない場合はロックオンターゲットを検出する状態にする
	if (!isLockon) {
		isLockonStart = true;
	}
	//ロックオン中ならロックオンを解除する
	else {
		LockonEnd(true);
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
