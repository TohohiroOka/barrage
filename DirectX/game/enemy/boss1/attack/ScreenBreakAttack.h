#pragma once
#include "../Object/3d/InstanceObject.h"
#include "../Object/3d/PrimitiveObject3D.h"
#include "../game/enemy/BaseAction.h"
#include <functional>

class ScreenBreakAttack : public BaseAction
{
private:

	enum class State {
		fieldMove,
		move1Start,
		move1End,
		attackWait,
		attack,
		move2Start,
		move2End,
		non,
	};


public:
	ScreenBreakAttack();
	~ScreenBreakAttack() {};

	void Update() override;

	void Draw() override{};

	void FrameReset() override {};

	void GetAttackCollisionSphere(std::vector<Sphere>& _info) override {};

	void GetAttackCollisionBox(std::vector<Box>& _info) override {};

	void GetAttackCollisionCapsule(std::vector<Capsule>& _info) override;

	void DeleteBullet(std::vector<int> _deleteNum) override {};

	void GetDamageInfo(int& _damageNum, int& _knockbackPower, int& _knockbackTime, bool& _isKnockbackStart) override {
		_damageNum = 50;
		_knockbackPower = 0;
		_knockbackTime = 0;
		_isKnockbackStart = false;
	}

private:

	void FieldMove();

	void Move1Start();

	void Move1End();

	void AttackWait();

	void Attack();

	void Move2Start();

	void Move2End();

private:

	//状態
	State state;
	std::vector<std::function<void()>> func_;

	//全体タイマー
	std::unique_ptr<Engine::Timer> timer;

	bool panelBreak;

	//移動前
	Vector3 moveBefore;
};