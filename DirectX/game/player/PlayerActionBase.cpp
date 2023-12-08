#include "PlayerActionBase.h"
#include "Player.h"
#include "system/GameInputManager.h"
#include "Object/3d/Fbx.h"
#include "PlayerSwordAttack1.h"
#include "PlayerSwordAttack2.h"

PlayerActionBase::PlayerActionBase(Player* player)
{
	//�v���C���[���Z�b�g
	this->player = player;
}

PlayerActionBase::~PlayerActionBase()
{
}

bool PlayerActionBase::JumpStart()
{
	//�W�����v�񐔂��A���W�����v�\�񐔂𒴂��Ă����甲����
	if (player->GetData()->jumpCount >= player->GetData()->jumpMaxNum) { return false; }
	//�W�����v���͂��Ȃ���Δ�����
	if (!GameInputManager::TriggerInputAction(GameInputManager::Jump)) { return false; }
	//���v�͂��W�����v�Ŏg�p����l�ȉ��Ȃ甲����
	if (player->GetData()->endurance < PlayerData::jumpUseEndurance) { return false; }

	return true;
}

bool PlayerActionBase::LightAttackStart()
{
	//���v�͂��U���Ŏg�p����l�ȉ��Ȃ甲����
	player->GetData()->attackAction = std::make_unique<PlayerSwordAttack1>(player);
	if (!player->GetData()->attackAction->NextAttack()) { return false; }

	return true;
}

bool PlayerActionBase::StrongAttackStart()
{
	//���v�͂��U���Ŏg�p����l�ȉ��Ȃ甲����
	player->GetData()->attackAction = std::make_unique<PlayerSwordAttack2>(player);
	if (!player->GetData()->attackAction->NextAttack()) { return false; }

	return true;
}

bool PlayerActionBase::AvoidStart()
{
	//�n�ʂɂ��Ȃ��ꍇ�͔�����
	if (!player->GetData()->onGround) { return false; }
	//�ړ����ɉ����͂��Ȃ���Δ�����
	if (!((player->GetData()->isMoveKey || player->GetData()->isMovePad) && GameInputManager::TriggerInputAction(GameInputManager::Avoid_Blink_Dash))) { return false; }
	//���v�͂�����Ŏg�p����l�ȉ��Ȃ甲����	
	if (player->GetData()->endurance < PlayerData::avoidUseEndurance) { return false; }

	return true;
}

bool PlayerActionBase::BlinkStart()
{
	//�A���u�����N�񐔂��w��񐔈ȉ��łȂ���Δ�����
	if (!(player->GetData()->blinkCount < 1)) { return false; }
	//�W�����v���łȂ���Δ�����
	if (!(player->GetData()->jumpCount >= 1)) { return false; }
	//�u�����N���͂��Ȃ���Δ�����
	if (!GameInputManager::TriggerInputAction(GameInputManager::Avoid_Blink_Dash)) { return false; }
	//���v�͂��u�����N�Ŏg�p����l�ȉ��Ȃ甲����
	if (player->GetData()->endurance < PlayerData::blinkUseEndurance) { return false; }

	return true;
}
