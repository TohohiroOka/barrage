#pragma once
#include "../BaseBullet.h"

class Boss1Bullet2 : public BaseBullet
{
public:

	struct BulletInfo {
		bool isAlive;//�o�����Ă��邩
		bool easing;
		Vector3 nowPos;//���W
		Vector3 beforePos;
		Vector3 afterPos;
		bool isSetVec;
		Vector3 moveVec;
		float timer;//�o������
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