#include "Boss1Move3.h"
#include "../boss1/Boss1Model.h"
#include "../BaseBoss.h"
#include "GameHelper.h"
#include "Math/Easing/Easing.h"

Boss1Move3::Boss1Move3()
{
	//アニメーションセット
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::runAttack_end));
	boss->GetBaseModel()->AnimationReset();
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::runAttack_start));
	boss->GetBaseModel()->AnimationReset();

	boss->GetBaseModel()->ChangesScale(int(Boss1Model::AttachName::LowerArm_R), 10.0f, { 4.0f, 10.0f, 4.0f });

	startPos = boss->GetCenter()->GetPosition();
	endPos = boss->GetTargetPos();

	state = State::start;

	timer = std::make_unique<Engine::Timer>();
	hitTimer = std::make_unique<Engine::Timer>();

	useCollision = UseCollision::box;

	//攻撃用オブジェクト
	cube = Model::CreateFromOBJ("boss1/slashing");
	object = Object3d::Create(cube.get());
	object->SetColor({ 0.7f,0.7f, 0.7f, 0.2f });
	object->SetLight(false);

	func_.emplace_back([this] {return Start(); });
	func_.emplace_back([this] {return Move(); });
	func_.emplace_back([this] {return Attack(); });
	func_.emplace_back([this] {return End(); });
}

void Boss1Move3::Update()
{
	if (boss->GetIsWince() || boss->GetIsBreak()) {
		isEnd = true;
		return;
	}

	if (state < State::size) {
		func_[int(state)]();
	}

	timer->Update();
}

void Boss1Move3::Draw()
{
	if (state == State::attack) {
		object->Draw(ObjectBase::DrawMode::add);
	}
}

void Boss1Move3::GetAttackCollisionBox(std::vector<Box>& _info)
{
	if (object->GetColor().w < 0.1f) { return; }
	Vector3 pos= object->GetPosition();
	Vector3 scale = object->GetScale();

	Box add;
	add.point1 = { pos.x - scale.x, pos.y - scale.y, pos.z - scale.z };
	add.point2 = { pos.x + scale.x, pos.y + scale.y, pos.z + scale.z };
	_info.emplace_back(add);
}

void Boss1Move3::Start()
{
	boss->SetPlayerDirection();
	if (!boss->GetBaseModel()->GetIsAnimationEnd()) { return; }
	state = State::move;
	timer->Reset();
}

void Boss1Move3::Move()
{
	const float maxTimer = 60.0f;
	const float rate = *timer.get() / maxTimer;
	Vector3 pos = Vector3(boss->GetCenter()->GetPosition());
	pos.y = 0.0f;

	if (*timer.get() < maxTimer - 40.0f) {
		boss->SetPlayerDirection();
		endPos = boss->GetTargetPos();
	}

	pos.x = Easing::OutCubic(startPos.x, endPos.x, rate);
	pos.z = Easing::OutCubic(startPos.z, endPos.z, rate);

	boss->GetCenter()->SetPosition(pos);

	if (rate < 1.0f) { return; }
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::attack1_end));
	timer->Reset();
	state = State::attack;

	Vector3 v = endPos - startPos;
	v.y = 0.0;

	//角度
	Vector3 rota = VelocityRotate(v);
	rota.y += 90;
	object->SetRotation(rota);

	//位置
	v = v.normalize();
	v = Vector3(endPos) + v * 9.0f;
	v.y = -20.0f;
	object->SetPosition(v);

	isCollision = true;
}

void Boss1Move3::Attack()
{
	const float maxTimer = 20.0f;

	if (*timer.get() < maxTimer) {
		const float rate = *timer.get() / maxTimer;
		Vector3 pos = object->GetPosition();
		pos.y = Easing::OutCubic(-20.0f, 10.0f, rate);

		object->SetPosition(pos);
	}
	//消える
	else {
		const float rate = (*timer.get() - maxTimer) / maxTimer;
		float alp = Easing::OutQuart(0.2f, 0.0f, rate);
		object->SetColor({ 0.7f,0.7f, 0.7f, alp });
	}
	object->Update();

	if (*timer.get() < maxTimer * 2.0f) { return; }
	boss->GetBaseModel()->ChangesScale(int(Boss1Model::AttachName::LowerArm_R), 15.0f, { 4.0f, 5.0f, 4.0f });
	timer->Reset();
	state = State::end;
}

void Boss1Move3::End()
{
	if (*timer.get() < 15) { return; }
	isEnd = true;
}