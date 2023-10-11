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
	Vector3 inoutEye = { ShakeDifference.x + eye.x,ShakeDifference.y + eye.y,ShakeDifference.z + eye.z };

	matView = XMMatrixLookAtLH(XMLoadFloat3(&inoutEye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void Camera::StartCameraShake(int _strength)
{
	float x = float(GameHelper::Instance()->RandomInt(_strength * 2)) - float(_strength);
	float y = float(GameHelper::Instance()->RandomInt(_strength * 2)) - float(_strength);

	ShakeDifference = { x ,y,0.0f };
}

void Camera::EndCameraShake()
{
	ShakeDifference = { 0.0f,0.0f,0.0f };
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