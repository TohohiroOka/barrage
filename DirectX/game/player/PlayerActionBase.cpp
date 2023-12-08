#include "PlayerActionBase.h"
#include "Player.h"
#include "system/GameInputManager.h"
#include "Object/3d/Fbx.h"
#include "PlayerSwordAttack1.h"
#include "PlayerSwordAttack2.h"

PlayerActionBase::PlayerActionBase(Player* player)
{
	//プレイヤーをセット
	this->player = player;
}

PlayerActionBase::~PlayerActionBase()
{
}

bool PlayerActionBase::JumpStart()
{
	//ジャンプ回数が連続ジャンプ可能回数を超えていたら抜ける
	if (player->GetData()->jumpCount >= player->GetData()->jumpMaxNum) { return false; }
	//ジャンプ入力がなければ抜ける
	if (!GameInputManager::TriggerInputAction(GameInputManager::Jump)) { return false; }
	//持久力がジャンプで使用する値以下なら抜ける
	if (player->GetData()->endurance < PlayerData::jumpUseEndurance) { return false; }

	return true;
}

bool PlayerActionBase::LightAttackStart()
{
	//持久力が攻撃で使用する値以下なら抜ける
	player->GetData()->attackAction = std::make_unique<PlayerSwordAttack1>(player);
	if (!player->GetData()->attackAction->NextAttack()) { return false; }

	return true;
}

bool PlayerActionBase::StrongAttackStart()
{
	//持久力が攻撃で使用する値以下なら抜ける
	player->GetData()->attackAction = std::make_unique<PlayerSwordAttack2>(player);
	if (!player->GetData()->attackAction->NextAttack()) { return false; }

	return true;
}

bool PlayerActionBase::AvoidStart()
{
	//地面にいない場合は抜ける
	if (!player->GetData()->onGround) { return false; }
	//移動中に回避入力がなければ抜ける
	if (!((player->GetData()->isMoveKey || player->GetData()->isMovePad) && GameInputManager::TriggerInputAction(GameInputManager::Avoid_Blink_Dash))) { return false; }
	//持久力が回避で使用する値以下なら抜ける	
	if (player->GetData()->endurance < PlayerData::avoidUseEndurance) { return false; }

	return true;
}

bool PlayerActionBase::BlinkStart()
{
	//連続ブリンク回数が指定回数以下でなければ抜ける
	if (!(player->GetData()->blinkCount < 1)) { return false; }
	//ジャンプ中でなければ抜ける
	if (!(player->GetData()->jumpCount >= 1)) { return false; }
	//ブリンク入力がなければ抜ける
	if (!GameInputManager::TriggerInputAction(GameInputManager::Avoid_Blink_Dash)) { return false; }
	//持久力がブリンクで使用する値以下なら抜ける
	if (player->GetData()->endurance < PlayerData::blinkUseEndurance) { return false; }

	return true;
}
