#include "PlayerActionKnockback.h"
#include "Player.h"
#include "Math/Easing/Easing.h"

PlayerActionKnockback::PlayerActionKnockback(Player* player, const Vector3& knockbackVec, int knockbackTime, int knockbackPower)
	: PlayerActionBase(player)
{
	//ノックバック情報をセット
	this->knockbackVec = knockbackVec;
	this->knockbackTime = knockbackTime;
	this->knockbackPower = (float)knockbackPower;

	//攻撃行動を解放する
	player->GetData()->attackAction.reset();
	player->GetSword()->GetSlashEffect()->SetIsParticleEmit(false); //斬撃演出生成も停止させておく

	//死亡アニメーションに変更
	player->GetFbxObject()->AnimationReset();
	player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::FEAR_ANIMATION);
	player->GetFbxObject()->SetIsRoop(false);

	//予め次の行動を設定しておく(終了後は通常移動)
	nextAction = PlayerActionName::MOVE_NORMAL;
}

PlayerActionKnockback::~PlayerActionKnockback()
{
}

void PlayerActionKnockback::Update()
{
	//ノックバック
	Knockback();
}

void PlayerActionKnockback::Knockback()
{
	//ノックバック
	const float easeTime = (float)knockbackTimer / (float)knockbackTime;
	const float power = Easing::OutSine(knockbackPower, 0, easeTime);
	player->GetData()->velocity = knockbackVec.normalize() * power;

	//タイマー更新
	knockbackTimer++;

	//タイマーが指定した時間になったらノックバック行動終了
	if (knockbackTimer >= knockbackTime) {
		isActionEnd = true;

		//予約していた次の行動をセット
		player->GetData()->action = nextAction;
	}
}
