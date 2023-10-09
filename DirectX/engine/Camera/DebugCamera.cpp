#include "DebugCamera.h"
#include "input/DirectInput.h"
#include "WindowApp.h"

const float DebugCamera::eyeTargetDistance = 100.0f;

std::unique_ptr<DebugCamera> DebugCamera::Create(const XMFLOAT3& targetPos)
{
	// �C���X�^���X�𐶐�
	DebugCamera* instance = new DebugCamera(targetPos);
	if (instance == nullptr) {
		return nullptr;
	}

	//���j�[�N�|�C���^��Ԃ�
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

	//���{�^����������Ă�����J��������]������
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

	// �}�E�X�̒��{�^����������Ă�����J��������s�ړ�������
	if (input->PushMouseButton(DirectInput::MOUSE_WHEEL)) {
		float dx = input->GetMouseVelocity().x;
		float dy = input->GetMouseVelocity().y;

		XMVECTOR move = { -dx, +dy, 0, 0 };
		move = XMVector3Transform(move, matRot);

		MoveVector(move);
		dirty = true;
	}

	// �z�C�[�����͂ŋ�����ύX
	if (input->GetMouseWheelVelocity() != 0) {
		distance -= input->GetMouseWheelVelocity() * 5.0f;
		distance = max(distance, 1.0f);
		dirty = true;
	}

	if (dirty) {
		//angle�̍X�V�𔽉f
		UpdateAngle3d(angle);
	}
}

void DebugCamera::UpdateAngle3d(const XMFLOAT2& angle)
{
	//�ǉ���]���̉�]�s��𐶐�
	XMMATRIX matRotNew = DirectX::XMMatrixIdentity();
	matRotNew *= DirectX::XMMatrixRotationX(-angle.x);
	matRotNew *= DirectX::XMMatrixRotationY(-angle.y);
	matRot = matRotNew * matRot;

	//�����_���王�_�ւ̃x�N�g���ƁA������x�N�g��
	XMVECTOR vTargetEye = { 0.0f, 0.0f, -distance, 1.0f };
	XMVECTOR vUp = { 0.0f, 1.0f, 0.0f, 0.0f };

	//�x�N�g������]
	vTargetEye = XMVector3Transform(vTargetEye, matRot);
	vUp = XMVector3Transform(vUp, matRot);

	//�����_���炸�炵���ʒu�Ɏ��_���W������
	const XMFLOAT3& target = GetTarget();
	eye = { target.x + vTargetEye.m128_f32[0], target.y + vTargetEye.m128_f32[1], target.z + vTargetEye.m128_f32[2] };

	up = { vUp.m128_f32[0], vUp.m128_f32[1], vUp.m128_f32[2] };
}
