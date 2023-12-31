#include "Boss1HalfAttack.h"
#include "../boss1/Boss1Model.h"
#include "GameHelper.h"
#include "../BaseBoss.h"
#include "Math/Easing/Easing.h"

Boss1HalfAttack::Boss1HalfAttack()
{
	boss->SetPlayerDirection();
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::attack1_end));
	boss->GetBaseModel()->AnimationReset();
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::attack1_start));
	boss->GetBaseModel()->AnimationReset();

	//状態
	state = State::start;

	//衝突判定
	useCollision = UseCollision::capsule;
	line = PrimitiveObject3D::Create();

	std::array<float, ringNum> numberY = { 2,-15,11, -6,19 };

	for (int ring = 0; ring < ringNum; ring++) {
		ringInfo[ring].dist = 0.0f;
		ringInfo[ring].maxDist = 20.0f + ring * 15.0f;
		for (int sword = 0; sword < ringInfo[ring].swordObject.size(); sword++) {
			ringInfo[ring].swordObject[sword].angle = {
			360.0f / ringInfo[ring].swordObject.size() * sword,
			360.0f / ringInfo[ring].swordObject.size() * numberY[ring] };
		}
	}

	model = Model::CreateFromOBJ("boss1/feet");
	instanceObject = InstanceObject::Create(model.get());

	timer = std::make_unique<Engine::Timer>();
	hitTimer = std::make_unique<Engine::Timer>();

	isCollision = true;

	//行動関数
	func_.emplace_back([this] {return Start(); });
	func_.emplace_back([this] {return Attack(); });
	func_.emplace_back([this] {return End(); });
}

void Boss1HalfAttack::Update()
{
	if (int(state) >= 0 && int(state) < int(State::non)) {
		func_[int(state)]();
	}

	instanceObject->Update();

	for (auto& i : ringInfo) {
		for (int j = 0; j < i.linePos.size(); j++) {
			line->SetVertex(i.linePos[j]);
			if (j < i.linePos.size() - 1) {
				line->SetVertex(i.linePos[j + 1]);
			} else {
				line->SetVertex(i.linePos[0]);
			}
		}
	}

	line->VertexInit();
	line->Update();
	timer->Update();

	if (!isCollision) {
		hitTimer->Update();
		//判定を取らない時間以上になったら戻す
		if (*hitTimer.get() > 2) {
			isCollision = true;
		}
	}
}

void Boss1HalfAttack::Draw()
{
	instanceObject->Draw();

	line->Draw();
}

void Boss1HalfAttack::GetAttackCollisionCapsule(std::vector<Capsule>& _info)
{
	Vector3 bossPos = boss->GetCenter()->GetPosition();
	for (auto& i : ringInfo) {
		for (auto& j : i.swordObject) {
			Vector3 vec = j.pos - bossPos;
			vec=vec.normalize();
			Capsule add;
			add.startPosition=j.pos;
			add.endPosition = j.pos + vec * 1.0f;
			add.radius = 1.0f;
			_info.emplace_back(add);
		}
	}
}

void Boss1HalfAttack::Start()
{
	const float maxTime = 80.0f;
	const float rate = *timer.get() / maxTime;
	const Vector3 bossPos = boss->GetCenter()->GetPosition();

	int swordNum = 0;
	for (auto& ring : ringInfo) {
		ring.dist = Easing::OutCubic(0.0f, ring.maxDist, rate);
		for (auto& sword : ring.swordObject) {
			Vector3 vec = GetMoveVec(sword, ring.dist);

			sword.pos = bossPos + vec;

			ring.linePos[swordNum] = sword.pos;

			sword.rota= VelocityRotate(vec);
			sword.rota.x += 90;

			instanceObject->DrawInstance(sword.pos, { 1.0f,1.0f, 1.0f }, sword.rota, { 1.0f,1.0f, 1.0f,1.0f });
			swordNum++;
		}
		swordNum = 0;
	}

	if (rate < 1.0f) { return; }
	state = State::attack;
	timer->Reset();
}

void Boss1HalfAttack::Attack()
{
	const float maxTime = 80.0f;
	const Vector3 bossPos = boss->GetCenter()->GetPosition();

	int swordNum = 0;
	for (auto& ring : ringInfo) {
		for (auto& sword : ring.swordObject) {
			Vector3 vec = GetMoveVec(sword, ring.dist);
			sword.pos = bossPos + vec;

			ring.linePos[swordNum] = sword.pos;

			sword.rota = VelocityRotate(vec);
			sword.rota.x += 90;

			instanceObject->DrawInstance(sword.pos, { 1.0f,1.0f, 1.0f }, sword.rota, { 1.0f,1.0f, 1.0f,1.0f });
			swordNum++;
		}
		swordNum = 0;
	}

	//if (*timer.get() < maxTime) { return; }
	//state = State::end;
	//timer->Reset();
	//boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::attack1_end));
}

void Boss1HalfAttack::End()
{
	const float maxTime = 80.0f;
	const float rate = *timer.get() / maxTime;
	const Vector3 bossPos = boss->GetCenter()->GetPosition();

	int swordNum = 0;
	for (auto& ring : ringInfo) {
		ring.dist = Easing::OutCubic(ring.maxDist, 0.0f, rate);
		for (auto& sword : ring.swordObject) {
			Vector3 vec = GetMoveVec(sword, ring.dist);
			sword.pos = bossPos + vec;

			ring.linePos[swordNum] = sword.pos;

			sword.rota = VelocityRotate(vec);
			sword.rota.x += 90;

			instanceObject->DrawInstance(sword.pos, { 1.0f,1.0f, 1.0f }, sword.rota, { 1.0f,1.0f, 1.0f,1.0f });
			swordNum++;
		}
		swordNum = 0;
	}

	if (rate < 1.0f) { return; }
	state = State::non;
	isEnd = true;
}

Vector3 Boss1HalfAttack::GetMoveVec(SwordInfo& info, float dist)
{
	using namespace DirectX;
	info.angle.x+=5.0f;
	info.angle.y++;
	if (info.angle.x >= 360.0f) { info.angle.x -= 360.0f; }
	if (info.angle.y >= 360.0f) { info.angle.y -= 360.0f; }

	Vector2 radius = { XMConvertToRadians(info.angle.x),XMConvertToRadians(info.angle.y) };

	return Vector3(cosf(radius.x) * cosf(radius.y) * dist,
		cosf(radius.x) * sinf(radius.y) * dist,
		sinf(radius.x) * dist);
}
