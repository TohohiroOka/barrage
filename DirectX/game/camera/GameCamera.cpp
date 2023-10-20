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
	// �C���X�^���X�𐶐�
	GameCamera* instance = new GameCamera();
	if (instance == nullptr) {
		return nullptr;
	}

	//���j�[�N�|�C���^��Ԃ�
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
	//���b�N�I�����͂��󂯕t����
	LockonInput();

	//�g�����X�t�H�[���X�V
	UpdateTransform();

	//�J�������W��]�p�𔽉f���čX�V
	//���s�ړ��s��̌v�Z
	const XMMATRIX matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//���[���h�s����X�V
	UpdateMatWorld(matTrans);
	//���_�A�����_���X�V
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
	//��]�@
	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	//�q�ł��鎩�@�p�̃��[���h�s��̍���
	matWorld = XMMatrixIdentity();	//�ό`�����Z�b�g
	matWorld *= matRot;		//���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrans;	//���[���h�s��ɕ��s�ړ��𔽉f
}

void GameCamera::UpdateEyeTarget()
{
	//���_�����[���h���W�ɐݒ�
	eye = { matWorld.r[3].m128_f32[0], matWorld.r[3].m128_f32[1], matWorld.r[3].m128_f32[2] };
	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//�J�����̉�]�𔽉f������
	forward = MatrixTransformDirection(forward, matWorld);
	//���_����O���ɐi�񂾈ʒu�𒍎��_�ɐݒ�
	target = eye + forward;
	//�V�n�����]���Ă������悤�ɏ�����x�N�g������]������
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
	//���E�ړ�
	const float Tgspeed = 1.0f;

	//�L�[����
	{
		if (input->PushKey(DIK_LEFT)) { rotation.y -= Tgspeed; }//�E����
		if (input->PushKey(DIK_RIGHT)) { rotation.y += Tgspeed; }//������
		if (input->PushKey(DIK_DOWN)) { rotation.x -= Tgspeed; }//������
		if (input->PushKey(DIK_UP)) { rotation.x += Tgspeed; }//�����
	}
	//�R���g���[���[����
	{
		//������x�X�e�B�b�N���X���Ȃ��Ɣ��肵�Ȃ�
		const float moveStickIncline = 0.3f;
		const XMFLOAT2 padIncline = XInputManager::GetInstance()->GetPadRStickIncline();
		if (fabsf(padIncline.x) >= moveStickIncline || fabsf(padIncline.y) >= moveStickIncline) {
			const XMFLOAT2 padIncline = XInputManager::GetInstance()->GetPadRStickIncline();
			const float stickRadian = XMConvertToRadians(XInputManager::GetInstance()->GetPadRStickAngle() - 90);
			rotation.y += cosf(stickRadian) * fabsf(padIncline.x) * Tgspeed;
			rotation.x += -sinf(stickRadian) * fabsf(padIncline.y) * Tgspeed;
		}
	}

	//�㉺�����̊p�x����
	rotation.x = max(rotation.x, -89);
	rotation.x = min(rotation.x, 89);

	//����]�̉�]�p��0�`360�ȓ��Ɏ��܂�悤�ɂ���
	if (rotation.y > 360) { rotation.y -= 360; }
	if (rotation.y < 0) { rotation.y += 360; }
}

void GameCamera::UpdateLockonRotate()
{
	//��]�p�ύX�ɂ����鎞��
	const float changeRotaTime = 15;
	
	//�v���C���[�ƃ��b�N�I���^�[�Q�b�g�̊p�x���擾(0�`360�ɒ���)
	float lockonRotate = -GameHelper::Instance()->GetAngle({ lockonTarget->GetPosition().x, lockonTarget->GetPosition().z }, { player->GetPosition().x, player->GetPosition().z }) - 90;
	while (lockonRotate < 0 || lockonRotate > 360) {
		//����]�̉�]�p��0�`360�ȓ��Ɏ��܂�悤�ɂ���
		if (lockonRotate > 360) { lockonRotate -= 360; }
		else if (lockonRotate < 0) { lockonRotate += 360; }
	}

	//�C�[�W���O�œ������ꍇ
	if (lockonChangeRotaTimer <= changeRotaTime) {
		lockonChangeRotaTimer++;

		//��]���Ȃ�ׂ��Z�����邽�߂Ɍ��p�x�𒲐�(��F350��10 ��� -10��10�̕����߂�)
		float adjustRota = easeBeforeRota.y;
		if (adjustRota > lockonRotate) {
			if (adjustRota - lockonRotate > 180) {
				adjustRota -= 360;
			}
		}

		//���b�N�I�������^�[�Q�b�g�̕������C�[�W���O�Ō����悤�ɂ���
		const float time = lockonChangeRotaTimer / changeRotaTime;
		rotation.y = Easing::OutCubic(adjustRota, lockonRotate, time);
	}
	else {
		//���b�N�I�������^�[�Q�b�g�̕���������
		rotation.y = lockonRotate;
	}
}

void GameCamera::UpdatePosition()
{
	//X,Y��]�p�����W�A���ɒ���
	const float angleX = XMConvertToRadians(rotation.x);
	const float angleY = XMConvertToRadians(rotation.y);
	//�A���_�[�t���[����\��������̂ŁA�����_��؂�̂Ă�
	const float divNum = 1000;
	const float floorAngleX = floorf(angleX * divNum) / divNum;
	const float floorAngleY = floorf(angleY * divNum) / divNum;

	//X,Y���W�A�����g�p���Asin,cos���Z�o
	const float sinfAngleY = sinf(floorAngleY);
	const float cosfAngleY = cosf(floorAngleY);
	const float sinfAngleX = sinf(floorAngleX);
	const float cosfAngleX = cosf(floorAngleX);

	//�v�Z���ʂ����蓖�Ăč��W���Z�b�g
	//Y���W��X��]�p��sin���g�p
	//X,Z���W��Y��]�p��sin,cos�Ōv�Z���AX��]�p(Y���W)��cos����Z���ĎZ�o
	Vector3 targetPos = player->GetPosition();
	position.x = (-sinfAngleY * cosfAngleX) * targetDistance + targetPos.x;
	position.y = sinfAngleX * targetDistance + targetPos.y;
	position.z = (-cosfAngleY * cosfAngleX) * targetDistance + targetPos.z;
}

void GameCamera::LockonInput()
{
	//���b�N�I���^�[�Q�b�g���o�t���O�𖈃t���[���߂��Ă���
	isLockonStart = false;

	//���b�N�I�����͂��Ȃ���Δ�����
	if (!(DirectInput::GetInstance()->TriggerKey(DIK_F1) || XInputManager::GetInstance()->TriggerButton(XInputManager::PAD_LEFT_STICK_PUSH))) { return; }

	//���b�N�I�����Ă��Ȃ��ꍇ�̓��b�N�I���^�[�Q�b�g�����o�����Ԃɂ���
	if (!isLockon) {
		isLockonStart = true;
	}
	//���b�N�I�����Ȃ烍�b�N�I������������
	else {
		lockonTarget = nullptr;
		isLockon = false;
	}
}
