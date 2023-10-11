#include "Camera.h"
#include "WindowApp.h"
#include "GameHelper.h"

using namespace DirectX;

Camera::Camera(const bool _mode)
{
	const XMFLOAT2 window = { float(WindowApp::GetWindowWidth()) , float(WindowApp::GetWindowHeight()) };
	aspectRatio = window.x / window.y;

	if (_mode) {
		 //�ˉe�s��̌v�Z
		matProjection = XMMatrixPerspectiveFovLH(
			XMConvertToRadians(60.0f),
			aspectRatio,
			0.1f, 1200.0f//���s/��O,�ŉ�
		);
	} else {
		// �ˉe�s��v�Z
		matProjection = XMMatrixOrthographicOffCenterLH(
			-window.x / 6.0f, window.x / 6.0f,
			-window.y / 6.0f, window.y / 6.0f,
			0.1f, 1200.0f);
	}
}

std::unique_ptr<Camera> Camera::Create(const bool _mode)
{
	// Sprite�̃C���X�^���X�𐶐�
	Camera* instance = new Camera(_mode);
	if (instance == nullptr) {
		return nullptr;
	}

	//���j�[�N�|�C���^��Ԃ�
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

	// �ˉe�s��̌v�Z
	matProjection = XMMatrixPerspectiveFovLH(
		XMConvertToRadians(60.0f),
		aspectRatio,
		0.1f, _back//���s/��O,�ŉ�
	);
}

void Camera::MoveVector(const XMVECTOR& move)
{
	// ���_�ƒ����_���W���ړ����A���f
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