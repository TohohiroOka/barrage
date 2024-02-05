#include "PlayerActionAvoid.h"
#include "player.h"
#include "system/GameInputManager.h"
#include "Math/Easing/Easing.h"
#include "engine/Audio/Audio.h"

const float PlayerActionAvoid::avoidStartMoveSpeed = 1.1f;
const float PlayerActionAvoid::avoidEndMoveSpeed = 0.4f;

PlayerActionAvoid::PlayerActionAvoid(Player* player)
	: PlayerActionBase(player)
{
	player->UseEndurance(PlayerData::avoidUseEndurance, 50, true); //���v�͂��g�p

	//�^�C�}�[����
	avoidTimer = std::make_unique<Engine::Timer>();

	//���G���
	player->GetData()->isNoDamage = true;

	//�G�̍U�����������������J�n����
	player->GetData()->isEnemyAttackAvoidJudge = true;

	//�������������悤�ɂ���
	Vector3 moveRotaVelocity = player->GetData()->avoidBlinkMoveVec;
	moveRotaVelocity.y = 0; //�v���C���[��]�ɃW�����v�͊֌W�Ȃ��̂ŁA���xY��0�ɂ��Ă���
	player->SetMoveRotate(moveRotaVelocity, 15.0f);

	//����A�j���[�V�����ɕύX
	player->GetFbxObject()->AnimationReset();
	player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::ROLL_ANIMATION);
	player->GetFbxObject()->SetIsRoop(false);

	//�\�ߎ��̍s����ݒ肵�Ă���(�I����͒ʏ�ړ�)
	nextAction = PlayerActionName::MOVE_NORMAL;

	//������Đ�
	Audio::Instance()->SoundPlayWava(Sound::SoundName::avoid, false, 0.1f);
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

	//�C�[�W���O�ňړ��X�s�[�h�𗎂Ƃ��Ă���
	player->GetData()->moveSpeed = Easing::InCirc(avoidStartMoveSpeed, avoidEndMoveSpeed, time);
	player->GetData()->velocity = player->GetData()->avoidBlinkMoveVec.normalize() * player->GetData()->moveSpeed;


	//�^�C�}�[���w�肵�����ԂɂȂ�����s���ύX��s����
	if (*avoidTimer.get() >= actionChangeStartTime) {
		if (JumpStart()) { nextAction = PlayerActionName::JUMP; }
		else if (GameInputManager::TriggerInputAction(GameInputManager::LightAttack)) { nextAction = PlayerActionName::LIGHT_ATTACK; }
		else if (GameInputManager::TriggerInputAction(GameInputManager::StrongAttack)) { nextAction = PlayerActionName::STRONG_ATTACK; }
	}


	//�^�C�}�[���w�肵�����ԂɂȂ��������s���I��
	if (*avoidTimer.get() >= avoidTime) {
		isActionEnd = true;

		//��s���͂Ŏ�U���܂��͋��U����I�����Ă���ꍇ�͍ŏI�`�F�b�N
		if (nextAction == PlayerActionName::LIGHT_ATTACK || nextAction == PlayerActionName::STRONG_ATTACK) {
			bool isNextActionAttack = false;
			if (nextAction == PlayerActionName::LIGHT_ATTACK && LightAttackStart()) { isNextActionAttack = true; }
			else if (nextAction == PlayerActionName::STRONG_ATTACK && StrongAttackStart()) { isNextActionAttack = true; }

			if (!isNextActionAttack) { nextAction = PlayerActionName::MOVE_NORMAL; }
		}

		//�\�񂵂Ă������̍s�����Z�b�g
		player->GetData()->action = nextAction;

		//���G��ԏI��
		player->GetData()->isNoDamage = false;
	}
}
