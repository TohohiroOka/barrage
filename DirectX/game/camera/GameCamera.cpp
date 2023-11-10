#include "GameCamera.h"
#include "system/GameInputManager.h"
#include "WindowApp.h"
#include "GameHelper.h"
#include "Math/Easing/Easing.h"

using namespace DirectX;

Player* GameCamera::player = nullptr;
const float GameCamera::rotateXMin = -45;
const float GameCamera::rotateXMax = 70;
const float GameCamera::rotateXBase = 7;
const float GameCamera::rotateCenterDistanceNormal = 15.0f;
const Vector3 GameCamera::targetDistanceNormal = { 0, 0, 0 };

GameCamera::GameCamera() :
	Camera(true)
{
	//��]�̒��S�Ƃ̋������Z�b�g
	rotateCenterDistance = rotateCenterDistanceNormal;
	//�^�[�Q�b�g�̒��S���炸�炷�������Z�b�g
	targetDistance = targetDistanceNormal;

	//�����̉�]�p���Z�b�g
	rotation.x = rotateXBase;

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
	isLockonEndRotate = false;

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
		if (isLockonEndRotate) {
			LockonEndRotate();
		}
		else {
			UpdateRotate();
		}
	}
	else {
		UpdateLockonRotate();
	}
	UpdatePosition();

	PositionCollision();
}

void GameCamera::UpdateRotate()
{
	DirectInput* input = DirectInput::GetInstance();
	//���E�ړ�
	const float rotSpeed = 2.0f;
	XMFLOAT2 rotNum = { 0,0 };//��]��

	//�L�[����
	{
		if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::CameraLeftRota).key)) { rotNum.y -= rotSpeed; }//�E����
		if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::CameraRightRota).key)) { rotNum.y += rotSpeed; }//������
		if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::CameraDownRota).key)) { rotNum.x += rotSpeed; }//������
		if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::CameraUpRota).key)) { rotNum.x -= rotSpeed; }//�����
	}
	//�R���g���[���[����
	{
		//������x�X�e�B�b�N���X���Ȃ��Ɣ��肵�Ȃ�
		const float moveStickIncline = 0.3f;
		const XMFLOAT2 padIncline = XInputManager::GetInstance()->GetPadRStickIncline();
		if (fabsf(padIncline.x) >= moveStickIncline || fabsf(padIncline.y) >= moveStickIncline) {
			const XMFLOAT2 padIncline = GameInputManager::GetPadRStickIncline();
			const float stickRadian = GameInputManager::GetPadRStickRadian();
			rotNum.y += cosf(stickRadian) * fabsf(padIncline.x) * rotSpeed;
			rotNum.x += sinf(stickRadian) * fabsf(padIncline.y) * rotSpeed;
		}
	}

	//���͐ݒ肪���o�[�X���[�h�Ȃ��]�ʂ��t�ɂ���
	if (GameInputManager::GetIsCameraRotaYReverse()) { rotNum.y = -rotNum.y; }
	if (GameInputManager::GetIsCameraRotaXReverse()) { rotNum.x = -rotNum.x; }

	//��]�ʂ����Z���ĉ�]������
	rotation.y += rotNum.y;
	rotation.x += rotNum.x;

	//�㉺�����̊p�x����
	rotation.x = max(rotation.x, rotateXMin);
	rotation.x = min(rotation.x, rotateXMax);

	//����]�̉�]�p��0�`360�ȓ��Ɏ��܂�悤�ɂ���
	if (rotation.y > 360) { rotation.y -= 360; }
	if (rotation.y < 0) { rotation.y += 360; }
}

void GameCamera::UpdateLockonRotate()
{
	//��]�p�ύX�ɂ����鎞��
	const float changeRotaTime = 15;

	//�v���C���[�ƃ��b�N�I���^�[�Q�b�g�̊p�x���擾(0�`360�ɒ���)
	Vector3 moveRotaVelocity = VelocityRotate(((Vector3)lockonTarget->GetPosition() - targetDistance) - (player->GetPosition() + targetDistance));
	while (moveRotaVelocity.y < 0 || moveRotaVelocity.y > 360) {
		//����]�̉�]�p��0�`360�ȓ��Ɏ��܂�悤�ɂ���
		if (moveRotaVelocity.y > 360) { moveRotaVelocity.y -= 360; }
		else if (moveRotaVelocity.y < 0) { moveRotaVelocity.y += 360; }
	}
	while (moveRotaVelocity.x < 0 || moveRotaVelocity.x > 360) {
		//�c��]�̉�]�p��0�`360�ȓ��Ɏ��܂�悤�ɂ���
		if (moveRotaVelocity.x > 360) { moveRotaVelocity.x -= 360; }
		else if (moveRotaVelocity.x < 0) { moveRotaVelocity.x += 360; }
	}

	//�C�[�W���O�œ������ꍇ
	if (*lockonChangeRotaTimer.get() <= changeRotaTime) {
		lockonChangeRotaTimer->Update();

		//��]���Ȃ�ׂ��Z�����邽�߂Ɍ��p�x�𒲐�(��F350��10 ��� -10��10�̕����߂�)
		Vector3 adjustRota = easeBeforeRota;
		if (adjustRota.y > moveRotaVelocity.y) {
			if (adjustRota.y - moveRotaVelocity.y > 180) {
				adjustRota.y -= 360;
			}
		}
		else if (adjustRota.y < moveRotaVelocity.y) {
			if (moveRotaVelocity.y - adjustRota.y > 180) {
				adjustRota.y += 360;
			}
		}
		if (adjustRota.x > moveRotaVelocity.x) {
			if (adjustRota.x - moveRotaVelocity.x > 180) {
				adjustRota.x -= 360;
			}
		}
		else if (adjustRota.x < moveRotaVelocity.x) {
			if (moveRotaVelocity.x - adjustRota.x > 180) {
				adjustRota.x += 360;
			}
		}

		//���b�N�I�������^�[�Q�b�g�̕������C�[�W���O�Ō����悤�ɂ���
		const float time = *lockonChangeRotaTimer.get() / changeRotaTime;
		rotation.y = Easing::OutCubic(adjustRota.y, moveRotaVelocity.y, time);
		rotation.x = Easing::OutCubic(adjustRota.x, moveRotaVelocity.x, time);
	}
	else {
		//���b�N�I�������^�[�Q�b�g�̕���������
		rotation = moveRotaVelocity;
	}
}

