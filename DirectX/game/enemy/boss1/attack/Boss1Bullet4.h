#pragma once
#include "../game/enemy/BaseAction.h"
#include "Object/3d/InstanceObject.h"
#include "../Math/Timer.h"
#include <functional>

/// <summary>
/// ��Ԏa��
/// </summary>
class Boss1Bullet4 : public BaseAction
{
public:

	struct ObjectInfo {
		bool isAlive;//�o�����Ă��邩
		Vector3 pos;//���W
		Vector3 moveVec;//�ړ�����
		Vector3 rota; 
	};

private:

	enum class State {
		start,
		attack,
		non,
	};

public:
	Boss1Bullet4();
	~Boss1Bullet4() {};

	void Update() override;

	void Draw() override;

	void FrameReset() override;

	void GetAttackCollisionSphere(std::vector<Sphere>& _info) override{};

	void GetAttackCollisionBox(std::vector<Box>& _info) override {};

	void GetAttackCollisionCapsule(std::vector<Capsule>& _info) override;

	void DeleteBullet(std::vector<int> _deleteNum) override {};

	void GetDamageInfo(int& _damageNum, int& _knockbackPower, int& _knockbackTime, bool& _isKnockbackStart) override {
		_damageNum = 5;
		_knockbackPower = 3;
		_knockbackTime = 10;
		_isKnockbackStart = true;
	}

	bool SetCameraTarget() override { return true; }

	void Start();

	void Attack();

private:

	std::unique_ptr<Model> model;
	std::array<ObjectInfo,2> object;
	std::unique_ptr<InstanceObject> instanceObject;

	//���݂̓���
	std::vector<std::function<void()>> func_;
	//���
	State state;
	//���ԋL�^
	float oldtime;
	//�o������ꏊ�i���S����̋����j
	float dist;
};