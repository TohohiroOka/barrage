#include "BaseBoss.h"
#include "Object/3d/collider/SphereCollider.h"
#include "Object/3d/collider/CollisionManager.h"
#include "Object/3d/collider/CollisionAttribute.h"
#include "WindowApp.h"

void BaseBoss::Initialize()
{
	HP = maxHP;
	hpGauge = std::make_unique<Gauge>(DirectX::XMFLOAT2({ WindowApp::GetWindowWidth() / 2 - 600.0f, 650.0f }), "bossGaugeFrame", DirectX::XMFLOAT2({ 5.0f, 5.0f }), maxHP, HP, 2.0f, DirectX::XMFLOAT4({ 0.5f, 0.1f, 0.1f, 1.0f }));
	TextureManager::LoadTexture("bossName", "Resources/SpriteTexture/bossgaugeName.png");
	bossNameSprite = Sprite::Create("bossName", {}, { 0,0.5f });
	bossNameSprite->SetPosition(DirectX::XMFLOAT2({ WindowApp::GetWindowWidth() / 2 - 600.0f / 2, 625.0f }));
	bossNameSprite->Update();

	damageTimer = std::make_unique<Engine::Timer>();

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

	DamageEffect();

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
	isDamage = true;
	damageTimer->Reset();
}

void BaseBoss::DamageEffect()
{
	if (isDamage) {
		if (isDamageRed) {
			bossModel->GetObjectInst()->SetColor({ 0.9f,0.1f,0.1f,1.0f });
		} else {
			bossModel->GetObjectInst()->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		}
		damageTimer->Update();

		if (*damageTimer.get() % 5 == 0) {
			isDamageRed = !isDamageRed;
		}

		if (*damageTimer.get() < 25.0f) { return; }
		bossModel->GetObjectInst()->SetColor({ 1.0f,1.0f,1.0f,1.0f });
		isDamage = false;
		isDamageRed = false;
		damageTimer->Reset();
	}
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
	bossModel->GetObjectInst()->SetRotation(rota);
}
