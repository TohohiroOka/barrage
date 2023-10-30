#include "Boss1Bullet1.h"
#include "GameHelper.h"
#include "../BaseBoss.h"

Boss1Bullet1::Boss1Bullet1()
{
	boss->GetBaseModel()->SetAnimation(1);
	model = Model::CreateFromOBJ("bullet");
	for (auto& i : instanceObject) {
		i = InstanceObject::Create(model.get());
	}
	predictionLine = std::make_unique<PredictionLine>();
	timer = std::make_unique<Engine::Timer>();
}

void Boss1Bullet1::Update()
{
	const float l_maxTimer = 100.0f;

	if (*timer.get() % 5 == 0 && *timer.get() < l_maxTimer) {
		for (int i = 0; i < 1; i++) {
			AddBullet();
		}
	}

	//çXêVèàóù
	for (std::forward_list<BulletInfo>::iterator it = bullet.begin();
		it != bullet.end(); it++) {
		BulletUpdate(*it);
	}

	//falseÇ»ÇÁè¡Ç∑
	bullet.remove_if([](BulletInfo& x) {
		return !x.isAlive;
		}
	);

	//èIóπ
	if (std::distance(bullet.begin(), bullet.end()) <= 0) {
		isEnd = true;
	}

	BaseBullet::Update();
}

void Boss1Bullet1::GetAttackCollision(std::vector<BaseAction::AttackCollision>& _info)
{
	for (std::forward_list<BulletInfo>::iterator it = bullet.begin();
		it != bullet.end(); it++) {
		BaseAction::AttackCollision add;
		add.pos = it->pos;
		add.radius = 1.0f;
		_info.emplace_back(add);
	}
}

void Boss1Bullet1::AddBullet()
{
	Vector3 bossPos = boss->GetCenter()->GetPosition();
	Vector3 targetPos = boss->GetTargetPos();

	//é˚ë©îÕàÕ
	float randomX = float(RandomInt(300) - 150) / 10.0f;
	float randomZ = float(RandomInt(300) - 150) / 10.0f;

	bossPos += {0.0f, 30.0f, 0.0f};
	targetPos += {randomX, 0.0f, randomZ};
	Vector3 vec = targetPos - bossPos;
	Vector3 normalVec = vec.normalize();

	//àÍÇ¬í«â¡
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

	for (auto& i : instanceObject) {
		if (!i->GetInstanceDrawCheck()) { continue; }
		i->DrawInstance(_bullet.pos, { 1.0f ,1.0f ,1.0f }, { 0.0f ,0.0f ,0.0f }, { 1,1,1,1 });
	}
	if (*_bullet.timer.get() > 10.0f) {
		_bullet.predictionLinePoint += _bullet.moveVec;
	}
	predictionLine->Update(_bullet.pos, _bullet.predictionLinePoint, 1.0f, { 1.0f,1.0f,1.0f,0.5f });
}
