#pragma once
#include "../BaseBullet.h"

/// <summary>
/// �v���C���[�΂��܂������i�ޒe�i�����ɂ�菭���U��΂�j
/// </summary>
class Boss1Bullet1 : public BaseBullet
{
public:

	struct BulletInfo {
		bool isAlive;//�o�����Ă��邩
		Vector3 pos;//���W
		Vector3 moveVec;//�ړ�����
		std::unique_ptr<Engine::Timer> timer;//�o������
		Vector3 predictionLinePoint;
	};

public:
	Boss1Bullet1();
	~Boss1Bullet1() {};

	void Update() override;

	void GetAttackCollision(std::vector<BaseAction::AttackCollision>& _info) override;

	void AddBullet();

	void BulletUpdate(BulletInfo& _bullet);

private:

	std::forward_list<BulletInfo> bullet;
};
