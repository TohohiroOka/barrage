#include "PlayerTitleActionIntoPortal.h"
#include "Player.h"
#include "Math/Easing/Easing.h"

PlayerTitleActionIntoPortal::PlayerTitleActionIntoPortal(Player* player, const Vector3& portalPos)
	:PlayerActionBase(player)
{
	//�C�[�W���O�p�����Z�b�g
	easeBeforePos = player->GetData()->pos;
	Vector3 portalDistance = { 0, 0, -5 };
	easeAfterPos = portalPos + portalDistance;

	easeBeforeRota = player->GetData()->rota;
	easeAfterRota = { 75, 0, 0 };
	Rotate360(easeAfterRota.x);
	Rotate360(easeAfterRota.y);
	Rotate360(easeAfterRota.z);
	//y����]�p��180�ȏ�̏ꍇ�͋t��]�ɂȂ�悤�ɒ���
	if (easeBeforeRota.y >= 180) {
		easeBeforeRota.y -= 360;
	}

	easeBeforeScale = player->GetFbxObject()->GetScale();
	const float afterSize = 0;
	easeAfterScale = { afterSize, afterSize, afterSize };

	//�^�C�}�[����
	intoPortalTimer = std::make_unique<Engine::Timer>();
}

PlayerTitleActionIntoPortal::~PlayerTitleActionIntoPortal()
{
}

void PlayerTitleActionIntoPortal::Update()
{
	//�|�[�^���ɓ���
	IntoPortal();
}

void PlayerTitleActionIntoPortal::IntoPortal()
{
	//���ɓ����Ă����甲����
	if (isInPortal) { return; }

	const float moveTime = 100;
	intoPortalTimer->Update();
	const float time = *intoPortalTimer.get() / moveTime;
	player->GetData()->pos.x = Easing::OutQuint(easeBeforePos.x, easeAfterPos.x, time);
	player->GetData()->pos.y = Easing::OutQuint(easeBeforePos.y, easeAfterPos.y, time);
	player->GetData()->pos.z = Easing::OutQuint(easeBeforePos.z, easeAfterPos.z, time);

	player->GetData()->rota.x = Easing::OutQuint(easeBeforeRota.x, easeAfterRota.x, time);
	player->GetData()->rota.y = Easing::OutQuint(easeBeforeRota.y, easeAfterRota.y, time);
	player->GetData()->rota.z = Easing::OutQuint(easeBeforeRota.z, easeAfterRota.z, time);
	player->GetFbxObject()->SetRotation(player->GetData()->rota);

	DirectX::XMFLOAT3 scale{};
	scale.x = Easing::OutQuad(easeBeforeScale.x, easeAfterScale.x, time);
	scale.y = Easing::OutQuad(easeBeforeScale.y, easeAfterScale.y, time);
	scale.z = Easing::OutQuad(easeBeforeScale.z, easeAfterScale.z, time);
	player->GetFbxObject()->SetScale(scale);

	if (*intoPortalTimer.get() >= moveTime) {
		//�|�[�^���ɓ�����
		isInPortal = true;
	}
}
