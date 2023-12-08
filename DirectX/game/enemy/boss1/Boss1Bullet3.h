#pragma once
#include "../BaseBullet.h"
#include <functional>

/// <summary>
/// �ߋ����ɉ~��łł�e
/// </summary>
class Boss1Bullet3 : public BaseBullet
{
public:

	struct BulletInfo {
		bool isAlive;//�o�����Ă��邩
		Vector3 pos;//���W
		Vector3 moveVec;//�ړ�����
		float acceleration;
		std::unique_ptr<Engine::Timer> timer;//�o������
		Vector3 predictionLinePoint;
	};

private:

	enum class State {
		start,
		attack,
		non,
	};

public:
	Boss1Bullet3();
	~Boss1Bullet3() {};

	void Update() override;

	void GetAttackCollisionSphere(std::vector<Sphere>& _info) override;

	void GetAttackCollisionBox(std::vector<Box>& _info) override {};

	void GetAttackCollisionCapsule(std::vector<Capsule>& _info) override {};

	void DeleteBullet(std::vector<int> _deleteNum) override;

	int GetDamage()override { return 5; }

	void Start();

	void Attack();

	void AddBullet();

	void BulletUpdate(BulletInfo& _bullet);

private:

	//���
	State state;
	std::vector<std::function<void()>> func_;

	std::forward_list<BulletInfo> bullet;
};

