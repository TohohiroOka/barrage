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

	void GetAttackCollisionSphere(std::vector<Sphere>& _info) override;

	void GetAttackCollisionBox(std::vector<Box>& _info) override {};

	void GetAttackCollisionCapsule(std::vector<Capsule>& _info) override {};

	void DeleteBullet(std::vector<int> _deleteNum) override;

	int GetDamage()override { return 5; }

	void AddBullet();

	void BulletUpdate(BulletInfo& _bullet);

private:

	std::forward_list<BulletInfo> bullet;

	DirectX::XMFLOAT4 bulletColor = { 1.0f,1.0f,1.0f,1.0f };
	DirectX::XMFLOAT4 effectColor = { 1.0f,0.3f,0.3f,0.3f };
};

