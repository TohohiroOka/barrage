#include "GameCamera.h"
#include "Input/DirectInput.h"
#include "Input/XInputManager.h"
#include "WindowApp.h"
#include "GameHelper.h"
#include "Math/Easing/Easing.h"

using namespace DirectX;

Player* GameCamera::player = nullptr;
const float GameCamera::baseDistance = 25.0f;

std::unique_ptr<GameCamera> GameCamera::Create()
{
	// インスタンスを生成
	GameCamera* instance = new GameCamera();
	if (instance == nullptr) {
		return nullptr;
	}

	//ユニークポインタを返す
	return std::unique_ptr<GameCamera>(instance);
}

GameCamera::GameCamera() :
	Camera(true)
{
	targetDistance = baseDistance;
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
	lockonChangeRotaTimer = 0;
	
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
		UpdateRotate();
	}
	else {
		UpdateLockonRotate();
	}
	UpdatePosition();
}

void GameCamera::UpdateRotate()
{
	DirectInput* input = DirectInput::GetInstance();
	//視界移動
	const float Tgspeed = 1.0f;

	//キー入力
	{
		if (input->PushKey(DIK_LEFT)) { rotation.y -= Tgspeed; }//右入力
		if (input->PushKey(DIK_RIGHT)) { rotation.y += Tgspeed; }//左入力
		if (input->PushKey(DIK_DOWN)) { rotation.x -= Tgspeed; }//下入力
		if (input->PushKey(DIK_UP)) { rotation.x += Tgspeed; }//上入力
	}
	//コントローラー入力
	{
		//ある程度スティックを傾けないと判定しない
		const float moveStickIncline = 0.3f;
		const XMFLOAT2 padIncline = XInputManager::GetInstance()->GetPadRStickIncline();
		if (fabsf(padIncline.x) >= moveStickIncline || fabsf(padIncline.y) >= moveStickIncline) {
			const XMFLOAT2 padIncline = XInputManager::GetInstance()->GetPadRStickIncline();
			const float stickRadian = XMConvertToRadians(XInputManager::GetInstance()->GetPadRStickAngle() - 90);
			rotation.y += cosf(stickRadian) * fabsf(padIncline.x) * Tgspeed;
			rotation.x += -sinf(stickRadian) * fabsf(padIncline.y) * Tgspeed;
		}
	}

	//上下方向の角度制限
	rotation.x = max(rotation.x, -89);
	rotation.x = min(rotation.x, 89);

	//横回転の回転角を0～360以内に収まるようにする
	if (rotation.y > 360) { rotation.y -= 360; }
	if (rotation.y < 0) { rotation.y += 360; }
}

void GameCamera::UpdateLockonRotate()
{
	//回転角変更にかかる時間
	const float changeRotaTime = 15;
	
	//プレイヤーとロックオンターゲットの角度を取得(0～360に調整)
	float lockonRotate = -GameHelper::Instance()->GetAngle({ lockonTarget->GetPosition().x, lockonTarget->GetPosition().z }, { player->GetPosition().x, player->GetPosition().z }) - 90;
	while (lockonRotate < 0 || lockonRotate > 360) {
		//横回転の回転角を0～360以内に収まるようにする
		if (lockonRotate > 360) { lockonRotate -= 360; }
		else if (lockonRotate < 0) { lockonRotate += 360; }
	}

	//イージングで動かす場合
	if (lockonChangeRotaTimer <= changeRotaTime) {
		lockonChangeRotaTimer++;

		//回転をなるべく短くするために元角度を調整(例：350→10 より -10→10の方が近い)
		float adjustRota = easeBeforeRota.y;
		if (adjustRota > lockonRotate) {
			if (adjustRota - lockonRotate > 180) {
				adjustRota -= 360;
			}
		}

		//ロックオンしたターゲットの方向をイージングで向くようにする
		const float time = lockonChangeRotaTimer / changeRotaTime;
		rotation.y = Easing::OutCubic(adjustRota, lockonRotate, time);
	}
	else {
		//ロックオンしたターゲットの方向を向く
		rotation.y = lockonRotate;
	}
}

void GameCamera::UpdatePosition()
{
	//X,Y回転角をラジアンに直す
	const float angleX = XMConvertToRadians(rotation.x);
	const float angleY = XMConvertToRadians(rotation.y);
	//アンダーフローする可能性があるので、小数点を切り捨てる
	const float divNum = 1000;
	const float floorAngleX = floorf(angleX * divNum) / divNum;
	const float floorAngleY = floorf(angleY * divNum) / divNum;

	//X,Yラジアンを使用し、sin,cosを算出
	const float sinfAngleY = sinf(floorAngleY);
	const float cosfAngleY = cosf(floorAngleY);
	const float sinfAngleX = sinf(floorAngleX);
	const float cosfAngleX = cosf(floorAngleX);

	//計算結果を割り当てて座標をセット
	//Y座標はX回転角のsinを使用
	//X,Z座標はY回転角のsin,cosで計算し、X回転角(Y座標)のcosを乗算して算出
	Vector3 targetPos = player->GetPosition();
	position.x = (-sinfAngleY * cosfAngleX) * targetDistance + targetPos.x;
	position.y = sinfAngleX * targetDistance + targetPos.y;
	position.z = (-cosfAngleY * cosfAngleX) * targetDistance + targetPos.z;
}

void GameCamera::LockonInput()
{
	//ロックオンターゲット検出フラグを毎フレーム戻しておく
	isLockonStart = false;

	//ロックオン入力がなければ抜ける
	if (!(DirectInput::GetInstance()->TriggerKey(DIK_F1) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_LEFT_STICK_PUSH))) { return; }

	//ロックオンしていない場合はロックオンターゲットを検出する状態にする
	if (!isLockon) {
		isLockonStart = true;
	}
	//ロックオン中ならロックオンを解除する
	else {
		lockonTarget = nullptr;
		isLockon = false;
	}
}
