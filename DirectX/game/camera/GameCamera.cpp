#include "GameCamera.h"
#include "system/GameInputManager.h"
#include "WindowApp.h"
#include "GameHelper.h"
#include "Math/Easing/Easing.h"

using namespace DirectX;

Player* GameCamera::player = nullptr;
const float GameCamera::baseDistance = 25.0f;

GameCamera::GameCamera() :
	Camera(true)
{
	targetDistance = baseDistance;

	//�^�C�}�[�N���X
	lockonChangeRotaTimer = std::make_unique<Engine::Timer>();
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
	lockonChangeRotaTimer->Reset();

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
	const float Tgspeed = 2.0f;
	XMFLOAT2 rotNum = { 0,0 };//��]��

	//�L�[����
	{
		if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::CameraLeftRota).key)) { rotNum.y -= Tgspeed; }//�E����
		if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::CameraRightRota).key)) { rotNum.y += Tgspeed; }//������
		if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::CameraDownRota).key)) { rotNum.x += Tgspeed; }//������
		if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::CameraUpRota).key)) { rotNum.x -= Tgspeed; }//�����
	}
	//�R���g���[���[����
	{
		//������x�X�e�B�b�N���X���Ȃ��Ɣ��肵�Ȃ�
		const float moveStickIncline = 0.3f;
		const XMFLOAT2 padIncline = XInputManager::GetInstance()->GetPadRStickIncline();
		if (fabsf(padIncline.x) >= moveStickIncline || fabsf(padIncline.y) >= moveStickIncline) {
			const XMFLOAT2 padIncline = GameInputManager::GetPadRStickIncline();
			const float stickRadian = GameInputManager::GetPadRStickRadian();
			rotNum.y += cosf(stickRadian) * fabsf(padIncline.x) * Tgspeed;
			rotNum.x += sinf(stickRadian) * fabsf(padIncline.y) * Tgspeed;
		}
	}

	//���͐ݒ肪���o�[�X���[�h�Ȃ��]�ʂ��t�ɂ���
	if (GameInputManager::GetIsCameraRotaYReverse()) { rotNum.y = -rotNum.y; }
	if (GameInputManager::GetIsCameraRotaXReverse()) { rotNum.x = -rotNum.x; }

	//��]�ʂ����Z���ĉ�]������
	rotation.y += rotNum.y;
	rotation.x += rotNum.x;

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
	float lockonRotate = -GetAngle({ lockonTarget->GetPosition().x, lockonTarget->GetPosition().z }, { player->GetPosition().x, player->GetPosition().z }) - 90;
	while (lockonRotate < 0 || lockonRotate > 360) {
		//����]�̉�]�p��0�`360�ȓ��Ɏ��܂�悤�ɂ���
		if (lockonRotate > 360) { lockonRotate -= 360; }
		else if (lockonRotate < 0) { lockonRotate += 360; }
	}

	//�C�[�W���O�œ������ꍇ
	if (*lockonChangeRotaTimer.get() <= changeRotaTime) {
		lockonChangeRotaTimer->Update();

		//��]���Ȃ�ׂ��Z�����邽�߂Ɍ��p�x�𒲐�(��F350��10 ��� -10��10�̕����߂�)
		float adjustRota = easeBeforeRota.y;
		if (adjustRota > lockonRotate) {
			if (adjustRota - lockonRotate > 180) {
				adjustRota -= 360;
			}
		}
		else if (adjustRota < lockonRotate) {
			if (lockonRotate - adjustRota > 180) {
				adjustRota += 360;
			}
		}

		//���b�N�I�������^�[�Q�b�g�̕������C�[�W���O�Ō����悤�ɂ���
		const float time = *lockonChangeRotaTimer.get() / changeRotaTime;
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
	if (!GameInputManager::TriggerInputAction(GameInputManager::Lockon)) { return; }

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
