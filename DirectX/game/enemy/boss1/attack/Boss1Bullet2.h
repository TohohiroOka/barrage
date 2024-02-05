#pragma once
#include "../game/enemy/BaseAction.h"
#include "Object/3d/InstanceObject.h"
#include <functional>
#include "../game/effect/BulletAttack2Effect.h"

/// <summary>
/// �����܂�������΂�
/// </summary>
class Boss1Bullet2 : public BaseAction
{
public:

	struct BulletInfo {
		bool isAlive;//�o�����Ă��邩
		Vector3 pos;//���W
		Vector3 moveVec;//�ړ�����
		Vector3 rota;
	};

	struct OutPosInfo {
		bool isAlive;//�o�����Ă��邩
		Vector3 pos;//���W
		std::unique_ptr<BulletAttack2Effect> effect;
	};

private:

	enum class State {
		start,
		attackWait,
		attack,
		end,
		non,
	};

public:
	Boss1Bullet2();
	~Boss1Bullet2() {};

	void Update() override;

	void Draw() override;

	void FrameReset() override;

	void GetAttackCollisionSphere(std::vector<Sphere>& _info) override {};

	void GetAttackCollisionBox(std::vector<Box>& _info) override {};

	void GetAttackCollisionCapsule(std::vector<Capsule>& _info) override;

	void DeleteBullet(std::vector<int> _deleteNum) override;

	void GetDamageInfo(int& _damageNum, int& _knockbackPower, int& _knockbackTime, bool& _isKnockbackStart) override {
		_damageNum = 5;
		_knockbackPower = 3;
		_knockbackTime = 10;
		_isKnockbackStart = true;
	}

	bool SetCameraTarget() override { return true; }

	void Start();

	void AttackWait();

	void Attack();

	void End();

	void OutPosEffectAdd();

	void BulletUpdate();

private:

	std::unique_ptr<Model> model;
	std::unique_ptr<InstanceObject> instanceObject;

	//���
	State state;
	std::vector<std::function<void()>> func_;

	Vector3 inPlayerPos;

	static const int bulletNum=100;
	std::array<BulletInfo, bulletNum> bullet;

	static const int outPosNum = 10;
	static const std::array<Vector3, outPosNum> outPos;
	std::array<OutPosInfo, outPosNum> outPosInfo;
	int nowNum;

	//�e�̈ړ�����
	Vector3 allMoveVec;

	//����
	float dist;

	//�e�̍X�V���I������
	bool isAttackEnd;
	bool isAllBulletAlive;

	//����p�x�N�g��
	Vector3 hitVec;

};