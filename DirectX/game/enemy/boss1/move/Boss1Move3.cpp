#include "Boss1Move3.h"
#include "../Boss1Model.h"
#include "../game/enemy/BaseBoss.h"
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

	startPos = boss->GetBaseModel()->GetPosition();
	endPos = boss->GetTargetPos();

	state = State::start;

	timer = std::make_unique<Engine::Timer>();
	hitTimer = std::make_unique<Engine::Timer>();

	useCollision = UseCollision::box;

	DirectX::XMFLOAT4 startColor = { 0.2f, 0.1f, 0.02f, 1.0f };
	DirectX::XMFLOAT4 endColor = { 0.01f, 0.005f, 0.001f, 1.0f };
	swordEffect = std::make_unique<SlashEffect>("effect", 10, 10, 10.0f, 1.0f, 0.0f, startColor, endColor);

	func_.emplace_back([this] {return Start(); });
	func_.emplace_back([this] {return Move(); });
	func_.emplace_back([this] {return Attack(); });
	func_.emplace_back([this] {return End(); });
}

void Boss1Move3::Update()
{
	swordEffect->Update(boss->GetBaseModel()->GetPartsPos("larm1"), boss->GetBaseModel()->GetPartsPos("larm2"));

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
	swordEffect->Draw();
}

void Boss1Move3::GetAttackCollisionBox(std::vector<Box>& _info)
{
	Vector3 scale = { 1.0f,4.0f,1.0f };

	Box add;
	add.point1 = { swordPos.x - scale.x, swordPos.y - scale.y, swordPos.z - scale.z };
	add.point2 = { swordPos.x + scale.x, swordPos.y + scale.y, swordPos.z + scale.z };
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
	Vector3 pos = Vector3(boss->GetBaseModel()->GetPosition());

	if (*timer.get() < maxTimer - 40.0f) {
		boss->SetPlayerDirection();
		endPos = boss->GetTargetPos();
	}

	pos.x = Easing::OutCubic(startPos.x, endPos.x, rate);
	pos.z = Easing::OutCubic(startPos.z, endPos.z, rate);

	boss->GetBaseModel()->SetPosition(pos);

	if (rate < 1.0f) { return; }
	boss->GetBaseModel()->SetAnimation(int(Boss1Model::Movement::runAttack_end));
	timer->Reset();
	state = State::attack;

	Vector3 v = endPos - startPos;
	v.y = 0.0;

	//位置
	v = v.normalize();
	swordPos = Vector3(endPos) + v * 9.0f;
	swordPos.y = -20.0f;
}

void Boss1Move3::Attack()
{
	const float maxTimer = 40.0f;

	if (!isHit && *timer.get() >= 30.0f) {
		isCollision = true;
		isHit = true;
	}

	if (*timer.get() <= maxTimer) {
		const float rate = *timer.get() / maxTimer;
		swordPos.y = Easing::OutCubic(-20.0f, 10.0f, rate);
	}

	if (*timer.get() < maxTimer * 2.0f) { return; }
	timer->Reset();
	state = State::end;
	boss->GetBaseModel()->ModelReset();
}

void Boss1Move3::End()
{
	if (*timer.get() < 10) { return; }
	isEnd = true;
}