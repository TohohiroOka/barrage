#include "Boss1Bullet4.h"
#include "../boss1/Boss1Model.h"
#include "GameHelper.h"
#include "../BaseBoss.h"
#include "Math/Easing/Easing.h"

const float bulletSpeed = 5.0f;

Boss1Bullet4::Boss1Bullet4()
{
	boss->SetPlayerDirection();
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::crossCut_end));
	boss->GetBaseModel()->AnimationReset();
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::crossCut_start));
	boss->GetBaseModel()->AnimationReset();

	useCollision = UseCollision::sphere;
	model = Model::CreateFromOBJ("slashing");
	instanceObject = InstanceObject::Create(model.get());
	instanceObject->SetBloom(true);
	instanceObject->SetOutline(true);
	instanceObject->SetOutlineColor({ 0.4f,0.2f ,0.5f });

	timer = std::make_unique<Engine::Timer>();

	hitTimer = std::make_unique<Engine::Timer>();

	func_.emplace_back([this] {return Start(); });
	func_.emplace_back([this] {return Attack(); });
}

void Boss1Bullet4::Update()
{
	if (int(state) >= 0 && int(state) <= int(State::non) && !boss->GetIsWince()) {
		func_[int(state)]();
	}

	instanceObject->Update();
}

void Boss1Bullet4::Draw()
{
	instanceObject->Draw(ObjectBase::DrawMode::add);
}

void Boss1Bullet4::FrameReset()
{
	instanceObject->FrameReset();
}

void Boss1Bullet4::GetAttackCollisionSphere(std::vector<Sphere>& _info)
{
}

void Boss1Bullet4::Start()
{
	boss->SetPlayerDirection();
	if (!boss->GetBaseModel()->GetIsAnimationEnd()) { return; }

	DirectX::XMFLOAT3 pos = boss->GetCenter()->GetPosition();
	DirectX::XMFLOAT3 target = boss->GetTargetPos();

	for (int i = 0; i < 2; i++) {
		object[i].isAlive = true;
		object[i].pos = pos;
		Vector3 normal= target - pos;
		normal.y = 0.0f;
		object[i].moveVec = normal.normalize() * bulletSpeed;
		object[i].rota = VelocityRotate(object[i].moveVec);
		object[i].rota.y += 90;
	}

	state = State::attack;
	timer->Reset();
	oldtime = 0;
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::runAttack_end));
}

void Boss1Bullet4::Attack()
{
	int objectNumber = 0;
	int aliveNum = 0;
	for (auto& i : object) {
		if (!i.isAlive) { continue; }
		aliveNum++;
		i.pos += i.moveVec;

		const float dist = 10.0f;
		if (i.pos.x < -dist || i.pos.x > GameHelper::Instance()->GetStageSize() + dist ||
			i.pos.y < -dist || i.pos.y > GameHelper::Instance()->GetStageSize() + dist ||
			i.pos.z < -dist || i.pos.z > GameHelper::Instance()->GetStageSize() + dist) {
			i.isAlive = false;
			return;
		}

		float angle2 = 0.0f;
		if (objectNumber == 1) {
			angle2 = 90.0f;
		}
		instanceObject->DrawInstance(i.pos, { 1.0f,1.5f,1.0f }, { i.rota.x+ angle2,i.rota.y,i.rota.z }, { 0.4f,0.2f ,0.5f ,1.0f });
		
		for (int num = 0; num < 3; num++) {
			float colorRate = (float(num + 1) / 9.0f) + 1.0f;
			instanceObject->DrawInstance(i.pos - i.moveVec * ((num * (1.0f/bulletSpeed)) + 0.5f), { 1.0f,1.5f,1.0f }, {i.rota.x + angle2, i.rota.y,i.rota.z }, { 0.4f * colorRate,0.2f * colorRate ,0.5f * colorRate ,0.4f / colorRate });
		}

		objectNumber++;
	}

	if ((*timer.get()) / 2.0f >= oldtime) {
		boss->SetAllHitEffect({ object[0].pos.x,0.0f,object[0].pos.z });
		oldtime++;
	}

	timer->Update();

	if (aliveNum != 0) { return; }
	isEnd = true;
}
