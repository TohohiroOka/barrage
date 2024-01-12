#include "PlayerActionLightAttack.h"
#include "Player.h"
#include "system/GameInputManager.h"

PlayerActionLightAttack::PlayerActionLightAttack(Player* player)
	: PlayerActionBaseAttack(player)
{
}

PlayerActionLightAttack::~PlayerActionLightAttack()
{
}

void PlayerActionLightAttack::NextAttack()
{
	//���̍s������͉\�łȂ���Δ�����
	if (!player->GetData()->attackAction->GetIsNextActionInput()) { return; }

	//�A���ōU�����Z�b�g
	if (GameInputManager::TriggerInputAction(GameInputManager::LightAttack)) {
		if (!player->GetData()->attackAction->NextAttack()) { return; }

		player->UseEndurance(player->GetData()->attackAction->GetUseEndranceNum(), 60, true); //���v�͂��g�p

		//�\�ߎ��̍s����ݒ肵�Ă���(�I����͒ʏ�ړ�)
		nextAction = PlayerActionName::MOVE_NORMAL;
	}
	//�W�����v�̐�s���͂��󂯕t����
	else if (JumpStart()) {
		nextAction = PlayerActionName::JUMP;
	}
	//���U���̐�s���͂��󂯕t����
	else if (GameInputManager::TriggerInputAction(GameInputManager::StrongAttack)) {
		nextAction = PlayerActionName::STRONG_ATTACK;
	}
}
