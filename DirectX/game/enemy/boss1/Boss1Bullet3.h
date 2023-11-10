#pragma once
#include "../BaseBullet.h"

/// <summary>
/// 近距離に円状ででる弾
/// </summary>
class Boss1Bullet3 : public BaseBullet
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

public:
	Boss1Bullet3();
	~Boss1Bullet3() {};

	void Update() override;

	void GetAttackCollisionSphere(std::vector<Sphere>& _info) override;

	void GetAttackCollisionBox(std::vector<Box>& _info) override {};

	void GetAttackCollisionCapsule(std::vector<Capsule>& _info) override {};

	void AddBullet();

	void BulletUpdate(BulletInfo& _bullet);

private:

	std::forward_list<BulletInfo> bullet;
};

