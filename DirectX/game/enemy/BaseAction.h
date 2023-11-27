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

	virtual int GetDamage() = 0;

	bool End() { return isEnd; }
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

	//�{�X�{�̃N���X
	static BaseBoss* boss;
	//�s���I��
	bool isEnd = false;
	//�ǂ̓����蔻����g�p���邩
	UseCollision useCollision = UseCollision::box;
	//������s����
	bool isCollision = false;
	//������s��Ȃ�����
	std::unique_ptr<Engine::Timer> hitTimer;
	//�З�
	float power;

};

