#include "Boss1Move3.h"
#include "../boss1/Boss1Model.h"
#include "../BaseBoss.h"
#include "GameHelper.h"
#include "Math/Easing/Easing.h"

Boss1Move3::Boss1Move3()
{
	//アニメーションセット
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::back_end));
	boss->GetBaseModel()->AnimationReset();
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::back_start));
	boss->GetBaseModel()->AnimationReset();
	boss->GetBaseModel()->SetIsRoop(false);

	startPos = boss->GetCenter()->GetPosition();
	endPos = boss->GetTargetPos();

	state = State::move;

	timer = std::make_unique<Engine::Timer>();
	hitTimer = std::make_unique<Engine::Timer>();

	useCollision = UseCollision::box;

	//攻撃用オブジェクト
	cube = Model::CreateFromOBJ("boss1/runAttackBlock");
	object = Object3d::Create(cube.get());
	object->SetScale({ 10.0f ,20.0f ,10.0f });

	func_.emplace_back([this] {return Move(); });
	func_.emplace_back([this] {return Attack(); });
}

void Boss1Move3::Update()
{
	if (boss->GetIsWince()) {
		return;
		isEnd = true;
	}

	if (state < State::size) {
		func_[int(state)]();
	}
}

void Boss1Move3::Draw()
{
	if (state == State::attack && object->GetColor().w > 0.1f) {
		object->Draw();
	}
}

void Boss1Move3::GetAttackCollisionBox(std::vector<Box>& _info)
{
	if (object->GetColor().w < 0.3f) { return; }
	Vector3 pos= object->GetPosition();
	Vector3 scale = object->GetScale();

	Box add;
	add.point1 = { pos.x - scale.x, pos.y - scale.y, pos.z - scale.z };
	add.point2 = { pos.x + scale.x, pos.y + scale.y, pos.z + scale.z };
	_info.emplace_back(add);
}

void Boss1Move3::Move()
{
	timer->Update();
	const float maxTimer = 60.0f;
	const float rate = *timer.get() / maxTimer;
	Vector3 pos = Vector3(boss->GetCenter()->GetPosition());
	pos.y = 0.0f;

	if (*timer.get() < maxTimer - 40.0f) {
		endPos = boss->GetTargetPos();
	}

	pos.x = Easing::OutCubic(startPos.x, endPos.x, rate) - pos.x;
	pos.z = Easing::OutCubic(startPos.z, endPos.z, rate) - pos.z;

	boss->SetMoveVec(pos);

	if (*timer.get() > maxTimer - 20.0f) {
		boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::attack1_end));
	}

	if (rate < 1.0f) { return; }
	timer->Reset();
	state = State::attack;

	Vector3 v = endPos - startPos;
	v.y = 0.0;
	v = v.normalize();
	v= Vector3(endPos) + v * 20.0f;
	v.y = -20.0f;
	object->SetPosition(v);

	isCollision = true;
}

void Boss1Move3::Attack()
{
	timer->Update();
	const float maxTimer = 20.0f;

	if (*timer.get() < maxTimer) {
		const float rate = *timer.get() / maxTimer;
		Vector3 pos = object->GetPosition();
		pos.y = Easing::OutCubic(-20.0f, 10.0f, rate);

		object->SetPosition(pos);
	} else {
		const float rate = (*timer.get() - maxTimer) / maxTimer;
		float alp = Easing::OutCubic(1.0f, 0.0f, rate);
		object->SetColor({ 1.0f,1.0f, 1.0f,alp });
	}
	object->Update();

	if (*timer.get() < maxTimer * 2.0f) { return; }
	timer->Reset();
	isEnd = true;
}
