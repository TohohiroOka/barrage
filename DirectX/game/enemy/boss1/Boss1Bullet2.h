#pragma once
#include "../BaseBullet.h"
#include <functional>

/// <summary>
/// 外側に出てからプレイヤーに追従する
/// </summary>
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
		std::unique_ptr<Engine::Timer> timer;//出現時間
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

	int GetDamage()override { return 5; }

	void Start();

	void Attack();

	void End();

	void AddBullet(bool _easing);

	void BulletUpdate(BulletInfo& _bullet);

private:

	//状態
	State state;
	std::vector<std::function<void()>> func_;

	std::forward_list<BulletInfo> bullet;

};