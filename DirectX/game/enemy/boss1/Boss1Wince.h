#pragma once
#include "../BaseAction.h"
#include "Math/Timer.h"

/// <summary>
/// ‹¯‚Ý
/// </summary>
class Boss1Wince : public BaseAction
{
public:
	Boss1Wince();
	~Boss1Wince() {};

	void Update() override;

	void Draw() override {};

	void FrameReset() override {};

	void GetAttackCollisionSphere(std::vector<Sphere>& _info) override {};

	void GetAttackCollisionBox(std::vector<Box>& _info) override {};

	void GetAttackCollisionCapsule(std::vector<Capsule>& _info) override {};

	void DeleteBullet(std::vector<int> _deleteNum) override {};

	int GetDamage()override { return 0; }

private:

	std::unique_ptr<Engine::Timer> timer;
};

