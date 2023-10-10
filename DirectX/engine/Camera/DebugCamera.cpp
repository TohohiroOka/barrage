#include "DebugCamera.h"
#include "input/DirectInput.h"
#include "WindowApp.h"

const float DebugCamera::eyeTargetDistance = 100.0f;

std::unique_ptr<DebugCamera> DebugCamera::Create(const Vector3& targetPos)
{
	// �C���X�^���X�𐶐�
	DebugCamera* instance = new DebugCamera(targetPos);
	if (instance == nullptr) {
		return nullptr;
	}

	//���j�[�N�|�C���^��Ԃ�
	return std::unique_ptr<DebugCamera>(instance);
}

DebugCamera::DebugCamera(const Vector3& targetPos) :
	Camera(true)
{
	target = targetPos;

	scale.x = 1.0f / (float)WindowApp::GetWindowHeight();
	scale.y = 1.0f / (float)WindowApp::GetWindowHeight();

	pos = {};
	oka_moveRota = {};

	UpdateAngle3d({});
}

DebugCamera::~DebugCamera()
{
}

void DebugCamera::Update()
{
	Move1();
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

void DebugCamera::Move1()
{
	using namespace DirectX;

	DirectInput* input = DirectInput::GetInstance();

	const float Tgspeed = 1.0f;
	if (input->PushKey(DIK_LEFT)) { oka_moveRota.x -= Tgspeed; }//�E����
	if (input->PushKey(DIK_RIGHT)) { oka_moveRota.x += Tgspeed; }//������
	if (input->PushKey(DIK_DOWN)) { oka_moveRota.y -= Tgspeed; }//������
	if (input->PushKey(DIK_UP)) { oka_moveRota.y += Tgspeed; }//�����

	//�㉺�����̊p�x����
	if (oka_moveRota.y <= -40) { oka_moveRota.y = -40; }//������
	if (oka_moveRota.y >= 40) { oka_moveRota.y = 40; }//�㐧��

	//���̐���
	if (oka_moveRota.x > 360) { oka_moveRota.x = 0; }
	if (oka_moveRota.x < -360) { oka_moveRota.x = 0; }

	Vector2 raidan_LR = Vector2(XMConvertToRadians(oka_moveRota.x), XMConvertToRadians(oka_moveRota.x + 90));
	Vector2 raidan_UD = Vector2(XMConvertToRadians(360.0f - oka_moveRota.x), XMConvertToRadians(360.0f - oka_moveRota.x + 90));

	//player�ړ�
	float Pspeed = 5.0f;
	//�E����
	if (input->PushKey(DIK_D)) {
		pos.x += Pspeed * cosf(raidan_LR.x);
		pos.z += Pspeed * cosf(raidan_LR.y);
	}
	//������
	if (input->PushKey(DIK_A)) {
		pos.x -= Pspeed * cosf(raidan_LR.x);
		pos.z -= Pspeed * cosf(raidan_LR.y);
	}
	//�O����
	if (input->PushKey(DIK_W)) {
		pos.x += Pspeed * cosf(raidan_UD.y);
		pos.z += Pspeed * cosf(raidan_UD.x);
	}
	//�����
	if (input->PushKey(DIK_S)) {
		pos.x -= Pspeed * cosf(raidan_UD.y);
		pos.z -= Pspeed * cosf(raidan_UD.x);
	}
	//�����
	if (input->PushKey(DIK_LSHIFT)) {
		pos.y += Pspeed;
	}
	//������
	if (input->PushKey(DIK_LCONTROL)) {
		pos.y -= Pspeed;
	}

	//�J�����ړ�
	XMFLOAT2 radian = { XMConvertToRadians(oka_moveRota.x),XMConvertToRadians(oka_moveRota.y) };
	XMFLOAT2 radian2 = { XMConvertToRadians(oka_moveRota.x + 180),XMConvertToRadians(oka_moveRota.y + 180) };

	const float dist = 15.0f;
	eye = { pos.x + sinf(radian2.x) * dist,pos.y,pos.z + cosf(radian2.x) * dist };
	target = { pos.x + sinf(radian.x) * dist,pos.y + sinf(radian.y) * dist,pos.z + cosf(radian.x) * dist };

}
