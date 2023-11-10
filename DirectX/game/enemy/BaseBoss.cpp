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
}

void BaseBoss::Draw()
{
	action->Draw();
	bossModel->Draw();
	hpGauge->Draw();
}

void BaseBoss::Damage(int damageNum)
{
	//HPからダメージ量を引く
	HP -= damageNum;
	HP = max(HP, 0);

	hpGauge->ChangeLength(HP, true);
}

void BaseBoss::Collider()
{
	using namespace DirectX;
	Vector3 pos = Vector3(bossModel->GetObjectInst()->GetPosition());
	// 球の上端から球の下端までのレイキャスト
	Segment ray;
	ray.start = { pos.x,pos.y,pos.z,0.0f };
	ray.end = { pos.x + moveVec.x,pos.y + moveVec.y,pos.z + moveVec.z };
	Vector3 nMove = moveVec;
	nMove.normalize();
	ray.dir = { nMove.x, nMove.y,nMove.z,0.0f };
	RAYCAST_HIT raycastHit;

	// スムーズに坂を下る為の吸着距離
	const float adsDistance = 0.2f;

	// 接地を維持
	if (CollisionManager::GetInstance()->Raycast(ray, COLLISION_ATTR_LANDSHAPE, &raycastHit, moveVec.length())) {
		XMFLOAT3 hugou = {};
		if (abs(moveVec.x) < 0.0001f) { hugou.x = 0.0f; } else { hugou.x = adsDistance * moveVec.x / abs(moveVec.x); }
		if (abs(moveVec.y) < 0.0001f) { hugou.y = 0.0f; } else { hugou.y = adsDistance * moveVec.y / abs(moveVec.y); }
		if (abs(moveVec.z) < 0.0001f) { hugou.z = 0.0f; } else { hugou.z = adsDistance * moveVec.z / abs(moveVec.z); }
		Vector3 a = { (raycastHit.inter.m128_f32[0] - pos.x - hugou.x),
						(raycastHit.inter.m128_f32[1] - pos.y - hugou.y),
						(raycastHit.inter.m128_f32[2] - pos.z - hugou.z) };
		if (abs(a.x) < 0.003f) { a.x = 0.0f; }
		if (abs(a.z) < 0.003f) { a.z = 0.0f; }
		if (abs(a.y) < 0.003f) { a.y = 0.0f; }

		pos += a;
	} else {
		pos += moveVec;
	}

	moveVec = { 0.0f,0.0f,0.0f };

	bossModel->GetObjectInst()->SetPosition(pos);

	//速度を加算して座標更新
	pos += moveVec;

	const XMFLOAT3 moveMinPos = { 0,0,0 };
	const XMFLOAT3 moveMaxPos = { 510,0,510 };

	//壁判定
	pos.x = max(pos.x, moveMinPos.x);
	pos.x = min(pos.x, moveMaxPos.x);
	pos.z = max(pos.z, moveMinPos.z);
	pos.z = min(pos.z, moveMaxPos.z);

	//地面に接地判定
	float scaleY = bossModel->GetObjectInst()->GetScale().y;
	const float modelHeight = 5; //スケール1のときのモデルの高さ
	if (pos.y <= scaleY * 2.0f * modelHeight) {
		pos.y = scaleY * 2.0f * modelHeight;
	}
	//最終的な座標をセット
	bossModel->GetObjectInst()->SetPosition(pos);
}
