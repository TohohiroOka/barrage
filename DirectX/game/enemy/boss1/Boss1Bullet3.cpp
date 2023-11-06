#include "Boss1Bullet3.h"
#include "GameHelper.h"
#include "../BaseBoss.h"
#include "Math/Easing/Easing.h"

Boss1Bullet3::Boss1Bullet3()
{
	model = Model::CreateFromOBJ("bullet");
	for (auto& i : instanceObject) {
		i = InstanceObject::Create(model.get());
	}
	AddBullet();
	timer = std::make_unique<Engine::Timer>();
	bulletEffect.Init();
}

void Boss1Bullet3::Update()
{
	const float maxTimer = 100.0f;
	//更新処理
	for (std::forward_list<BulletInfo>::iterator it = bullet.begin();
		it != bullet.end(); it++) {
		BulletUpdate(*it);
	}

	//falseなら消す
	bullet.remove_if([](BulletInfo& x) {
		return !x.isAlive;
		}
	);

	//終了
	if (std::distance(bullet.begin(), bullet.end()) <= 0) {
		isEnd = true;
	}

	BaseBullet::Update();
}

void Boss1Bullet3::GetAttackCollision(std::vector<BaseAction::AttackCollision>& _info)
{
	for (std::forward_list<BulletInfo>::iterator it = bullet.begin();
		it != bullet.end(); it++) {
		BaseAction::AttackCollision add;
		add.pos = it->pos;
		add.radius = 1.0f;
		_info.emplace_back(add);
	}
}

void Boss1Bullet3::AddBullet()
{
	using namespace DirectX;
	Vector3 bossPos = boss->GetCenter()->GetPosition();

	const int num = 6;

	for (int x = 1; x <= num; x++) {
		for (int y = 0; y < num; y++) {
			for (int z = 1; z <= num; z++) {
				bullet.emplace_front();
				BulletInfo& add = bullet.front();
				add.isAlive = true;
				add.pos = bossPos;
				add.acceleration = 10.0f;
				add.moveVec = {
					sinf(XMConvertToRadians(360.0f/ num * float(x))),
					0.0f,
					cosf(XMConvertToRadians(360.0f/ num * float(z)))
				};
				add.moveVec = add.moveVec.normalize();
				add.moveVec.y = float(y-1) * 0.1f;
				add.timer = std::make_unique<Engine::Timer>();
			}
		}
	}
}

void Boss1Bullet3::BulletUpdate(BulletInfo& _bullet)
{
	const float maxTime = 40.0f;
	if (*_bullet.timer.get() <= maxTime) {
		_bullet.acceleration = Easing::OutCirc(10.0f, 0.0f, *_bullet.timer.get() / maxTime);
		_bullet.pos += _bullet.moveVec * _bullet.acceleration * GameHelper::Instance()->GetGameSpeed();
	} else if (*_bullet.timer.get() <= maxTime + maxTime + 1) {
		_bullet.acceleration = Easing::OutCirc(0.0f, -5.0f, (*_bullet.timer.get() - maxTime) / maxTime);
		_bullet.pos += Vector3{_bullet.moveVec.x, 0.0f, _bullet.moveVec.z} *_bullet.acceleration* GameHelper::Instance()->GetGameSpeed();
	} else {
		_bullet.pos += Vector3{_bullet.moveVec.x, 0.0f, _bullet.moveVec.z} *_bullet.acceleration* GameHelper::Instance()->GetGameSpeed();
	}

	const float dist = 10;
	if (_bullet.pos.x < -dist || _bullet.pos.x > 510.0f + dist ||
		_bullet.pos.y < -dist || _bullet.pos.y > 255.0f + dist ||
		_bullet.pos.z < -dist || _bullet.pos.z > 510.0f + dist) {
		_bullet.isAlive = false;
		return;
	}

	_bullet.timer->Update();

	for (auto& i : instanceObject) {
		if (!i->GetInstanceDrawCheck()) { continue; }
		i->DrawInstance(_bullet.pos, { 2.0f ,2.0f ,2.0f }, { 0.0f ,0.0f ,0.0f }, { 1,1,1,1 });
	}

	//エフェクト追加
	DirectX::XMFLOAT4 bulletColor = { 0.f,0.f,0.f,1.0f };
	DirectX::XMFLOAT4 effectColor = { 0.2f,0.2f,0.8f,1.0f };
	float effectScale = 5.f;
	bulletEffect.AddBulletEffect(_bullet.pos, B_SCALE, bulletColor, effectScale, effectColor);
}
