#include "Camera.h"
#include "WindowApp.h"
#include "GameHelper.h"

using namespace DirectX;

Camera::Camera(const bool _mode)
{
	const XMFLOAT2 window = { float(WindowApp::GetWindowWidth()) , float(WindowApp::GetWindowHeight()) };
	aspectRatio = window.x / window.y;

	if (_mode) {
		 //射影行列の計算
		matProjection = XMMatrixPerspectiveFovLH(
			XMConvertToRadians(60.0f),
			aspectRatio,
			0.1f, 1200.0f//奥行/手前,最奥
		);
	} else {
		// 射影行列計算
		matProjection = XMMatrixOrthographicOffCenterLH(
			-window.x / 6.0f, window.x / 6.0f,
			-window.y / 6.0f, window.y / 6.0f,
			0.1f, 1200.0f);
	}
}

std::unique_ptr<Camera> Camera::Create(const bool _mode)
{
	// Spriteのインスタンスを生成
	Camera* instance = new Camera(_mode);
	if (instance == nullptr) {
		return nullptr;
	}

	//ユニークポインタを返す
	return std::unique_ptr<Camera>(instance);
}

void Camera::Update()
{
	//シェイク状態ならカメラをシェイクさせる
	if (isShake) {
		Shake();
	}

	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void Camera::ShakeStart(const float _shakePower, const float _shakeTime)
{
	//シェイクタイマーをリセット
	shakeTimer = 0;
	//シェイクする時間をセット
	this->shakeTime = _shakeTime;
	//シェイク最大の強さをセット
	this->maxShakePower = _shakePower;
	//シェイク状態にする
	isShake = true;
}

void Camera::Shake()
{
	//タイマーをカウント
	shakeTimer++;
	const float time = shakeTimer / shakeTime;

	//シェイクする値を計算
	const float randShake = maxShakePower * (1 - time);
	Vector3 shake{};

	//ゼロ除算を避けるために0の場合はランダムを生成しない
	if (!(randShake == 0)) {
		shake.x = RandomFloat(randShake) - randShake / 2.0f;
		shake.y = RandomFloat(randShake) - randShake / 2.0f;
	}

	//値が大きいので割り算して小さくする
	const float div = 100;
	shake /= div;

	//カメラにシェイクの値を加算
	eye += shake;

	//シェイクが完了したらシェイク状態を解除
	if (shakeTimer >= shakeTime) {
		isShake = false;
	}
}

void Camera::SetMatProjection(float _back)
{
	aspectRatio = float(WindowApp::GetWindowWidth() / WindowApp::GetWindowHeight());

	// 射影行列の計算
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		aspectRatio,
		0.1f, _back//奥行/手前,最奥
	);
}

void Camera::MoveVector(const XMVECTOR& move)
{
	// 視点と注視点座標を移動し、反映
	Vector3 eye_moved = GetEye();
	Vector3 target_moved = GetTarget();

	eye_moved.x += move.m128_f32[0];
	eye_moved.y += move.m128_f32[1];
	eye_moved.z += move.m128_f32[2];

	target_moved.x += move.m128_f32[0];
	target_moved.y += move.m128_f32[1];
	target_moved.z += move.m128_f32[2];

	SetEye(eye_moved);
	SetTarget(target_moved);
}