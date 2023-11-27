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
	bulletEffect.Init();

	hitTimer = std::make_unique<Engine::Timer>();
}

void Boss1Bullet1::Update()
{
	const float l_maxTimer = 100.0f;

	if (state == State::start) {
		Start();
	}
	else if (state == State::attack) {
		if (!boss->GetIsWince()) {
			if (*timer.get() % 5 == 0 && *timer.get() < l_maxTimer) {
				AddBullet();
			}

			if (*timer.get() > l_maxTimer - 30.0f) {
				boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::attack1_end));
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
		if (std::distance(bullet.begin(), bullet.end()) <= 0 && *timer.get() > l_maxTimer) {
			isEnd = true;
		}

		BaseBullet::Update();
	}
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
	if (!boss->GetBaseModel()->GetIsAnimationEnd()) { return; }
	state = State::attack;
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
	add.moveVec = normalVec * 15.0f;
	add.timer = std::make_unique<Engine::Timer>();
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
	float effectScale = 7.5f;
	bulletEffect.AddBulletEffect(_bullet.pos, bulletColor, effectScale, effectColor);

	for (auto& i : instanceObject) {
		if (!i->GetInstanceDrawCheck()) { continue; }
		i->DrawInstance(_bullet.pos, { 1.0f ,1.0f ,1.0f }, { 0.0f ,0.0f ,0.0f }, { 1,1,1,1 });
	}
	if (*_bullet.timer.get() > 10.0f) {
		_bullet.predictionLinePoint += _bullet.moveVec;
	}
	predictionLine->AddLine(_bullet.pos, _bullet.predictionLinePoint, 1.0f, { 1.0f,1.0f,1.0f,0.5f });
}
