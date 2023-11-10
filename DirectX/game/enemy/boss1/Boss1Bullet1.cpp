#include "Boss1Bullet1.h"
#include "GameHelper.h"
#include "../BaseBoss.h"

Boss1Bullet1::Boss1Bullet1()
{
	boss->GetBaseModel()->AnimationReset();
	boss->GetBaseModel()->SetAnimation(1);
	useCollision = UseCollision::sphere;
	model = Model::CreateFromOBJ("bullet");
	for (auto& i : instanceObject) {
		i = InstanceObject::Create(model.get());
	}
	predictionLine = std::make_unique<PredictionLine>();
	timer = std::make_unique<Engine::Timer>();
	bulletEffect.Init();
}

void Boss1Bullet1::Update()
{
	const float l_maxTimer = 100.0f;

	if (*timer.get() % 5 == 0 && *timer.get() < l_maxTimer) {
		for (int i = 0; i < 1; i++) {
			AddBullet();
		}
	}

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

void Boss1Bullet1::GetAttackCollisionSphere(std::vector<Sphere>& _info)
{
	for (std::forward_list<BulletInfo>::iterator it = bullet.begin();
		it != bullet.end(); it++) {
		Sphere add;
		add.center = { it->pos.x,it->pos.y ,it->pos.z };
		add.radius = 1.0f;
		_info.emplace_back(add);
	}
}

void Boss1Bullet1::AddBullet()
{
	Vector3 bossPos = boss->GetCenter()->GetPosition();
	Vector3 targetPos = boss->GetTargetPos();

	//収束範囲
	float randomX = float(RandomInt(300) - 150) / 10.0f;
	float randomZ = float(RandomInt(300) - 150) / 10.0f;

	bossPos += {0.0f, 30.0f, 0.0f};
	targetPos += {randomX, 0.0f, randomZ};
	Vector3 vec = targetPos - bossPos;
	Vector3 normalVec = vec.normalize();

	//一つ追加
	bullet.emplace_front();
	BulletInfo& add = bullet.front();
	add.isAlive = true;
	add.pos = bossPos;
	add.moveVec = normalVec * 20.0f;
	add.timer=std::make_unique<Engine::Timer>();
	add.predictionLinePoint = bossPos;

}

void Boss1Bullet1::BulletUpdate(BulletInfo& _bullet)
{
	_bullet.pos += _bullet.moveVec * GameHelper::Instance()->GetGameSpeed();

	const float dist = 10;
	if (_bullet.pos.x < -dist || _bullet.pos.x > 510.0f + dist ||
		_bullet.pos.y < -dist || _bullet.pos.y > 255.0f + dist ||
		_bullet.pos.z < -dist || _bullet.pos.z > 510.0f + dist) {
		_bullet.isAlive = false;
		return;
	}

	_bullet.timer->Update();

	//エフェクト追加
	DirectX::XMFLOAT4 bulletColor = { 0.f,0.f,0.f,1.0f };
	DirectX::XMFLOAT4 effectColor = { 0.2f,0.2f,0.8f,1.0f };
	float effectScale = 5.f;
	bulletEffect.AddBulletEffect(_bullet.pos, B_SCALE, bulletColor, effectScale, effectColor);

	for (auto& i : instanceObject) {
		if (!i->GetInstanceDrawCheck()) { continue; }
		i->DrawInstance(_bullet.pos, { 1.0f ,1.0f ,1.0f }, { 0.0f ,0.0f ,0.0f }, { 1,1,1,1 });
	}
	if (*_bullet.timer.get() > 10.0f) {
		_bullet.predictionLinePoint += _bullet.moveVec;
	}
	predictionLine->Update(_bullet.pos, _bullet.predictionLinePoint, 1.0f, { 1.0f,1.0f,1.0f,0.5f });
}
