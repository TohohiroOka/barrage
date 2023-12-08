#include "PlayerActionKnockback.h"
#include "Player.h"
#include "Math/Easing/Easing.h"

PlayerActionKnockback::PlayerActionKnockback(Player* player, const Vector3& knockbackVec, int knockbackTime, int knockbackPower)
	: PlayerActionBase(player)
{
	//�m�b�N�o�b�N�����Z�b�g
	this->knockbackVec = knockbackVec;
	this->knockbackTime = knockbackTime;
	this->knockbackPower = (float)knockbackPower;

	//�\�ߎ��̍s����ݒ肵�Ă���(�I����͒ʏ�ړ�)
	nextAction = PlayerActionName::MOVENORMAL;
}

PlayerActionKnockback::~PlayerActionKnockback()
{
}

void PlayerActionKnockback::Update()
{
	//�m�b�N�o�b�N
	Knockback();
}

void PlayerActionKnockback::Knockback()
{
	//�m�b�N�o�b�N
	const float easeTime = (float)knockbackTimer / (float)knockbackTime;
	const float power = Easing::OutSine(knockbackPower, 0, easeTime);
	player->GetData()->velocity = knockbackVec.normalize() * power;

	//�^�C�}�[�X�V
	knockbackTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ�����m�b�N�o�b�N�s���I��
	if (knockbackTimer >= knockbackTime) {
		isActionEnd = true;

		//�\�񂵂Ă������̍s�����Z�b�g
		player->GetData()->action = nextAction;
	}
}
