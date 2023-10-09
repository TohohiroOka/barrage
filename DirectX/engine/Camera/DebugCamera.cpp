#include "DebugCamera.h"
#include "input/DirectInput.h"
#include "WindowApp.h"

const float DebugCamera::eyeTargetDistance = 100.0f;

std::unique_ptr<DebugCamera> DebugCamera::Create(const XMFLOAT3& targetPos)
{
	// インスタンスを生成
	DebugCamera* instance = new DebugCamera(targetPos);
	if (instance == nullptr) {
		return nullptr;
	}

	//ユニークポインタを返す
	return std::unique_ptr<DebugCamera>(instance);
}

DebugCamera::DebugCamera(const XMFLOAT3& targetPos) :
	Camera(true)
{
	target = targetPos;

	scale.x = 1.0f / (float)WindowApp::GetWindowHeight();
	scale.y = 1.0f / (float)WindowApp::GetWindowHeight();

	UpdateAngle3d({});
}

DebugCamera::~DebugCamera()
{
}

void DebugCamera::Update()
{
	MoveCamera();
	Camera::Update();
}

void DebugCamera::MoveCamera()
{
	ChangeTargetDistance();
}

void DebugCamera::ChangeTargetDistance()
{
	DirectInput* input = DirectInput::GetInstance();

	bool dirty = false;
	XMFLOAT2 angle = {};

	//左ボタンが押されていたらカメラを回転させる
	if (input->PushMouseButton(DirectInput::MOUSE_LEFT)) {
		float dy = input->GetMouseVelocity().x * scale.y;
		float dx = input->GetMouseVelocity().y * scale.x;
		if (fabsf(input->GetMouseVelocity().x) >= fabsf(input->GetMouseVelocity().y)) {
			angle.y = -dy * DirectX::XM_PI;
		}
		else {
			angle.x = -dx * DirectX::XM_PI;
		}
		dirty = true;
	}

	// マウスの中ボタンが押されていたらカメラを並行移動させる
	if (input->PushMouseButton(DirectInput::MOUSE_WHEEL)) {
		float dx = input->GetMouseVelocity().x;
		float dy = input->GetMouseVelocity().y;

		XMVECTOR move = { -dx, +dy, 0, 0 };
		move = XMVector3Transform(move, matRot);

		MoveVector(move);
		dirty = true;
	}

	// ホイール入力で距離を変更
	if (input->GetMouseWheelVelocity() != 0) {
		distance -= input->GetMouseWheelVelocity() * 5.0f;
		distance = max(distance, 1.0f);
		dirty = true;
	}

	if (dirty) {
		//angleの更新を反映
		UpdateAngle3d(angle);
	}
}

void DebugCamera::UpdateAngle3d(const XMFLOAT2& angle)
{
	//追加回転分の回転行列を生成
	XMMATRIX matRotNew = DirectX::XMMatrixIdentity();
	matRotNew *= DirectX::XMMatrixRotationX(-angle.x);
	matRotNew *= DirectX::XMMatrixRotationY(-angle.y);
	matRot = matRotNew * matRot;

	//注視点から視点へのベクトルと、上方向ベクトル
	XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
	XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };

	//ベクトルを回転
	vTargetEye = XMVector3Transform(vTargetEye, matRot);
	vUp = XMVector3Transform(vUp, matRot);

	//注視点からずらした位置に視点座標を決定
	const XMFLOAT3& target = GetTarget();
	eye = { target.x + vTargetEye.m128_f32[0], target.y + vTargetEye.m128_f32[1], target.z + vTargetEye.m128_f32[2] };

	up = { vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2] };
}
