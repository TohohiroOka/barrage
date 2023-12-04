#include "PlayerActionAttack.h"
#include "Player.h"
#include "Camera/GameCamera.h"
#include "system/GameInputManager.h"

using namespace DirectX;


PlayerActionAttack::PlayerActionAttack(Player* player)
	: PlayerActionBase(player)
{
	player->UseEndurance(player->GetData()->attackAction->GetUseEndranceNum(), 60, true); //���v�͂��g�p

	//�\�ߎ��̍s����ݒ肵�Ă���(�I����͒ʏ�ړ�)
	nextAction = PlayerActionName::MOVENORMAL;
}

PlayerActionAttack::~PlayerActionAttack()
{
}

void PlayerActionAttack::Update()
{
	//���t���[��������
	player->GetData()->isMoveKey = false;
	player->GetData()->isMovePad = false;

	//�U���̕�����ύX
	ChangeRotate();

	//���̍U�������
	NextAttack();

	//�U���̓��e���X�V
	player->GetData()->attackAction->Update();
	//�U���̍s�����I��������U���s���I��
	if (player->GetData()->attackAction->GetIsAttackActionEnd()) {
		isActionEnd = true;

		//�\�񂵂Ă������̍s�����Z�b�g
		player->GetData()->action = nextAction;
	}

	//���ł�����ōU���𒆒f�ł���
	if (AvoidStart()) {
		isActionEnd = true;

		//����̃x�N�g����ݒ�
		player->GetData()->avoidBlinkMoveVec = player->GetData()->moveVec;

		//���̍s��������ɃZ�b�g
		nextAction = PlayerActionName::AVOID;
		player->GetData()->action = nextAction;
	}
}

void PlayerActionAttack::ChangeRotate()
{
	DirectInput* input = DirectInput::GetInstance();

	//�L�[���͂𔻒�
	player->GetData()->isMoveKey = (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveRight).key) ||
		input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveLeft).key) ||
		input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveForward).key) ||
		input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveBack).key));

	//������x�X�e�B�b�N���X���Ȃ��ƃp�b�h���͔��肵�Ȃ�
	const XMFLOAT2 padIncline = GameInputManager::GetPadLStickIncline();
	player->GetData()->isMovePad = (fabsf(padIncline.x) >= GameInputManager::GetPadStickInputIncline() || fabsf(padIncline.y) >= GameInputManager::GetPadStickInputIncline());


	//���͔��肪�Ȃ���Δ�����
	if (!(player->GetData()->isMoveKey || player->GetData()->isMovePad)) { return; }

	Vector3 inputMoveVec{};

	if (player->GetData()->isMoveKey) {
		if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveRight).key)) {
			inputMoveVec.x = 1;
		}
		if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveLeft).key)) {
			inputMoveVec.x = -1;
		}
		if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveForward).key)) {
			inputMoveVec.z = 1;
		}
		if (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveBack).key)) {
			inputMoveVec.z = -1;
		}
	}
	if (player->GetData()->isMovePad) {
		//�p�b�h�X�e�B�b�N�̕������x�N�g����
		inputMoveVec.x = GameInputManager::GetPadLStickIncline().x;
		inputMoveVec.z = GameInputManager::GetPadLStickIncline().y;
	}

	//�x�N�g�����J�����̌X���ŉ�]������
	inputMoveVec.normalize();
	const float cameraRotaRadian = XMConvertToRadians(-player->GetGameCamera()->GetCameraRota().y);
	player->GetData()->moveVec.x = inputMoveVec.x * cosf(cameraRotaRadian) - inputMoveVec.z * sinf(cameraRotaRadian);
	player->GetData()->moveVec.z = inputMoveVec.x * sinf(cameraRotaRadian) + inputMoveVec.z * cosf(cameraRotaRadian);

	//�i�s�����������悤�ɂ���
	const Vector3 moveRotaVelocity = { player->GetData()->moveVec.x, 0, player->GetData()->moveVec.z }; //�v���C���[��]�ɃW�����v�͊֌W�Ȃ��̂ŁA���xY��0�ɂ��Ă���
	player->SetMoveRotate(moveRotaVelocity, 5.0f);
}

void PlayerActionAttack::NextAttack()
{
	//���̍s������͉\�łȂ���Δ�����
	if (!player->GetData()->attackAction->GetIsNextActionInput()) { return; }

	//�A���ōU�����Z�b�g
	if (GameInputManager::TriggerInputAction(GameInputManager::Attack)) {
		if (!player->GetData()->attackAction->NextAttack()) { return; }

		player->UseEndurance(player->GetData()->attackAction->GetUseEndranceNum(), 60, true); //���v�͂��g�p

		//�\�ߎ��̍s����ݒ肵�Ă���(�I����͒ʏ�ړ�)
		nextAction = PlayerActionName::MOVENORMAL;
	}
	//�W�����v�̐�s���͂��󂯕t����
	else if (JumpStart()) {
		nextAction = PlayerActionName::JUMP;
	}
}
