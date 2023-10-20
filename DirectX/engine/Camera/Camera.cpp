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
	//�V�F�C�N��ԂȂ�J�������V�F�C�N������
	if (isShake) {
		Shake();
	}

	matView = XMMatrixLookAtLH(XMLoadFloat3(&eye), XMLoadFloat3(&target), XMLoadFloat3(&up));
}

void Camera::ShakeStart(const float _shakePower, const float _shakeTime)
{
	//�V�F�C�N�^�C�}�[�����Z�b�g
	shakeTimer = 0;
	//�V�F�C�N���鎞�Ԃ��Z�b�g
	this->shakeTime = _shakeTime;
	//�V�F�C�N�ő�̋������Z�b�g
	this->maxShakePower = _shakePower;
	//�V�F�C�N��Ԃɂ���
	isShake = true;
}

void Camera::Shake()
{
	//�^�C�}�[���J�E���g
	shakeTimer++;
	const float time = shakeTimer / shakeTime;

	//�V�F�C�N����l���v�Z
	const float randShake = maxShakePower * (1 - time);
	Vector3 shake{};

	//�[�����Z������邽�߂�0�̏ꍇ�̓����_���𐶐����Ȃ�
	if (!(randShake == 0)) {
		shake.x = RandomFloat(randShake) - randShake / 2.0f;
		shake.y = RandomFloat(randShake) - randShake / 2.0f;
	}

	//�l���傫���̂Ŋ���Z���ď���������
	const float div = 100;
	shake /= div;

	//�J�����ɃV�F�C�N�̒l�����Z
	eye += shake;

	//�V�F�C�N������������V�F�C�N��Ԃ�����
	if (shakeTimer >= shakeTime) {
		isShake = false;
	}
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