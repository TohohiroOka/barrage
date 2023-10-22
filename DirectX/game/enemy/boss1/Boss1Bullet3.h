#pragma once
#include "../BaseBullet.h"

/// <summary>
/// ‹ß‹——£‚É‰~ó‚Å‚Å‚é’e
/// </summary>
class Boss1Bullet3 : public BaseBullet
{
public:

	struct BulletInfo {
		bool isAlive;//oŒ»‚µ‚Ä‚¢‚é‚©
		Vector3 pos;//À•W
		Vector3 moveVec;//ˆÚ“®•ûŒü
		float acceleration;
		std::unique_ptr<Engine::Timer> timer;//oŒ»ŠÔ
		Vector3 predictionLinePoint;
	};

public:
	Boss1Bullet3();
	~Boss1Bullet3() {};

	void Update() override;

	void GetAttackCollision(std::vector<BaseAction::AttackCollision>& _info) override;

	void AddBullet();

	void BulletUpdate(BulletInfo& _bullet);

private:

	std::forward_list<BulletInfo> bullet;
};

