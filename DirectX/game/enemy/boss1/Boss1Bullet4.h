#pragma once
#include "../BaseBullet.h"
#include <functional>

class Boss1Bullet4 : public BaseBullet
{
public:

	struct BulletInfo {
		bool isAlive;//出現しているか
		Vector3 pos;//座標
		Vector3 moveVec;//移動方向
		float acceleration;
		std::unique_ptr<Engine::Timer> timer;//出現時間
		Vector3 predictionLinePoint;
	};

private:

	enum class State {
		start,
		attack,
		non,
	};

public:
	Boss1Bullet4();
	~Boss1Bullet4() {};

	void Update() override;

	void GetAttackCollisionSphere(std::vector<Sphere>& _info) override;

	void GetAttackCollisionBox(std::vector<Box>& _info) override {};

	void GetAttackCollisionCapsule(std::vector<Capsule>& _info) override {};

	void DeleteBullet(std::vector<int> _deleteNum) override;

	int GetDamage()override { return 5; }

	void Start();

private:

	//状態
	State state;
	std::vector<std::function<void()>> func_;

	std::forward_list<BulletInfo> bullet;
};