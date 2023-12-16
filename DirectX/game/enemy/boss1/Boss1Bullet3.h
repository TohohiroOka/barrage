#pragma once
#include "../BaseAction.h"
#include "Object/3d/InstanceObject.h"
#include "../Math/Timer.h"
#include <functional>

/// <summary>
/// �ߋ����ɉ~��łł�a��
/// </summary>
class Boss1Bullet3 : public BaseAction
{
public:

	struct ObjectInfo {
		bool isAlive;//�o�����Ă��邩
		Vector3 pos;//���W
		Vector3 moveVec;//�ړ�����
		Vector3 rota;
		float alpha;
	};

private:

	enum class State {
		start,
		cut,
		end,
		non,
	};

public:
	Boss1Bullet3();
	~Boss1Bullet3() {};

	void Update() override;

	void Draw() override;

	void FrameReset() override;

	void GetAttackCollisionSphere(std::vector<Sphere>& _info) override{};

	void GetAttackCollisionBox(std::vector<Box>& _info) override;

	void GetAttackCollisionCapsule(std::vector<Capsule>& _info) override{};

	void DeleteBullet(std::vector<int> _deleteNum) override{};

	int GetDamage()override { return 5; }

	void Start();

	void Cut();

	void End();

	void BulletUpdate();

private:

	static const int bulletNum = 10;
	std::unique_ptr<Model> model;
	std::unique_ptr<Model> swordModel;
	std::array<ObjectInfo, bulletNum> object;
	std::array<ObjectInfo, bulletNum> swordObject;
	std::array<std::unique_ptr<InstanceObject>, 2> instanceObject;

	//���
	State state;
	std::vector<std::function<void()>> func_;

	std::unique_ptr<Engine::Timer> timer;
};

