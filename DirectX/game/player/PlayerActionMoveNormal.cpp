#include "PlayerActionMoveNormal.h"
#include "Player.h"
#include "Camera/GameCamera.h"
#include "system/GameInputManager.h"
#include "GameHelper.h"

using namespace DirectX;

float PlayerActionMoveNormal::moveSpeedMax = 0.6f;
float PlayerActionMoveNormal::dashSpeedMax = 1.0f;

PlayerActionMoveNormal::PlayerActionMoveNormal(Player* player)
	: PlayerActionBase(player)
{
	//�W�����v�A�j���[�V�����ȊO�Ȃ�ҋ@�A�j���[�V�����ɕύX
	if (player->GetFbxObject()->GetUseAnimation() == PlayerAnimationName::JUMP_ANIMATION) { return; }
	player->GetFbxObject()->AnimationReset();
	player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::STAY_ANIMATION);
}

PlayerActionMoveNormal::~PlayerActionMoveNormal()
{
}

void PlayerActionMoveNormal::Update()
{
	//���t���[��������
	player->GetData()->isMoveKey = false;
	player->GetData()->isMovePad = false;

	//�ړ�
	Move();

	//���̍s�����J�n�\������
	NextActionStart();
}

void PlayerActionMoveNormal::Move()
{
	DirectInput* input = DirectInput::GetInstance();

	//�ړ��L�[���͂𔻒�
	player->GetData()->isMoveKey = player->GetData()->actionInput.isMove && (input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveRight).key) ||
		input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveLeft).key) ||
		input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveForward).key) ||
		input->PushKey(GameInputManager::GetKeyInputActionData(GameInputManager::MoveBack).key));

	//������x�X�e�B�b�N���X���Ȃ��ƈړ��p�b�h���͔��肵�Ȃ�
	const XMFLOAT2 padIncline = GameInputManager::GetPadLStickIncline();
	player->GetData()->isMovePad = player->GetData()->actionInput.isMove && (fabsf(padIncline.x) >= GameInputManager::GetPadStickInputIncline() || fabsf(padIncline.y) >= GameInputManager::GetPadStickInputIncline());

	//�_�b�V��
	Dash();

	//���͂������
	const float moveAccel = 0.05f * GameHelper::Instance()->GetGameSpeed();
	if (player->GetData()->isMoveKey || player->GetData()->isMovePad) {
		player->GetData()->moveSpeed += moveAccel;
		if (isDash) { player->GetData()->moveSpeed = min(player->GetData()->moveSpeed, dashSpeedMax); }
		else { player->GetData()->moveSpeed = min(player->GetData()->moveSpeed, moveSpeedMax); }

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
		player->SetMoveRotate(moveRotaVelocity, 15.0f);

		//�ҋ@�A�j���[�V�����̂Ƃ��̂ݑ���A�j���[�V�������J�n����(���݂͑ҋ@�A�j���[�V�������Ȃ��̂ő�p��)
		if (player->GetFbxObject()->GetUseAnimation() == PlayerAnimationName::STAY_ANIMATION) {
			player->GetFbxObject()->AnimationReset();
			player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::RUN_ANIMATION);
		}
	}
	else {
		player->GetData()->moveSpeed -= moveAccel * 2;
		player->GetData()->moveSpeed = max(player->GetData()->moveSpeed, 0);

		//�X�s�[�h���Ȃ��Ȃ�����ҋ@�A�j���[�V�����ɕύX(�݌v��)
		if (player->GetFbxObject()->GetUseAnimation() == PlayerAnimationName::RUN_ANIMATION && player->GetData()->moveSpeed <= 0) {
			player->GetFbxObject()->AnimationReset();
			player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::STAY_ANIMATION);
		}
	}

	//���x���Z�b�g
	player->GetData()->velocity.x = player->GetData()->moveVec.x * player->GetData()->moveSpeed;
	player->GetData()->velocity.z = player->GetData()->moveVec.z * player->GetData()->moveSpeed;
}

void PlayerActionMoveNormal::Dash()
{
	//�n�ʂɂ��Ȃ��ꍇ�́A�ύX���󂯕t���Ȃ��Ŕ�����
	if (!player->GetData()->onGround) { return; }

	if (!isDash) {
		//�_�b�V���J�n�\�Œn�ʐڒn���Ă��邩�A�_�b�V�����͂������Ĉړ������ꍇ�Ƀ_�b�V����Ԃɂ���
		if (isDashStart && (player->GetData()->isMoveKey || player->GetData()->isMovePad) && player->GetData()->endurance > 0 && GameInputManager::PushInputAction(GameInputManager::Avoid_Blink_Dash)) {
			isDash = true;
			isDashStart = false;
		}

		//�_�b�V���J�n�s�\���́A�_�b�V���{�^�����͂���x�������Ƃŉ\�ɂȂ�
		if ((!isDashStart) && (!GameInputManager::PushInputAction(GameInputManager::Avoid_Blink_Dash))) {
			isDashStart = true;
		}
	}
	else {
		//�ړ� & ���͒��̓_�b�V����Ԃ��ێ�
		if ((player->GetData()->isMoveKey || player->GetData()->isMovePad) && player->GetData()->endurance > 0 && GameInputManager::PushInputAction(GameInputManager::Avoid_Blink_Dash)) {
			player->UseEndurance(PlayerData::dashUseEndurance, 10, false); //���v�͂��g�p
		}
		//���͂��r�؂ꂽ�Ƃ��Ƀ_�b�V�����I������
		else {
			isDash = false;
		}
	}
}

void PlayerActionMoveNormal::NextActionStart()
{
	//�����ꂩ�̍s�����J�n�ł����Ԃ�����
	if (JumpStart()) { nextAction = PlayerActionName::JUMP; }
	else if (player->GetData()->actionInput.isLightAttack && GameInputManager::TriggerInputAction(GameInputManager::LightAttack) && LightAttackStart()) { nextAction = PlayerActionName::LIGHT_ATTACK; }
	else if (player->GetData()->actionInput.isStrongAttack && GameInputManager::TriggerInputAction(GameInputManager::StrongAttack) && StrongAttackStart()) { nextAction = PlayerActionName::STRONG_ATTACK; }
	else if (AvoidStart()) { nextAction = PlayerActionName::AVOID; player->GetData()->avoidBlinkMoveVec = player->GetData()->moveVec; }
	else if (BlinkStart()) { nextAction = PlayerActionName::BLINK; player->GetData()->avoidBlinkMoveVec = player->GetData()->moveVec; }
	else { return; }

	//�s���J�n�\�Ȃ�A���̍s�����Z�b�g
	player->GetData()->action = nextAction;

	//�ʏ�ړ��s���I��
	isActionEnd = true;
}
