#include "PlayerActionBlink.h"
#include "Player.h"
#include "Math/Easing/Easing.h"
#include "engine/Audio/Audio.h"
#include "system/GameInputManager.h"

PlayerActionBlink::PlayerActionBlink(Player* player)
	: PlayerActionBase(player)
{
	player->UseEndurance(PlayerData::blinkUseEndurance, 45, true); //���v�͂��g�p

	//�u�����N�񐔂𑝂₷(�n�ʂɒ����΃��Z�b�g)
	player->GetData()->blinkCount++;
	
	//�^�C�}�[����
	blinkTimer = std::make_unique<Engine::Timer>();

	//�������x��0�ɂ���
	player->GetData()->velocity.y = 0;
	player->GetData()->isNoGravity = true;

	//���G���
	player->GetData()->isNoDamage = true;

	//�u�����N�����������悤�ɂ���
	const Vector3 moveRotaVelocity = { player->GetData()->avoidBlinkMoveVec.x, 0, player->GetData()->avoidBlinkMoveVec.z }; //�v���C���[��]�ɃW�����v�͊֌W�Ȃ��̂ŁA���xY��0�ɂ��Ă���
	player->SetMoveRotate(moveRotaVelocity, 15.0f);

	//����A�j���[�V�����ɕύX
	player->GetFbxObject()->AnimationReset();
	player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::BLINK_ANIMATION);

	//�\�ߎ��̍s����ݒ肵�Ă���(�I����͒ʏ�ړ�)
	nextAction = PlayerActionName::MOVE_NORMAL;

	//������Đ�
	Audio::Instance()->SoundPlayWava(Sound::SoundName::avoid, false, 0.1f);
}

PlayerActionBlink::~PlayerActionBlink()
{
}

void PlayerActionBlink::Update()
{
	//�u�����N
	Blink();
}

void PlayerActionBlink::Blink()
{
	//�^�C�}�[�X�V
	blinkTimer->Update();
	const float time = *blinkTimer.get() / (float)blinkTime;

	//�C�[�W���O�ő��x�𗎂Ƃ��Ă���
	const float power = Easing::OutCirc(4, 1, time);
	player->GetData()->velocity = player->GetData()->avoidBlinkMoveVec.normalize() * power;

	//�^�C�}�[���w�肵�����ԂɂȂ�����s���ύX��s����
	if (*blinkTimer.get() >= actionChangeStartTime) {
		if (JumpStart()) { nextAction = PlayerActionName::JUMP; }
		else if (GameInputManager::TriggerInputAction(GameInputManager::LightAttack)) { nextAction = PlayerActionName::LIGHT_ATTACK; }
		else if (GameInputManager::TriggerInputAction(GameInputManager::StrongAttack)) { nextAction = PlayerActionName::STRONG_ATTACK; }
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����u�����N�s���I��
	if (*blinkTimer.get() >= blinkTime) {
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

		//���d�͏�ԏI��
		player->GetData()->isNoGravity = false;
		//���G��ԏI��
		player->GetData()->isNoDamage = false;
	}
}
