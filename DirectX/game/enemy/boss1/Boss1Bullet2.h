#pragma once
#include "../BaseBullet.h"
#include <functional>

/// <summary>
/// �O���ɏo�Ă���v���C���[�ɒǏ]����
/// </summary>
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
		std::unique_ptr<Engine::Timer> timer;//�o������
		std::vector<Vector3> predictionLinePoint;
	};

private:

	enum class State {
		start,
		attack,
		end,
		non,
	};

public:
	Boss1Bullet2();
	~Boss1Bullet2() {};

	void Update() override;

	void GetAttackCollisionSphere(std::vector<Sphere>& _info) override;

	void GetAttackCollisionBox(std::vector<Box>& _info) override {};

	void GetAttackCollisionCapsule(std::vector<Capsule>& _info) override {};

	void DeleteBullet(std::vector<int> _deleteNum) override;

	void GetDamageInfo(int& _damageNum, int& _knockbackPower, int& _knockbackTime, bool& _isKnockbackStart) override {
		_damageNum = 5;
		_knockbackPower = 3;
		_knockbackTime = 10;
		_isKnockbackStart = true;
	}

	void Start();

	void Attack();

	void End();

	void AddBullet(bool _easing);

	void BulletUpdate();

private:

	//���
	State state;
	std::vector<std::function<void()>> func_;

	std::forward_list<BulletInfo> bullet;

};