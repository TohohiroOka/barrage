#include "PlayerActionBaseAttack.h"
#include "Player.h"

PlayerActionBaseAttack::PlayerActionBaseAttack(Player* player)
	: PlayerActionBase(player)
{
	player->UseEndurance(player->GetData()->attackAction->GetUseEndranceNum(), 60, true); //���v�͂��g�p

	//�\�ߎ��̍s����ݒ肵�Ă���(�I����͒ʏ�ړ�)
	nextAction = PlayerActionName::MOVE_NORMAL;
}

PlayerActionBaseAttack::~PlayerActionBaseAttack()
{
}

void PlayerActionBaseAttack::Update()
{
	//���̍U�������
	NextAttack();

	//�U���̓��e���X�V
	player->GetData()->attackAction->Update();
	//�U���̍s�����I��������U���s���I��
	if (player->GetData()->attackAction->GetIsAttackActionEnd()) {
		isActionEnd = true;

		//��s���͂Ŏ�U���܂��͋��U����I�����Ă���ꍇ�͍ŏI�`�F�b�N
		if (nextAction == PlayerActionName::LIGHT_ATTACK || nextAction == PlayerActionName::STRONG_ATTACK) {
			bool isNextActionAttack = false;
			if (nextAction == PlayerActionName::LIGHT_ATTACK && LightAttackStart()) { isNextActionAttack = true; }
			else if (nextAction == PlayerActionName::STRONG_ATTACK && StrongAttackStart()) { isNextActionAttack = true; }

			if (!isNextActionAttack) { nextAction = PlayerActionName::MOVE_NORMAL; }
		}
		//�U���ȊO��I�����Ă���ꍇ�͍U���s�����������
		else {
			player->GetData()->attackAction.reset();
			player->GetSword()->GetSlashEffect()->SetIsParticleEmit(false); //�a�����o��������~�����Ă���
		}

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

		//�U���s�����������
		player->GetData()->attackAction.reset();
		player->GetSword()->GetSlashEffect()->SetIsParticleEmit(false); //�a�����o��������~�����Ă���
	}
}
