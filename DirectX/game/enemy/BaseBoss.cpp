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
	//怯み加算
	winceValue += damageNum;

	//HPからダメージ量を引く
	HP -= damageNum;
	HP = max(HP, 0);

	hpGauge->ChangeLength(HP, true);
}

void BaseBoss::Collider()
{
	using namespace DirectX;
	Vector3 pos = Vector3(bossModel->GetObjectInst()->GetPosition());
	//速度を加算して座標更新
	pos += moveVec;

	const XMFLOAT3 moveMinPos = { 0,100,0 };
	const XMFLOAT3 moveMaxPos = { 510,0,510 };

	//壁判定
	pos.x = max(pos.x, moveMinPos.x);
	pos.x = min(pos.x, moveMaxPos.x);
	pos.z = max(pos.z, moveMinPos.z);
	pos.z = min(pos.z, moveMaxPos.z);

	//地面に接地判定
	if (pos.y <= 2.0f * hitScale) {
		pos.y = 2.0f * hitScale;
	}
	//最終的な座標をセット
	bossModel->GetObjectInst()->SetPosition(pos);
}
