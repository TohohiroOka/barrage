#include "Boss1Bullet2.h"
#include "GameHelper.h"
#include "../BaseBoss.h"
#include "Math/Easing/Easing.h"
#include <iterator>

Boss1Bullet2::Boss1Bullet2()
{
	model = Model::CreateFromOBJ("bullet");
	for (auto& i : instanceObject) {
		i = InstanceObject::Create(model.get());
	}
	predictionLine = std::make_unique<PredictionLine>();
	timer = std::make_unique<Engine::Timer>();
}

void Boss1Bullet2::Update()
{
	const float maxTimer = 100.0f;

	if (*timer.get() % 5 == 0 && *timer.get() < maxTimer) {
		for (int i = 0; i < 4; i++) {
			bool easing = (i % 2) == 0;
			AddBullet(easing);
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

	timer->Update();
}

void Boss1Bullet2::GetAttackCollision(std::vector<BaseAction::AttackCollision>& _info)
{
	for (std::forward_list<BulletInfo>::iterator it = bullet.begin();
		it != bullet.end(); it++) {
		BaseAction::AttackCollision add;
		add.pos = it->nowPos;
		add.radius = 1.0f;
		_info.emplace_back(add);
	}
}

void Boss1Bullet2::AddBullet(bool _easing)
{
	Vector3 bossPos = boss->GetCenter()->GetPosition();
	Vector3 targetPos = boss->GetTargetPos();

	//収束範囲
	float randomX = float(RandomInt(300) - 150) / 10.0f;
	float randomY = float(RandomInt(300) - 150) / 10.0f;
	float randomZ = float(RandomInt(300) - 150) / 10.0f;

	bossPos += {0.0f, 30.0f, 0.0f};
	targetPos += {randomX, randomY, randomZ};

	//一つ追加
	bullet.emplace_front();
	BulletInfo& add = bullet.front();
	add.isAlive = true;
	add.easing = _easing;
	add.nowPos = bossPos;
	add.beforePos= bossPos;
	add.afterPos = targetPos;
	add.moveVec = {};
	add.timer=std::make_unique<Engine::Timer>();
	add.predictionLinePoint.emplace_back(bossPos);
}

void Boss1Bullet2::BulletUpdate(BulletInfo& _bullet)
{
	const float maxTimer = 20.0f;
	//イージング移動
	if (!_bullet.isSetVec) {
		const float l_reto = *_bullet.timer.get() / maxTimer;
		if (_bullet.easing) {
			_bullet.nowPos.x = Easing::OutCirc(_bullet.beforePos.x, _bullet.afterPos.x, l_reto);
			_bullet.nowPos.y = Easing::Lerp(_bullet.beforePos.y, _bullet.afterPos.y, l_reto);
			_bullet.nowPos.z = Easing::InCirc(_bullet.beforePos.z, _bullet.afterPos.z, l_reto);
		} else {
			_bullet.nowPos.x = Easing::InCirc(_bullet.beforePos.x, _bullet.afterPos.x, l_reto);
			_bullet.nowPos.y = Easing::Lerp(_bullet.beforePos.y, _bullet.afterPos.y, l_reto);
			_bullet.nowPos.z = Easing::OutCirc(_bullet.beforePos.z, _bullet.afterPos.z, l_reto);
		}
		//移動を変更
		if (*_bullet.timer.get() > maxTimer - 5.0f) {
			_bullet.moveVec = _bullet.afterPos - _bullet.nowPos;
			_bullet.moveVec = _bullet.moveVec.normalize() * 10.0f;
			_bullet.isSetVec = true;
		}

		_bullet.timer->Update();
	}
	//イージング途中のベクトル方向に壁に当たるまで移動
	else {
		_bullet.nowPos += _bullet.moveVec * GameHelper::Instance()->GetGameSpeed();
	}

	const float dist = 10;
	if (_bullet.nowPos.x < -dist || _bullet.nowPos.x > 510.0f + dist ||
		_bullet.nowPos.y < -dist || _bullet.nowPos.y > 255 + dist ||
		_bullet.nowPos.z < -dist || _bullet.nowPos.z > 510.0f + dist) {
		_bullet.isAlive = false;
		return;
	}

	for (auto& i : instanceObject) {
		if (!i->GetInstanceDrawCheck()) { continue; }
		i->DrawInstance(_bullet.nowPos, { 1.0f ,1.0f ,1.0f }, { 0.0f ,0.0f ,0.0f }, { 1,1,1,1 });
	}

	//弾道セット
	if (_bullet.predictionLinePoint.size() < 10) {
		_bullet.predictionLinePoint.insert(_bullet.predictionLinePoint.begin(), _bullet.nowPos);
	} else {
		for (int i = 1; i < 10; i++) {
			_bullet.predictionLinePoint[i] = _bullet.predictionLinePoint[i - 1];
		}
		_bullet.predictionLinePoint[0] = _bullet.nowPos;
	}

	//弾道描画セット
	predictionLine->Update(_bullet.nowPos, _bullet.predictionLinePoint[0], 1.0f, { 1.0f,1.0f,1.0f,0.5f });
	for (int i = 1; i < _bullet.predictionLinePoint.size(); i++) {
		predictionLine->Update(_bullet.predictionLinePoint[i - 1], _bullet.predictionLinePoint[i], 1.0f, { 1.0f,1.0f,1.0f,0.5f });
	}
}
