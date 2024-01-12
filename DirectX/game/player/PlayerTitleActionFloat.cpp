#include "PlayerTitleActionFloat.h"
#include "Player.h"
#include "Math/Easing/Easing.h"

PlayerTitleActionFloat::PlayerTitleActionFloat(Player* player, const Vector3& portalPos)
	:PlayerActionBase(player)
{
	//�C�[�W���O�p�����Z�b�g
	easeBeforePos = player->GetData()->pos;
	Vector3 portalDistance = { 0, 0, -15 };
	easeAfterPos = portalPos + portalDistance;

	easeBeforeRota = player->GetData()->rota;
	easeAfterRota = { 15, 0, 0 };
	Rotate360(easeAfterRota.x);
	Rotate360(easeAfterRota.y);
	Rotate360(easeAfterRota.z);
	//y����]�p��180�ȏ�̏ꍇ�͋t��]�ɂȂ�悤�ɒ���
	if (easeBeforeRota.y >= 180) {
		easeBeforeRota.y -= 360;
	}

	//�ҋ@�A�j���[�V�����ɕύX
	if (!(player->GetFbxObject()->GetUseAnimation() == PlayerAnimationName::STAY_ANIMATION)) {
		player->GetFbxObject()->AnimationReset();
		player->GetFbxObject()->SetUseAnimation(PlayerAnimationName::STAY_ANIMATION);
	}

	//�^�C�}�[����
	floatTimer = std::make_unique<Engine::Timer>();

	//�d�̓I�t
	player->GetData()->isNoGravity = true;
	//���x��0�ɂ���
	player->GetData()->velocity = {0,0,0};

	//�\�ߎ��̍s����ݒ肵�Ă���(�I����̓|�[�^���ɓ���)
	nextAction = PlayerActionName::TITLE_INTO_PORTAL;
}

PlayerTitleActionFloat::~PlayerTitleActionFloat()
{
}

void PlayerTitleActionFloat::Update()
{
	//���V
	Float();
}

void PlayerTitleActionFloat::Float()
{
	const float floatTime = 180;
	floatTimer->Update();
	const float time = *floatTimer.get() / floatTime;
	player->GetData()->pos.x = Easing::InOutQuart(easeBeforePos.x, easeAfterPos.x, time);
	player->GetData()->pos.y = Easing::InOutQuart(easeBeforePos.y, easeAfterPos.y, time);
	player->GetData()->pos.z = Easing::InOutQuart(easeBeforePos.z, easeAfterPos.z, time);

	player->GetData()->rota.x = Easing::InOutQuart(easeBeforeRota.x, easeAfterRota.x, time);
	player->GetData()->rota.y = Easing::InOutQuart(easeBeforeRota.y, easeAfterRota.y, time);
	player->GetData()->rota.z = Easing::InOutQuart(easeBeforeRota.z, easeAfterRota.z, time);
	player->GetFbxObject()->SetRotation(player->GetData()->rota);

	if (*floatTimer.get() >= floatTime) {
		isActionEnd = true;

		//�\�񂵂Ă������̍s�����Z�b�g
		player->GetData()->action = nextAction;
	}
}
