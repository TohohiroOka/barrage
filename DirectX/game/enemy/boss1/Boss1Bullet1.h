#pragma once
#include "../BaseBullet.h"
#include <functional>

/// <summary>
/// �v���C���[�΂��܂������i�ޒe�i�����ɂ�菭���U��΂�j
/// </summary>
class Boss1Bullet1 : public BaseBullet
{
public:

	struct BulletInfo {
		bool isAlive;//�o�����Ă��邩
		bool isShot;//���˂���Ă��邩
		Vector3 pos;//���W
		float angle;
		Vector3 moveVec;//�ړ�����
		std::unique_ptr<Engine::Timer> timer;
		int nowIntTime;
		std::array<Vector3, 3> predictionLinePoint;
	};

private:

	enum class State {
		start,
		attack,
		end,
		non,
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

private:

	void Start();

	void Attack();

	void End();

	void BulletRotate(BulletInfo& _bullet);

	void BulletUpdate(BulletInfo& _bullet);

private:

	//���
	State state;
	std::vector<std::function<void()>> func_;

	static const int maxBulletNum = 20;
	int addBulletNum;
	std::array<BulletInfo, maxBulletNum> bullet;

	DirectX::XMFLOAT4 bulletColor = { 1.0f,1.0f,1.0f,1.0f };
	DirectX::XMFLOAT4 effectColor = { 1.0f,0.3f,0.3f,0.3f };
};

