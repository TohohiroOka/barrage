#include "BaseBoss.h"
#include "Object/3d/collider/SphereCollider.h"
#include "Object/3d/collider/CollisionManager.h"
#include "Object/3d/collider/CollisionAttribute.h"
#include "WindowApp.h"

void BaseBoss::Initialize()
{
	HP = maxHP;
	const float length = 1200;
	hpGauge = std::make_unique<Gauge>(DirectX::XMFLOAT2({ WindowApp::GetWindowWidth() / 2 - length / 2, 650.0f }), length, maxHP, HP, DirectX::XMFLOAT4({ 0.5f, 0.1f, 0.1f, 1.0f }));
}

void BaseBoss::Update()
{
	Collider();

	hpGauge->Update();

	action->Update();

	bossModel->Update();

	if (winceAction) {
		if (!winceAction->End()) {
			winceAction->Update();
		} else {
			isWince = false;
			winceAction.reset();
		}
	}
}

void BaseBoss::Draw()
{
	action->Draw();
	bossModel->Draw();
}

void BaseBoss::DrawLightView()
{
	bossModel->DrawLightView();
}

void BaseBoss::DrawSprite()
{
	hpGauge->Draw();
}

void BaseBoss::Damage(int damageNum)
{
	//���݉��Z
	winceValue += damageNum;

	//HP����_���[�W�ʂ�����
	HP -= damageNum;
	HP = max(HP, 0);

	hpGauge->ChangeLength(HP, true);
}

void BaseBoss::Collider()
{
	using namespace DirectX;
	Vector3 pos = Vector3(bossModel->GetObjectInst()->GetPosition());
	//���x�����Z���č��W�X�V
	pos += moveVec;

	const XMFLOAT3 moveMinPos = { 0,100,0 };
	const XMFLOAT3 moveMaxPos = { 510,0,510 };

	//�ǔ���
	pos.x = max(pos.x, moveMinPos.x);
	pos.x = min(pos.x, moveMaxPos.x);
	pos.z = max(pos.z, moveMinPos.z);
	pos.z = min(pos.z, moveMaxPos.z);

	//�n�ʂɐڒn����
	if (pos.y <= 2.0f * hitScale) {
		pos.y = 2.0f * hitScale;
	}
	//�ŏI�I�ȍ��W���Z�b�g
	bossModel->GetObjectInst()->SetPosition(pos);
}
