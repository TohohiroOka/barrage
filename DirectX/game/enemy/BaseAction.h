#pragma once
#include <vector>
#include "Math/Vector3.h"
#include "../Object/3d/collider/CollisionPrimitive.h"

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

	bool End() { return isEnd; }
	void SetUseCollision(UseCollision _useCollision) { useCollision = _useCollision; }
	UseCollision GetUseCollision() { return useCollision; }

	static void SetBossPtr(BaseBoss* _boss) { boss = _boss; }

protected:

	//ボス本体クラス
	static BaseBoss* boss;
	//行動終了
	bool isEnd = false;
	//どの当たり判定を使用するか
	UseCollision useCollision= UseCollision::box;
};

