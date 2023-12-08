#include "PlayerActionStrongAttack.h"
#include "Player.h"
#include "system/GameInputManager.h"

PlayerActionStrongAttack::PlayerActionStrongAttack(Player* player)
	: PlayerActionBaseAttack(player)
{
}

PlayerActionStrongAttack::~PlayerActionStrongAttack()
{
}

void PlayerActionStrongAttack::NextAttack()
{
	//���̍s������͉\�łȂ���Δ�����
	if (!player->GetData()->attackAction->GetIsNextActionInput()) { return; }

	//�A���ōU�����Z�b�g
	if (GameInputManager::TriggerInputAction(GameInputManager::StrongAttack)) {
		if (!player->GetData()->attackAction->NextAttack()) { return; }

		player->UseEndurance(player->GetData()->attackAction->GetUseEndranceNum(), 60, true); //���v�͂��g�p

		//�\�ߎ��̍s����ݒ肵�Ă���(�I����͒ʏ�ړ�)
		nextAction = PlayerActionName::MOVENORMAL;
	}
	//�W�����v�̐�s���͂��󂯕t����
	else if (JumpStart()) {
		nextAction = PlayerActionName::JUMP;
	}
	//��U���̐�s���͂��󂯕t����
	else if (GameInputManager::TriggerInputAction(GameInputManager::LightAttack)) {
		nextAction = PlayerActionName::LIGHTATTACK;
	}
}
