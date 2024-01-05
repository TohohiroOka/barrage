#include "PlayerActionBaseAttack.h"
#include "Player.h"

PlayerActionBaseAttack::PlayerActionBaseAttack(Player* player)
	: PlayerActionBase(player)
{
	player->UseEndurance(player->GetData()->attackAction->GetUseEndranceNum(), 60, true); //持久力を使用

	//予め次の行動を設定しておく(終了後は通常移動)
	nextAction = PlayerActionName::MOVE_NORMAL;
}

PlayerActionBaseAttack::~PlayerActionBaseAttack()
{
}

void PlayerActionBaseAttack::Update()
{
	//次の攻撃を入力
	NextAttack();

	//攻撃の内容を更新
	player->GetData()->attackAction->Update();
	//攻撃の行動が終了したら攻撃行動終了
	if (player->GetData()->attackAction->GetIsAttackActionEnd()) {
		isActionEnd = true;

		//先行入力で弱攻撃または強攻撃を選択している場合は最終チェック
		if (nextAction == PlayerActionName::LIGHT_ATTACK || nextAction == PlayerActionName::STRONG_ATTACK) {
			bool isNextActionAttack = false;
			if (nextAction == PlayerActionName::LIGHT_ATTACK && LightAttackStart()) { isNextActionAttack = true; }
			else if (nextAction == PlayerActionName::STRONG_ATTACK && StrongAttackStart()) { isNextActionAttack = true; }

			if (!isNextActionAttack) { nextAction = PlayerActionName::MOVE_NORMAL; }
		}
		//攻撃以外を選択している場合は攻撃行動を解放する
		else {
			player->GetData()->attackAction.reset();
			player->GetSword()->GetSlashEffect()->SetIsParticleEmit(false); //斬撃演出生成も停止させておく
		}

		//予約していた次の行動をセット
		player->GetData()->action = nextAction;
	}

	//いつでも回避で攻撃を中断できる
	if (AvoidStart()) {
		isActionEnd = true;

		//回避のベクトルを設定
		player->GetData()->avoidBlinkMoveVec = player->GetData()->moveVec;

		//次の行動を回避にセット
		nextAction = PlayerActionName::AVOID;
		player->GetData()->action = nextAction;

		//攻撃行動を解放する
		player->GetData()->attackAction.reset();
		player->GetSword()->GetSlashEffect()->SetIsParticleEmit(false); //斬撃演出生成も停止させておく
	}
}
