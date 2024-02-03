#pragma once
#include <vector>
#include "Math/Vector3.h"
#include "../Object/3d/collider/CollisionPrimitive.h"
#include "Math/Timer.h"

class BaseBoss;

class BaseAction
{
public:
	enum class UseCollision {
		sphere,
		box,
		capsule,
	};

	virtual ~BaseAction(){};

	virtual void Update() = 0;

	virtual void Draw() = 0;

	virtual void FrameReset() = 0;

	virtual void GetAttackCollisionSphere(std::vector<Sphere>& _info) = 0;

	virtual void GetAttackCollisionBox(std::vector<Box>& _info) = 0;

	virtual void GetAttackCollisionCapsule(std::vector<Capsule>& _info) = 0;

	virtual void DeleteBullet(std::vector<int> _deleteNum) = 0;

	virtual void GetDamageInfo(int& _damageNum, int& _knockbackPower, int& _knockbackTime, bool& _isKnockbackStart) = 0;

	virtual bool SetCameraTarget() = 0;

	void SetActionBreak() {
		if (timer) {
			timer->Reset();
		}
	}

	bool GetEnd() { return isEnd; }
	void SetEnd() { isEnd = true; }
	void SetUseCollision(const UseCollision _useCollision) { useCollision = _useCollision; }
	UseCollision GetUseCollision() { return useCollision; }
	void SetIsCollision(const bool _isCollision) {
		isCollision = _isCollision;
		hitTimer->Reset();
	}
	bool GetIsCollision() { return isCollision; }
	float GetPower() { return power; }

	static void SetBossPtr(BaseBoss* _boss) { boss = _boss; }

protected:

	//ボス本体クラス
	static BaseBoss* boss;
	//全体タイマー
	std::unique_ptr<Engine::Timer> timer;
	//行動終了
	bool isEnd = false;
	//どの当たり判定を使用するか
	UseCollision useCollision = UseCollision::box;
	//判定を行うか
	bool isCollision = false;
	//判定を行わない時間
	std::unique_ptr<Engine::Timer> hitTimer;
	//威力
	float power;
};

