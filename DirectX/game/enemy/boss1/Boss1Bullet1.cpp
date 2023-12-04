#include "Boss1Bullet1.h"
#include "../boss1/Boss1Model.h"
#include "GameHelper.h"
#include "../BaseBoss.h"

Boss1Bullet1::Boss1Bullet1()
{
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::attack1_end));
	boss->GetBaseModel()->AnimationReset();
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::attack1_start));
	boss->GetBaseModel()->AnimationReset();

	boss->GetBaseModel()->SetIsRoop(false);

	state = State::start;

	useCollision = UseCollision::sphere;
	model = Model::CreateFromOBJ("bullet");
	for (auto& i : instanceObject) {
		i = InstanceObject::Create(model.get());
	}

	predictionLine = std::make_unique<PredictionLine>();
	timer = std::make_unique<Engine::Timer>();
	
	bulletEffect = std::make_unique<BulletEffect>();
	bulletEffect->Init();

	hitTimer = std::make_unique<Engine::Timer>();

	addBulletNum = 0;

	func_.emplace_back([this] {return Start(); });
	func_.emplace_back([this] {return Attack(); });
	func_.emplace_back([this] {return End(); });
}

void Boss1Bullet1::Update()
{
	if (int(state) >= 0 && int(state) <= int(State::non)&& !boss->GetIsWince()) {
		func_[int(state)]();
	}

	for (auto& i : bullet) {
		if (!i.isAlive) { continue; }
		for (auto& inst : instanceObject) {
			if (!inst->GetInstanceDrawCheck()) { continue; }
			inst->DrawInstance(i.pos, { 1.0f ,1.0f ,1.0f }, { 0.0f ,0.0f ,0.0f }, { 1,1,1,1 });
		}
	}

	BaseBullet::Update();
}

void Boss1Bullet1::GetAttackCollisionSphere(std::vector<Sphere>& _info)
{
	for (auto& i : bullet) {
	if(i.isShot){}
		Sphere add;
		add.center = { i.pos.x, i.pos.y , i.pos.z };
		add.radius = 1.0f;
		_info.emplace_back(add);
	}
}

void Boss1Bullet1::DeleteBullet(std::vector<int> _deleteNum)
{
	for (auto& i : _deleteNum) {
		bullet[i].isAlive = false;
	}
}

void Boss1Bullet1::Start()
{
	if ((*timer.get() / 2.0f) >= addBulletNum && addBulletNum < maxBulletNum) {
		bullet[addBulletNum].isAlive = true;
		bullet[addBulletNum].isShot = false;
		bullet[addBulletNum].angle = 0.0f;
		bullet[addBulletNum].pos = {};
		bullet[addBulletNum].timer = std::make_unique<Engine::Timer>();
		bullet[addBulletNum].nowIntTime = 0;
		addBulletNum++;
	}

	for (auto& i : bullet) {
		if (!i.isAlive) { continue; }
		BulletRotate(i);
	}

	if (*timer.get() < 100.0f) { return; }
	addBulletNum = 0;
	timer->Reset();
	state = State::attack;
}

void Boss1Bullet1::Attack()
{
	Vector3 targetPos = boss->GetTargetPos();

	if (*timer.get() >= addBulletNum) {
		bullet[addBulletNum].isShot = true;
		float radius = DirectX::XMConvertToRadians(bullet[addBulletNum].angle);
		bullet[addBulletNum].moveVec = Vector3(targetPos - bullet[addBulletNum].pos).normalize() * 10.0f;
		bullet[addBulletNum].predictionLinePoint[0] = bullet[addBulletNum].pos;
		addBulletNum++;
	}

	for (auto& i : bullet) {
		if (!i.isAlive) { continue; }
		if (i.isShot) {
			BulletUpdate(i);
		} else {
			BulletRotate(i);
		}
	}

	if (addBulletNum < maxBulletNum) { return; }
	addBulletNum = 0;
	timer->Reset();
	state = State::end;
}

void Boss1Bullet1::End(){
	int num = 0;
	for (auto& i : bullet) {
		if (!i.isAlive|| !i.isShot) { continue; }
		BulletUpdate(i);
		num++;
	}

	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::attack1_end));
	if (num != 0) { return; }
	isEnd = true;
}

void Boss1Bullet1::BulletRotate(BulletInfo& _bullet)
{
	Vector3 bossPos = boss->GetCenter()->GetPosition();

	_bullet.angle += 10.0f;
	if (_bullet.angle >= 360) {
		_bullet.angle -= 360.0f;
	}
	float radius = DirectX::XMConvertToRadians(_bullet.angle);
	_bullet.pos = bossPos + Vector3(sinf(radius) * 15.0f, 15.0f, cosf(radius) * 15.0f);
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

	//エフェクト追加
	DirectX::XMFLOAT4 bulletColor = { 0.f,0.f,0.f,1.0f };
	DirectX::XMFLOAT4 effectColor = { 0.2f,0.2f,0.8f,1.0f };
	float effectScale = 7.5f;
	bulletEffect->AddBulletEffect(_bullet.pos, bulletColor, effectScale, effectColor);

	//弾道
	{
		if (_bullet.nowIntTime >= 1.0f) {
			for (int i = 1; i < _bullet.predictionLinePoint.size(); i++) {
				_bullet.predictionLinePoint[i] = _bullet.predictionLinePoint[i - 1];
			}
		}

		int nowline = _bullet.nowIntTime;
		if (*_bullet.timer.get() >= _bullet.nowIntTime) {
			_bullet.predictionLinePoint[0] = _bullet.pos;
			_bullet.nowIntTime++;
		}

		if (nowline >= _bullet.predictionLinePoint.size()) {
			nowline = int(_bullet.predictionLinePoint.size()) - 2;
		}
		_bullet.timer->Update();

		for (int i = 0; i < nowline; i++) {
			predictionLine->AddLine(_bullet.predictionLinePoint[i], _bullet.predictionLinePoint[i + 1], 1.0f, { 1.0f,1.0f,1.0f,0.5f });
		}
	}
}
