#include "Boss1Bullet1.h"
#include "../boss1/Boss1Model.h"
#include "GameHelper.h"
#include "../BaseBoss.h"
#include "Math/Easing/Easing.h"

Boss1Bullet1::Boss1Bullet1()
{
	boss->SetPlayerDirection();
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::attack1_end));
	boss->GetBaseModel()->AnimationReset();
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::attack1_start));
	boss->GetBaseModel()->AnimationReset();

	state = State::start;

	useCollision = UseCollision::sphere;
	model = Model::CreateFromOBJ("bullet");
	bulletModel = Model::CreateFromOBJ("boss1/feet");
	instanceObject[0] = InstanceObject::Create(model.get());
	for (int i = 1; i < instanceObject.size(); i++) {
		instanceObject[i] = InstanceObject::Create(bulletModel.get());
	}

	predictionLine = std::make_unique<PredictionLine>();
	timer = std::make_unique<Engine::Timer>();
	
	bulletEffect = std::make_unique<BulletEffect>();
	bulletEffect->Init();

	hitTimer = std::make_unique<Engine::Timer>();

	addBulletNum = 0;

	usePoint = { 0,0 };

	angleSpeed = 15.0f;

	func_.emplace_back([this] {return Start(); });
	func_.emplace_back([this] {return Attack(); });
	func_.emplace_back([this] {return End(); });
}

Boss1Bullet1::~Boss1Bullet1()
{
	bullet.clear();
}

void Boss1Bullet1::Update()
{
	if (int(state) >= 0 && int(state) < int(State::non)&& !boss->GetIsWince()) {
		func_[int(state)]();
	}

	for (auto& i : bulletAddPoint) {
		if (!i.isAlive || i.alpha < 0.1f) { continue; }
		instanceObject[0]->DrawInstance(i.pos, { 1.0f ,1.0f ,1.0f }, { 0.0f ,0.0f ,0.0f }, { 1,1,1,i.alpha });
	}

	//falseなら消す
	bullet.remove_if([](BulletInfo& x) {
		return !x.isAlive;
		}
	);

	for (auto& i : bullet) {
		if (!i.isAlive) { continue; }
		for (int j = 1; j < instanceObject.size(); j++) {
			if (!instanceObject[j]->GetInstanceDrawCheck()) { continue; }
			instanceObject[j]->DrawInstance(i.pos, { 1.0f ,1.0f ,1.0f }, i.rota, { 1,1,1,i.alpha });
		}
	}

	BaseBullet::Update();
}

void Boss1Bullet1::GetAttackCollisionSphere(std::vector<Sphere>& _info)
{
	for (auto& i : bullet) {
		if (i.alpha < 0.9) { continue; }
		Sphere add;
		add.center = { i.pos.x, i.pos.y , i.pos.z };
		add.radius = 1.0f;
		_info.emplace_back(add);
	}
}

void Boss1Bullet1::DeleteBullet(std::vector<int> _deleteNum)
{
	int num = -1;
	int vecNum = 0;
	for (std::forward_list<BulletInfo>::iterator it = bullet.begin();
		it != bullet.end(); it++) {
		num++;
		if (num != _deleteNum[vecNum]) { continue; }
		it->isAlive = false;
		vecNum++;
		if (_deleteNum.size() == vecNum) { break; }
	}
}

void Boss1Bullet1::Start()
{
	//出現ポイント出し
	if ((*timer.get() / 2.0f) >= addBulletNum && addBulletNum < maxBulletNum) {
		bulletAddPoint[addBulletNum].isAlive = true;
		bulletAddPoint[addBulletNum].angle = 0.0f;
		bulletAddPoint[addBulletNum].pos = {};
		bulletAddPoint[addBulletNum].alpha = 1.0f;
		addBulletNum++;
		if (addBulletNum >= maxBulletNum) {
			timer->Reset();
		}
	}

	for (auto& i : bulletAddPoint) {
		if (!i.isAlive) { continue; }
		BulletRotate(i);
	}

	const float maxTime = 50.0f;
	if (addBulletNum >= maxBulletNum) {
		angleSpeed = Easing::InExpo(15.0f,-1.0f, *timer.get() / maxTime);
	}

	if (*timer.get() < maxTime) { return; }
	timer->Reset();
	state = State::attack;
}

