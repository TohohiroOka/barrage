#include "PlayerActionAvoid.h"
#include "player.h"
#include "Math/Easing/Easing.h"

PlayerActionAvoid::PlayerActionAvoid(Player* player)
	: PlayerActionBase(player)
{
	player->UseEndurance(avoidUseEndurance, 45, true); //���v�͂��g�p

	//�^�C�}�[����
	avoidTimer = std::make_unique<Engine::Timer>();

	//���G���
	player->GetData()->isNoDamage = true;

	//�������������悤�ɂ���
	const Vector3 moveRotaVelocity = { player->GetData()->avoidBlinkMoveVec.x, 0, player->GetData()->avoidBlinkMoveVec.z }; //�v���C���[��]�ɃW�����v�͊֌W�Ȃ��̂ŁA���xY��0�ɂ��Ă���
	player->SetMoveRotate(moveRotaVelocity, 15.0f);

	//����A�j���[�V�����ɕύX
	player->GetFbxObject()->AnimationReset();
	player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::ROLL_ANIMATION);

	//�\�ߎ��̍s����ݒ肵�Ă���(�I����͒ʏ�ړ�)
	nextAction = PlayerActionName::MOVENORMAL;
}

PlayerActionAvoid::~PlayerActionAvoid()
{
}

void PlayerActionAvoid::Update()
{
	//���
	Avoid();
}

void PlayerActionAvoid::Avoid()
{
	//�^�C�}�[�X�V
	avoidTimer->Update();
	const float time = *avoidTimer.get() / (float)avoidTime;

	//�C�[�W���O�ő��x�𗎂Ƃ��Ă���
	const float power = Easing::OutCirc(2, 0.5f, time);
	player->GetData()->velocity = player->GetData()->avoidBlinkMoveVec.normalize() * power;


	//�^�C�}�[���w�肵�����ԂɂȂ�����s���ύX��s����
	if (*avoidTimer.get() >= actionChangeStartTime) {
		if (JumpStart()) { nextAction = PlayerActionName::JUMP; }
		if (AttackStart()) { nextAction = PlayerActionName::ATTACK; }
	}


	//�^�C�}�[���w�肵�����ԂɂȂ��������s���I��
	if (*avoidTimer.get() >= avoidTime) {
		isActionEnd = true;

		//�\�񂵂Ă������̍s�����Z�b�g
		player->GetData()->action = nextAction;

		//���G��ԏI��
		player->GetData()->isNoDamage = false;
	}
}
