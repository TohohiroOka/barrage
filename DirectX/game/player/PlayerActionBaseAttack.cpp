#include "PlayerActionBaseAttack.h"
#include "Player.h"

PlayerActionBaseAttack::PlayerActionBaseAttack(Player* player)
	: PlayerActionBase(player)
{
	player->UseEndurance(player->GetData()->attackAction->GetUseEndranceNum(), 60, true); //���v�͂��g�p

	//�\�ߎ��̍s����ݒ肵�Ă���(�I����͒ʏ�ړ�)
	nextAction = PlayerActionName::MOVENORMAL;
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
		if (nextAction == PlayerActionName::LIGHTATTACK || nextAction == PlayerActionName::STRONGATTACK) {
			bool isNextActionAttack = false;
			if (nextAction == PlayerActionName::LIGHTATTACK && LightAttackStart()) { isNextActionAttack = true; }
			else if (nextAction == PlayerActionName::STRONGATTACK && StrongAttackStart()) { isNextActionAttack = true; }

			if (!isNextActionAttack) { nextAction = PlayerActionName::MOVENORMAL; }
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
