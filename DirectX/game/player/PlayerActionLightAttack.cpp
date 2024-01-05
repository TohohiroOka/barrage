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
	//次の行動を入力可能でなければ抜ける
	if (!player->GetData()->attackAction->GetIsNextActionInput()) { return; }

	//連続で攻撃をセット
	if (GameInputManager::TriggerInputAction(GameInputManager::LightAttack)) {
		if (!player->GetData()->attackAction->NextAttack()) { return; }

		player->UseEndurance(player->GetData()->attackAction->GetUseEndranceNum(), 60, true); //持久力を使用

		//予め次の行動を設定しておく(終了後は通常移動)
		nextAction = PlayerActionName::MOVE_NORMAL;
	}
	//ジャンプの先行入力も受け付ける
	else if (JumpStart()) {
		nextAction = PlayerActionName::JUMP;
	}
	//強攻撃の先行入力も受け付ける
	else if (GameInputManager::TriggerInputAction(GameInputManager::StrongAttack)) {
		nextAction = PlayerActionName::STRONG_ATTACK;
	}
}
