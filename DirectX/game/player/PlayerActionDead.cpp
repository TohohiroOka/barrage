#include "PlayerActionDead.h"
#include "Player.h"

PlayerActionDead::PlayerActionDead(Player* player)
	: PlayerActionBase(player)
{
	//死亡状態にする
	player->GetData()->isDead = true;

	//速度を0にする
	player->GetData()->velocity.x = 0;
	player->GetData()->velocity.z = 0;
	
	//死亡アニメーションに変更
	player->GetFbxObject()->AnimationReset();
	player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::STAY_ANIMATION);
}

PlayerActionDead::~PlayerActionDead()
{
}
