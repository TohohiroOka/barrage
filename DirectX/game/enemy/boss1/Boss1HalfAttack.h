#pragma once
#include "../BaseAction.h"

class Boss1HalfAttack : public BaseAction
{
public:
	Boss1HalfAttack();
	~Boss1HalfAttack() {};

	void Update() override;

	void Draw() override;

	void FrameReset() override {};

	void GetAttackCollisionSphere(std::vector<Sphere>& _info) override {};

	void GetAttackCollisionBox(std::vector<Box>& _info) override {};

	void GetAttackCollisionCapsule(std::vector<Capsule>& _info) override {};

	void DeleteBullet(std::vector<int> _deleteNum) override {};

	int GetDamage()override { return 0; }

private:

	//イージングタイマー
	std::unique_ptr<Engine::Timer> moveTime;
	//移動方向
	Vector3 moveV;
};

