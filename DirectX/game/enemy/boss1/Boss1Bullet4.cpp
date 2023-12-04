#include "Boss1Bullet4.h"
#include "../boss1/Boss1Model.h"
#include "GameHelper.h"
#include "../BaseBoss.h"
#include "Math/Easing/Easing.h"
#include <iterator>

Boss1Bullet4::Boss1Bullet4()
{
	useCollision = UseCollision::sphere;
	model = Model::CreateFromOBJ("slashing");
	for (auto& i : instanceObject) {
		i = InstanceObject::Create(model.get());
	}
	predictionLine = std::make_unique<PredictionLine>();
	timer = std::make_unique<Engine::Timer>();

	hitTimer = std::make_unique<Engine::Timer>();

	func_.emplace_back([this] {return Start(); });
}

void Boss1Bullet4::Update()
{
	if (int(state) >= 0 && int(state) <= int(State::non) && !boss->GetIsWince()) {
		func_[int(state)]();
	}
}

void Boss1Bullet4::GetAttackCollisionSphere(std::vector<Sphere>& _info)
{
}

void Boss1Bullet4::DeleteBullet(std::vector<int> _deleteNum)
{
}

void Boss1Bullet4::Start()
{
}