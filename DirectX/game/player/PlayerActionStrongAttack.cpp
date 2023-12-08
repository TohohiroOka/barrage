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
	//次の行動を入力可能でなければ抜ける
	if (!player->GetData()->attackAction->GetIsNextActionInput()) { return; }

	//連続で攻撃をセット
	if (GameInputManager::TriggerInputAction(GameInputManager::StrongAttack)) {
		if (!player->GetData()->attackAction->NextAttack()) { return; }

		player->UseEndurance(player->GetData()->attackAction->GetUseEndranceNum(), 60, true); //持久力を使用

		//予め次の行動を設定しておく(終了後は通常移動)
		nextAction = PlayerActionName::MOVENORMAL;
	}
	//ジャンプの先行入力も受け付ける
	else if (JumpStart()) {
		nextAction = PlayerActionName::JUMP;
	}
	//弱攻撃の先行入力も受け付ける
	else if (GameInputManager::TriggerInputAction(GameInputManager::LightAttack)) {
		nextAction = PlayerActionName::LIGHTATTACK;
	}
}
