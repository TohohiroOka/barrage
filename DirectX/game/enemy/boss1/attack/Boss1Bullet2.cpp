#include "Boss1Bullet2.h"
#include "GameHelper.h"
#include "../Boss1Model.h"
#include "../game/enemy/BaseBoss.h"
#include "Math/Easing/Easing.h"
#include <iterator>

Boss1Bullet2::Boss1Bullet2()
{
	boss->SetPlayerDirection();
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::attack2_end));
	boss->GetBaseModel()->AnimationReset();
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::attack2_start));
	boss->GetBaseModel()->AnimationReset();

	//状態
	state = State::start;

	//衝突判定
	useCollision = UseCollision::sphere;

	//オブジェクト
	model = Model::CreateFromOBJ("bullet");
	for (auto& i : instanceObject) {
		i = InstanceObject::Create(model.get());
	}

	//弾道
	predictionLine = std::make_unique<PredictionLine>();

	//タイマー系
	timer = std::make_unique<Engine::Timer>();
	hitTimer = std::make_unique<Engine::Timer>();

	//エフェクト
	bulletEffect = std::make_unique<BulletEffect>();
	bulletEffect->Init();

	//行動関数
	func_.emplace_back([this] {return Start(); });
	func_.emplace_back([this] {return Attack(); });
	func_.emplace_back([this] {return End(); });
}

void Boss1Bullet2::Update()
{
	if (!boss->GetIsWince() && !boss->GetIsBreak()) {
		if (int(state) >= 0 && int(state) < int(State::non)) {
			func_[int(state)]();
		}
	} else {
		End();
	}

	//falseなら消す
	bullet.remove_if([](BulletInfo& x) {
		return !x.isAlive;
		}
	);

	//更新処理
	BulletUpdate();

	BaseBullet::Update();
}

void Boss1Bullet2::GetAttackCollisionSphere(std::vector<Sphere>& _info)
{
	for (std::forward_list<BulletInfo>::iterator it = bullet.begin();
		it != bullet.end(); it++) {
		Sphere add;
		add.center = { it->nowPos.x,it->nowPos.y ,it->nowPos.z };
		add.radius = 1.0f;
		_info.emplace_back(add);
	}
}

void Boss1Bullet2::DeleteBullet(std::vector<int> _deleteNum)
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

void Boss1Bullet2::Start()
{
	if (!boss->GetBaseModel()->GetIsAnimationEnd()) { return; }
	state = State::attack;
}

void Boss1Bullet2::Attack()
{
	const float maxTimer = 100.0f;

	if (*timer.get() % 5 == 0 && *timer.get() < maxTimer) {
		for (int i = 0; i < 4; i++) {
			bool easing = (i % 2) == 0;
			AddBullet(easing);
		}
	}

	if (*timer.get() > maxTimer - 30.0f) {
		boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::attack1_end));
	}

	if (*timer.get() < maxTimer) { return; }
	state = State::end;
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::attack2_end));
}

void Boss1Bullet2::End()
{
	if (!boss->GetBaseModel()->GetIsAnimationEnd() || std::distance(bullet.begin(), bullet.end()) != 0) { return; }
	state = State::non;
	isEnd = true;
}

void Boss1Bullet2::AddBullet(bool _easing)
{
	Vector3 bossPos = boss->GetBaseModel()->GetPosition();
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
	add.beforePos = bossPos;
	add.afterPos = targetPos;
	add.moveVec = {};
	add.timer = std::make_unique<Engine::Timer>();
	add.predictionLinePoint.emplace_back(bossPos);
}

void Boss1Bullet2::BulletUpdate()
{
	const float maxTimer = 20.0f;
	for (auto& _bullet : bullet) {
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

		const float dist = 10.0f;
		if (_bullet.nowPos.x < -dist || _bullet.nowPos.x > moveMaxPos.x + dist ||
			_bullet.nowPos.y < -dist || _bullet.nowPos.y > moveMaxPos.y + dist ||
			_bullet.nowPos.z < -dist || _bullet.nowPos.z > moveMaxPos.z + dist) {
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
		predictionLine->AddLine(_bullet.nowPos, _bullet.predictionLinePoint[0], 1.0f, { 1.0f,1.0f,1.0f,0.5f });
		for (int i = 1; i < _bullet.predictionLinePoint.size(); i++) {
			predictionLine->AddLine(_bullet.predictionLinePoint[i - 1], _bullet.predictionLinePoint[i], 1.0f, { 1.0f,1.0f,1.0f,0.5f });
		}

		//エフェクト追加
		DirectX::XMFLOAT4 bulletColor = { 0.f,0.f,0.f,1.0f };
		DirectX::XMFLOAT4 effectColor = { 0.2f,0.2f,0.8f,1.0f };
		float effectScale = 5.f;
		bulletEffect->AddBulletEffect(_bullet.nowPos, bulletColor, effectScale, effectColor);
	}
}