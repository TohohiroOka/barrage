#include "Boss1Bullet4.h"
#include "../boss1/Boss1Model.h"
#include "GameHelper.h"
#include "../BaseBoss.h"
#include "Math/Easing/Easing.h"
#include "../game/effect/AllHitEffect.h"

const float bulletSpeed = 5.0f;

Boss1Bullet4::Boss1Bullet4()
{
	boss->SetPlayerDirection();
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::crossCut_end));
	boss->GetBaseModel()->AnimationReset();
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::crossCut_start));
	boss->GetBaseModel()->AnimationReset();

	//状態
	state = State::start;

	//衝突判定
	useCollision = UseCollision::capsule;

	//オブジェクト
	model = Model::CreateFromOBJ("boss1/slashing");
	instanceObject = InstanceObject::Create(model.get());
	instanceObject->SetBloom(true);
	instanceObject->SetOutlineColor({ 0.4f,0.2f ,0.5f });

	//タイマー系
	timer = std::make_unique<Engine::Timer>();
	hitTimer = std::make_unique<Engine::Timer>();

	//行動関数
	func_.emplace_back([this] {return Start(); });
	func_.emplace_back([this] {return Attack(); });
}

void Boss1Bullet4::Update()
{
	if (!boss->GetIsWince() && !boss->GetIsBreak()) {
		if (int(state) >= 0 && int(state) <= int(State::non)) {
			func_[int(state)]();
		}
	} else if (state == State::attack) {
		func_[int(state)]();
	} else {
		isEnd = true;
		return;
	}

	instanceObject->Update();

	timer->Update();
}

void Boss1Bullet4::Draw()
{
	instanceObject->Draw(ObjectBase::DrawMode::add);
}

void Boss1Bullet4::FrameReset()
{
	instanceObject->FrameReset();
}

void Boss1Bullet4::GetAttackCollisionCapsule(std::vector<Capsule>& _info)
{
	using namespace DirectX;
	int num = 0;
	for (auto& i : object) {
		Capsule add;
		// スケール、回転、平行移動行列の計算
		DirectX::XMMATRIX matRot = XMMatrixIdentity();
		matRot *= XMMatrixRotationZ(XMConvertToRadians(i.rota.z - object[0].rota.z));
		matRot *= XMMatrixRotationX(XMConvertToRadians(i.rota.x - object[0].rota.x));
		matRot *= XMMatrixRotationY(XMConvertToRadians(i.rota.y - object[0].rota.y));

		XMFLOAT3 spos = { i.pos.x - 2.0f,i.pos.y ,i.pos.z + 2.0f };;
		XMFLOAT3 epos = { i.pos.x - 2.0f,i.pos.y + 20.0f ,i.pos.z + 2.0f };;
		XMVECTOR matSPos = XMVector3Transform({ spos.x,spos.y,spos.z }, matRot);
		XMVECTOR matEPos = XMVector3Transform({ epos.x,epos.y,epos.z }, matRot);

		add.startPosition = XMFLOAT3{ matSPos.m128_f32[0], matSPos.m128_f32[1], matSPos.m128_f32[2] };
		add.endPosition = XMFLOAT3{ matEPos.m128_f32[0], matEPos.m128_f32[1], matEPos.m128_f32[2] };
		add.radius = 10.0f;
		_info.emplace_back(add);
	}
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
		if (i == 1) {
			object[i].rota.x += 90.0f;
		}
		object[i].rota.y += 90;
	}

	state = State::attack;
	timer->Reset();
	oldtime = 0;
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::runAttack_end));
	isCollision = true;
}

void Boss1Bullet4::Attack()
{
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

		instanceObject->DrawInstance(i.pos, { 1.0f,1.5f,1.0f }, { i.rota.x,i.rota.y,i.rota.z }, { 0.4f,0.2f ,0.5f ,1.0f });
		
		for (int num = 0; num < 3; num++) {
			float colorRate = (float(num + 1) / 9.0f) + 1.0f;
			instanceObject->DrawInstance(i.pos - i.moveVec * ((num * (1.0f/bulletSpeed)) + 0.5f), { 1.0f,1.5f,1.0f }, {i.rota.x, i.rota.y,i.rota.z }, { 0.4f * colorRate,0.2f * colorRate ,0.5f * colorRate ,0.4f / colorRate });
		}
	}

	if ((*timer.get()) / 2.0f >= oldtime) {
		AllHitEffect::Instance()->AddParticle({ object[0].pos.x,0.0f,object[0].pos.z });
		oldtime++;
	}

	if (aliveNum != 0) { return; }
	isEnd = true;
}