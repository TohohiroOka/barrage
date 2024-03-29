#pragma once
#include "../game/enemy/BaseAction.h"
#include "Object/3d/InstanceObject.h"
#include "../Math/Timer.h"
#include <functional>

/// <summary>
/// 近距離に円状ででる斬撃
/// </summary>
class Boss1Bullet3 : public BaseAction
{
public:

	struct ObjectInfo {
		bool isAlive;//出現しているか
		Vector3 pos;//座標
		Vector3 moveVec;//移動方向
		Vector3 rota;
		float alpha;
	};

private:

	enum class State {
		start,
		cut,
		end,
		non,
	};

public:
	Boss1Bullet3();
	~Boss1Bullet3() {};

	void Update() override;

	void Draw() override;

	void FrameReset() override;

	void GetAttackCollisionSphere(std::vector<Sphere>& _info) override{};

	void GetAttackCollisionBox(std::vector<Box>& _info) override;

	void GetAttackCollisionCapsule(std::vector<Capsule>& _info) override{};

	void DeleteBullet(std::vector<int> _deleteNum) override{};

	void GetDamageInfo(int& _damageNum, int& _knockbackPower, int& _knockbackTime, bool& _isKnockbackStart) override {
		_damageNum = 15;
		_knockbackPower = 3;
		_knockbackTime = 10;
		_isKnockbackStart = true;
	}

	bool SetCameraTarget() override { return true; }

	void Start();

	void Cut();

	void End();

	void BulletUpdate();

private:

	static const int bulletNum = 10;
	std::unique_ptr<Model> model;
	std::unique_ptr<Model> swordModel;
	std::array<ObjectInfo, bulletNum> object;
	std::array<ObjectInfo, bulletNum> swordObject;
	std::array<std::unique_ptr<InstanceObject>, 2> instanceObject;

	//状態
	State state;
	std::vector<std::function<void()>> func_;

	std::unique_ptr<Engine::Timer> timer;

	bool isSwordSound;
};