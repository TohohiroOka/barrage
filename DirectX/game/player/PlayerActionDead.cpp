#include "PlayerActionDead.h"
#include "Player.h"

PlayerActionDead::PlayerActionDead(Player* player)
	: PlayerActionBase(player)
{
	//���S��Ԃɂ���
	player->GetData()->isDead = true;

	//���x��0�ɂ���
	player->GetData()->velocity.x = 0;
	player->GetData()->velocity.z = 0;
	
	//���S�A�j���[�V�����ɕύX
	player->GetFbxObject()->AnimationReset();
	player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::STAY_ANIMATION);
}

PlayerActionDead::~PlayerActionDead()
{
}
