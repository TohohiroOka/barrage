#pragma once
#include "../BaseBullet.h"

class Boss1Bullet2 : public BaseBullet
{
public:

	struct BulletInfo {
		bool isAlive;//出現しているか
		bool easing;
		Vector3 nowPos;//座標
		Vector3 beforePos;
		Vector3 afterPos;
		bool isSetVec;
		Vector3 moveVec;
		float timer;//出現時間
		std::vector<Vector3> predictionLinePoint;
	};

public:
	Boss1Bullet2();
	~Boss1Bullet2() {};

	void Update() override;

	void GetAttackCollision(std::vector<BaseAction::AttackCollision>& _info) override;

	void AddBullet(bool _easing);

	void BulletUpdate(BulletInfo& _bullet);

private:

	std::forward_list<BulletInfo> bullet;

};