void Boss1Bullet1::Attack()
{
	for (auto& i : bulletAddPoint) {
		if (!i.isAlive) { continue; }
		BulletRotate(i);
	}

	const float maxTime = 120.0f;

	Vector3 targetPos = boss->GetCenter()->GetPosition();

	const float deviationWidth = 20.0f;
	usePoint[0] = int((*timer.get() / (maxTime - deviationWidth)) * (maxBulletNum - 1));
	if (*timer.get() > 20.0f) {
		usePoint[1] = int(((*timer.get() - deviationWidth) / (maxTime - deviationWidth)) * (maxBulletNum - 1));
	}

	//12以上になったら11に戻す
	if (usePoint[0] >= 12) {
		usePoint[0] = 11;
	}
	if (usePoint[1] >= 12) {
		usePoint[1] = 11;
	}

	for (int i = 0; i < 3; i++) {
		int inPoint = RandomInt(usePoint[0] - usePoint[1]) + usePoint[1];
		AddBullet(bulletAddPoint[inPoint].pos);
	}

	for (auto& i : bullet) {
		BulletUpdate(i);
	}

	if ((*timer.get()) <= maxTime) { return; }
	timer->Reset();
	state = State::end;
}

void Boss1Bullet1::End()
{
	int num = 0;
	for (auto& i : bullet) {
		BulletUpdate(i);
		num++;
	}

	const float maxTime = 30.0f;
	float bulletAddPointAlpha = Easing::InExpo(1.0f, 0.0f, *timer.get() / maxTime);

	for (auto& i : bulletAddPoint) {
		i.alpha = bulletAddPointAlpha;
	}

	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::attack1_end));

	if (num != 0 || *timer.get() < maxTime) { return; }
	isEnd = true;
}

void Boss1Bullet1::AddBullet(const Vector3& _pos)
{
	Vector3 moveVec = _pos - Vector3(boss->GetCenter()->GetPosition());
	moveVec.y = 0.0f;
	moveVec = moveVec.normalize();
	moveVec.y = -(RandomFloat(20.0f) + 50.0f) / 100.0f;

	//一つ追加
	bullet.emplace_front();
	BulletInfo& add = bullet.front();
	add.isAlive = true;
	add.pos = _pos;
	add.alpha = 1.0f;
	add.moveVec = moveVec * 2.0f;
	add.timer = std::make_unique<Engine::Timer>();
	add.rota= VelocityRotate(moveVec);
	add.rota.x += 90;
}

void Boss1Bullet1::BulletRotate(BulletAddPointInfo& _bullet)
{
	Vector3 bossPos = boss->GetCenter()->GetPosition();

	_bullet.angle += angleSpeed;
	if (_bullet.angle >= 360) {
		_bullet.angle -= 360.0f;
	}
	float radius = DirectX::XMConvertToRadians(_bullet.angle);
	_bullet.pos = bossPos + Vector3(cosf(radius) * 15.0f, 15.0f, sinf(radius) * 15.0f);
}

void Boss1Bullet1::BulletUpdate(BulletInfo& _bullet)
{
	//アルファかyが一定以下なら
	if (_bullet.alpha < 0.1f) {
		_bullet.isAlive = false;
		return;
	}
	//壁の中
	else if (_bullet.pos.x > 0.0f && _bullet.pos.x < moveMaxPos.x &&
		_bullet.pos.z > 0.0f && _bullet.pos.z < moveMaxPos.z) {
		//地面より上
		if (_bullet.pos.y > 0.0f && _bullet.pos.y < moveMaxPos.y) {
			_bullet.pos += _bullet.moveVec * GameHelper::Instance()->GetGameSpeed();
		} else {
			_bullet.alpha -= 0.04f;
		}
	}
	//壁の外
	else {
		if (_bullet.pos.y > -10.0f) {
			_bullet.pos += _bullet.moveVec * GameHelper::Instance()->GetGameSpeed();
		} else {
			_bullet.isAlive = false;
		}
	}

	////エフェクト追加
	//DirectX::XMFLOAT4 bulletColor = { 0.f,0.f,0.f,1.0f };
	//DirectX::XMFLOAT4 effectColor = { 0.2f,0.2f,0.8f,1.0f };
	//float effectScale = 7.5f;
	//bulletEffect->AddBulletEffect(_bullet.pos, bulletColor, effectScale, effectColor);

	//アルファが1でないなら移動してないため弾道は出さない
	if (_bullet.alpha != 1.0) {
		return;
	}

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
