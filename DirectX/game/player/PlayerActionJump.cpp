#include "PlayerActionJump.h"
#include "Player.h"

float PlayerActionJump::jumpPower = 2.0f;

PlayerActionJump::PlayerActionJump(Player* player)
	: PlayerActionBase(player)
{
	player->UseEndurance(PlayerData::jumpUseEndurance, 45, true); //���v�͂��g�p

	player->GetData()->onGround = false; //�n�ʂƂ̐ڒn������
	player->GetData()->isInputJump = true; //�W�����v���͏�Ԃɂ���
	player->GetData()->velocity.y = jumpPower; //������ɒ���
	player->GetData()->fallSpeed = 0; //���Ԃ̂ŗ������x��0��
	player->GetData()->jumpCount++; //�W�����v�񐔂𑝂₷

	//�W�����v�A�j���[�V�����ɕύX
	player->GetFbxObject()->AnimationReset();
	player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::JUMP_ANIMATION);

	//�\�ߎ��̍s����ݒ肵�Ă���(�I����͒ʏ�ړ�)
	nextAction = PlayerActionName::MOVENORMAL;
}

PlayerActionJump::~PlayerActionJump()
{
}

void PlayerActionJump::Update()
{
	//�W�����v�����邾���Ȃ̂ň�u�ŏI��
	isActionEnd = true;
	//�\�񂵂Ă������̍s�����Z�b�g
	player->GetData()->action = nextAction;
}