void GameCamera::UpdatePosition()
{
	//X,Y��]�p�����W�A���ɒ���
	Vector2 radian;
	radian.x = XMConvertToRadians(rotation.x);
	radian.y = XMConvertToRadians(rotation.y);
	//�A���_�[�t���[����\��������̂ŁA�����_��؂�̂Ă�
	const float divNum = 1000;
	radian.x = floorf(radian.x * divNum) / divNum;
	radian.y = floorf(radian.y * divNum) / divNum;

	//�^�[�Q�b�g�ւ̒Ǐ]���w�肵���t���[���̕��x�点��
	targetPositionsKeep.push_back(player->GetPosition());
	const int frame = 3;
	if (targetPositionsKeep.size() > frame) {
		targetPositionsKeep.pop_front();
	}

	//�v�Z���ʂ����蓖�Ăč��W���Z�b�g
	//Y���W��X��]�p��sin���g�p
	//X,Z���W��Y��]�p��sin,cos�Ōv�Z���AX��]�p(Y���W)��cos����Z���ĎZ�o
	Vector3 targetPos = targetPositionsKeep.front() + targetDistance;
	position.x = (-sinf(radian.y) * cosf(radian.x)) * rotateCenterDistance + targetPos.x;
	position.y = sinf(radian.x) * rotateCenterDistance + targetPos.y;
	position.z = (-cosf(radian.y) * cosf(radian.x)) * rotateCenterDistance + targetPos.z;
}

void GameCamera::PositionCollision()
{
	//�n�ʂ̂߂荞�݂���������
	const float groundHeight = 1; //���u���n�ʂ̍���
	const float collisionHeight = groundHeight + 0.1f; //�����߂����J�n���鍂��
	if (position.y < collisionHeight) {
		//�߂荞�ݗʂ��Z�o
		const float sinkNum = collisionHeight - position.y;

		//���������߂�
		position.y += sinkNum;

		//�����߂��̕��AX,Z�̓����ʈړ�������
		Vector2 posXZ = { position.x, position.z };
		Vector2 targetPosXZ = { player->GetPosition().x, player->GetPosition().z };
		Vector2 vec = targetPosXZ - posXZ;
		vec.normalize();

		position.x += vec.x * sinkNum;
		position.z += vec.y * sinkNum;
	}
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

		//���b�N�I���������ɉ�]�p��߂���
		const float rotateLine = 5.0f; //��]�p���C�[�W���O�œ������������肷�鍷��
		if (rotation.x > rotateLine || rotation.x < -rotateLine) {
			easeBeforeRota = rotation;
			lockonChangeRotaTimer->Reset();

			isLockonEndRotate = true;
		}
	}
}

void GameCamera::LockonEndRotate()
{
	//��]�p�ύX�ɂ����鎞��
	const float changeRotaTime = 15;
	lockonChangeRotaTimer->Update();
	const float time = *lockonChangeRotaTimer.get() / changeRotaTime;

	//��]���Ȃ�ׂ��Z�����邽�߂Ɍ��p�x�𒲐�(��F350��10 ��� -10��10�̕����߂�)
	Vector3 adjustRota = easeBeforeRota;
	if (adjustRota.x > 180) {
		adjustRota.x -= 360;
	}

	//�����̍����ɃJ�������C�[�W���O�œ�����
	rotation.x = Easing::OutCubic(adjustRota.x, rotateXBase, time);

	//�^�C�}�[���w�肵�����ԂɂȂ�����I��
	if (*lockonChangeRotaTimer.get() > changeRotaTime) {
		isLockonEndRotate = false;
	}
}
