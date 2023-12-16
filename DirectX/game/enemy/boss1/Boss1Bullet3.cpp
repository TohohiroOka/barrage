#include "Boss1Bullet3.h"
#include "../boss1/Boss1Model.h"
#include "GameHelper.h"
#include "../BaseBoss.h"
#include "Math/Easing/Easing.h"
#include "../game/effect/AllHitEffect.h"

const float bulletSpeed = 5.0f;

Boss1Bullet3::Boss1Bullet3()
{
	boss->SetPlayerDirection();
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::attack2_end));
	boss->GetBaseModel()->AnimationReset();
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::attack2_start));
	boss->GetBaseModel()->AnimationReset();

	//状態
	state = State::start;

	//衝突判定
	useCollision = UseCollision::capsule;

	//オブジェクト
	swordModel = Model::CreateFromOBJ("boss1/feet");;
	instanceObject[0] = InstanceObject::Create(swordModel.get());
	instanceObject[0]->SetOutlineColor({ 0.4f,0.2f ,0.5f });
	model = Model::CreateFromOBJ("boss1/slashing");
	instanceObject[1] = InstanceObject::Create(model.get());
	instanceObject[1]->SetBloom(true);
	instanceObject[1]->SetOutlineColor({ 0.4f,0.2f ,0.5f });

	//タイマー系
	timer = std::make_unique<Engine::Timer>();
	hitTimer = std::make_unique<Engine::Timer>();

	const float dist = 20.0f;
	const Vector3 bossPos = boss->GetCenter()->GetPosition();
	for (int i = 0; i < bulletNum; i++) {
		float radius = DirectX::XMConvertToRadians(360.0f / 10.0f * float(i));
		Vector3 vec = { cosf(radius) ,0.0f,sinf(radius) };
		vec = vec.normalize();

		object[i].isAlive = false;
		object[i].pos = bossPos + vec * dist;
		object[i].moveVec = vec * bulletSpeed;
		object[i].rota = VelocityRotate(object[i].moveVec);
		object[i].rota.y += 90;
		object[i].alpha = 0.0f;

		swordObject[i].isAlive = true;
		swordObject[i].pos = { object[i].pos.x,object[i].pos.y + 50.0f,object[i].pos.z };
		swordObject[i].rota = object[i].rota;
		swordObject[i].alpha = 0.0f;
	}

	//行動関数
	func_.emplace_back([this] {return Start(); });
	func_.emplace_back([this] {return Cut(); });
	func_.emplace_back([this] {return End(); });
}

void Boss1Bullet3::Update()
{
	if (int(state) >= 0 && int(state) <= int(State::non) && !boss->GetIsWince()) {
		func_[int(state)]();
	}

	for (auto& i : instanceObject) {
		i->Update();
	}

	timer->Update();
}

void Boss1Bullet3::Draw()
{
	instanceObject[0]->Draw();
	instanceObject[1]->Draw(ObjectBase::DrawMode::add);
}

void Boss1Bullet3::FrameReset()
{
	for (auto& i : instanceObject) {
		i->FrameReset();
	}
}

void Boss1Bullet3::GetAttackCollisionBox(std::vector<Box>& _info)
{
	for (auto& i : object) {
		Box add;
		add.point1 = { i.pos.x - 1.0f, i.pos.y - 5.0f, i.pos.z - 1.0f };
		add.point2 = { i.pos.x + 1.0f, i.pos.y - 5.0f, i.pos.z + 1.0f };
		_info.emplace_back(add);
	}
}

void Boss1Bullet3::Start()
{
	const float maxTime = 50;
	const float rate = *timer.get() / maxTime;
	for (auto& i : swordObject) {
		i.pos.y = Easing::OutCubic(50.0f, 10.0f, rate);
		i.alpha = Easing::OutSine(0.0f, 1.0f, rate);
		instanceObject[0]->DrawInstance(i.pos, { 1.0f,2.0f,1.0f }, { i.rota.x,i.rota.y,i.rota.z }, { 1.0f,1.0f,1.0f ,i.alpha });
	}


	if (!boss->GetBaseModel()->GetIsAnimationEnd() || *timer.get() < maxTime) { return; }
	state = State::cut;
	timer->Reset();
}

void Boss1Bullet3::Cut()
{
	if (*timer.get() <= 20) {
		const float maxTime = 20;
		const float rate = *timer.get() / maxTime;
		for (auto& i : swordObject) {
			i.rota.z = Easing::InExpo(0.0f, 180.0f, rate);
			instanceObject[0]->DrawInstance(i.pos, { 1.0f,2.0f,1.0f }, { i.rota.x,i.rota.y,i.rota.z }, { 1.0f,1.0f,1.0f ,i.alpha });
		}
		if (*timer.get() < maxTime - 3) { return; }
		for (auto& i : object) {
			i.isAlive = true;
		}
	} else {
		const float maxTime = 20;
		const float rate = (*timer.get() - 10.0f) / maxTime;
		for (auto& i : swordObject) {
			i.alpha = Easing::InCubic(1.0f, 0.0f, rate);
			instanceObject[0]->DrawInstance(i.pos, { 1.0f,2.0f,1.0f }, { i.rota.x,i.rota.y,i.rota.z }, { 1.0f,1.0f,1.0f ,i.alpha });
		}
	}

	BulletUpdate();

	if (*timer.get() < 40) { return; }
	timer->Reset();
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::attack2_end));
	state = State::end;
}

void Boss1Bullet3::End()
{
	BulletUpdate();

	int aliveNum = 0;
	for (auto& i : object) {
		if (!i.isAlive) { continue; }
		aliveNum++;
	}

	if (aliveNum != 0) { return; }
	timer->Reset();
	state = State::non;;
	isEnd = true;
}

void Boss1Bullet3::BulletUpdate()
{
	for (auto& i : object) {
		if (!i.isAlive) { continue; }
		i.pos += i.moveVec;

		const float dist = 10.0f;
		if (i.pos.x < -dist || i.pos.x > GameHelper::Instance()->GetStageSize() + dist ||
			i.pos.y < -dist || i.pos.y > GameHelper::Instance()->GetStageSize() + dist ||
			i.pos.z < -dist || i.pos.z > GameHelper::Instance()->GetStageSize() + dist) {
			i.isAlive = false;
			continue;
		}

		instanceObject[1]->DrawInstance(i.pos, { 1.0f,1.5f,1.0f }, { i.rota.x,i.rota.y,i.rota.z }, { 0.4f,0.2f ,0.5f ,1.0f });

		for (int num = 0; num < 3; num++) {
			float colorRate = (float(num + 1) / 9.0f) + 1.0f;
			instanceObject[1]->DrawInstance(i.pos - i.moveVec * ((num * (1.0f / bulletSpeed)) + 0.5f), { 1.0f,1.5f,1.0f }, { i.rota.x, i.rota.y,i.rota.z }, { 0.4f * colorRate,0.2f * colorRate ,0.5f * colorRate ,0.4f / colorRate });
		}

		AllHitEffect::Instance()->AddParticle({ i.pos.x,0.0f,i.pos.z });
	}

}