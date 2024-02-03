#pragma once
#include "../BaseAction.h"
#include "../game/effect/Boss1DestroyEffect.h"

class BreakAction : public BaseAction
{
public:
	BreakAction();
	~BreakAction() {};

	void Update() override;

	void Draw() override;

	void FrameReset() override {};

	void GetAttackCollisionSphere(std::vector<Sphere>& _info) override {};

	void GetAttackCollisionBox(std::vector<Box>& _info) override {};

	void GetAttackCollisionCapsule(std::vector<Capsule>& _info) override{};

	void DeleteBullet(std::vector<int> _deleteNum) override {};

	void GetDamageInfo(int& _damageNum, int& _knockbackPower, int& _knockbackTime, bool& _isKnockbackStart) override {
		_damageNum = 5;
		_knockbackPower = 3;
		_knockbackTime = 10;
		_isKnockbackStart = true;
	}

	bool SetCameraTarget() override { return true; }

private:

	//エフェクト
	std::unique_ptr<Boss1DestroyEffect> effect;
};