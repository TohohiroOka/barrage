#include "PlayerActionBlink.h"
#include "Player.h"
#include "Math/Easing/Easing.h"

PlayerActionBlink::PlayerActionBlink(Player* player)
	: PlayerActionBase(player)
{
	player->UseEndurance(blinkUseEndurance, 60, true); //���v�͂��g�p

	//�u�����N�񐔂𑝂₷(�n�ʂɒ����΃��Z�b�g)
	player->GetData()->blinkCount++;
	
	//�^�C�}�[����
	blinkTimer = std::make_unique<Engine::Timer>();

	//�������x��0�ɂ���
	player->GetData()->velocity.y = 0;
	player->GetData()->isNoGravity = true;

	//���G���
	player->GetData()->isNoDamage = true;

	//�u�����N�����������悤�ɂ���
	const Vector3 moveRotaVelocity = { player->GetData()->avoidBlinkMoveVec.x, 0, player->GetData()->avoidBlinkMoveVec.z }; //�v���C���[��]�ɃW�����v�͊֌W�Ȃ��̂ŁA���xY��0�ɂ��Ă���
	player->SetMoveRotate(moveRotaVelocity, 15.0f);

	//����A�j���[�V�����ɕύX
	player->GetFbxObject()->AnimationReset();
	player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::ROLL_ANIMATION);

	//�\�ߎ��̍s����ݒ肵�Ă���(�I����͒ʏ�ړ�)
	nextAction = PlayerActionName::MOVENORMAL;
}

PlayerActionBlink::~PlayerActionBlink()
{
}

void PlayerActionBlink::Update()
{
	//�u�����N
	Blink();
}

void PlayerActionBlink::Blink()
{
	//�^�C�}�[�X�V
	blinkTimer->Update();
	const float time = *blinkTimer.get() / (float)blinkTime;

	//�C�[�W���O�ő��x�𗎂Ƃ��Ă���
	const float power = Easing::OutCirc(4, 1, time);
	player->GetData()->velocity = player->GetData()->avoidBlinkMoveVec.normalize() * power;

	//�^�C�}�[���w�肵�����ԂɂȂ�����s���ύX��s����
	if (*blinkTimer.get() >= actionChangeStartTime) {
		if (JumpStart()) { nextAction = PlayerActionName::JUMP; }
		if (AttackStart()) { nextAction = PlayerActionName::ATTACK; }
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����u�����N�s���I��
	if (*blinkTimer.get() >= blinkTime) {
		isActionEnd = true;

		//�\�񂵂Ă������̍s�����Z�b�g
		player->GetData()->action = nextAction;

		//���d�͏�ԏI��
		player->GetData()->isNoGravity = false;
		//���G��ԏI��
		player->GetData()->isNoDamage = false;
	}
}
