#include "PlayerActionKnockback.h"
#include "Player.h"

PlayerActionKnockback::PlayerActionKnockback(Player* player, const Vector3& subjectPos, int power)
	: PlayerActionBase(player)
{
	//�U���ΏۂƎ����̃x�N�g�����Z�o
	knockbackVec = player->GetData()->pos - subjectPos;
	//�m�b�N�o�b�N�̋������Z�b�g(���ŐH������_���[�W�� / 10)
	knockbackPower = (float)power / 10;

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
	player->GetData()->velocity = knockbackVec.normalize() * knockbackPower;

	//�m�b�N�o�b�N���キ���Ă���
	knockbackPower -= 0.05f * GameHelper::Instance()->GetGameSpeed();

	//�m�b�N�o�b�N�ɂ��ړ����Ȃ��Ȃ�����m�b�N�o�b�N�s���I��
	if (knockbackPower < 0) {
		isActionEnd = true;

		//�\�񂵂Ă������̍s�����Z�b�g
		player->GetData()->action = nextAction;
	}
}
