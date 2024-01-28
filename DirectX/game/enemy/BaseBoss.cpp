#include "BaseBoss.h"
#include "Object/3d/collider/SphereCollider.h"
#include "Object/3d/collider/CollisionManager.h"
#include "Object/3d/collider/CollisionAttribute.h"
#include "WindowApp.h"

void BaseBoss::Initialize()
{
	HP = maxHP;
	const float length = 1200;
	hpGauge = std::make_unique<Gauge>(DirectX::XMFLOAT2({ WindowApp::GetWindowWidth() / 2 - length / 2, 650.0f }), length, maxHP, HP, 2.0f, DirectX::XMFLOAT4({ 0.5f, 0.1f, 0.1f, 1.0f }));
	TextureManager::LoadTexture("bossName", "Resources/SpriteTexture/bossgaugeName.png");
	bossNameSprite = Sprite::Create("bossName", {}, { 0,0.5f });
	bossNameSprite->SetPosition(DirectX::XMFLOAT2({ WindowApp::GetWindowWidth() / 2 - length / 2, 625.0f }));
	bossNameSprite->Update();

	isCollider = true;
}

void BaseBoss::Update()
{
	if (isCollider) {
		Collider();
	}

	hpGauge->Update();

	action->Update();

	bossModel->Update();

	//中断行動
	//break行動優先
	if (breakAction) {
		if (!breakAction->GetEnd()) {
			breakAction->Update();
		} else {
			isBreak = false;
			action->SetEnd();
			breakAction.reset();
		}
	} else if (winceAction) {
		if (!winceAction->GetEnd()) {
			winceAction->Update();
		} else {
			isWince = false;
			action->SetEnd();
			winceAction.reset();
		}
	}
}

void BaseBoss::Draw()
{
	bossModel->Draw();
	action->Draw();
	if (breakAction) {
		breakAction->Draw();
	}
}

void BaseBoss::DrawLightView()
{
	bossModel->DrawLightView();
}

void BaseBoss::DrawSprite()
{
	bossNameSprite->Draw();
	hpGauge->Draw();
}

void BaseBoss::Damage(int damageNum)
{
	//怯み加算
	if (!isWince) {
		winceValue += damageNum;
	}

	//HPからダメージ量を引く
	HP -= damageNum;
	HP = max(HP, 0);

	hpGauge->ChangeLength(HP, true);
}

void BaseBoss::Collider()
{
	using namespace DirectX;
	Vector3 pos = Vector3(bossModel->GetPosition());

	const XMFLOAT3 moveMinPos = { 0,100,0 };
	const XMFLOAT3 moveMaxPos = { GameHelper::Instance()->GetStageSize() - 5.0f,10.0f,GameHelper::Instance()->GetStageSize() - 5.0f };

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
	bossModel->SetPosition(pos);
}

void BaseBoss::SetPlayerDirection()
{
	Vector3 rota = VelocityRotate(Vector3(targetPos.x, 0.0f, targetPos.z) - Vector3(bossModel->GetPosition().x, 0.0f, bossModel->GetPosition().z));
	//rota.x += 90.0f;
	bossModel->GetObjectInst()->SetRotation(rota);
}
