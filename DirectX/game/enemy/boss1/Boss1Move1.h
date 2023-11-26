#pragma once
#include "../game/enemy/BaseAction.h"
#include <DirectXMath.h>
#include "../Math/Timer.h"

/// <summary>
/// ���
/// </summary>
class Boss1Move1 : public BaseAction
{
public:
	Boss1Move1();
	~Boss1Move1() {};

	void Update() override;

	void Draw() override {};

	void FrameReset() override {};

	void GetAttackCollisionSphere(std::vector<Sphere>& _info) override {};

	void GetAttackCollisionBox(std::vector<Box>& _info) override {};

	void GetAttackCollisionCapsule(std::vector<Capsule>& _info) override {};

	void DeleteBullet(std::vector<int> _deleteNum) override {};

	int GetDamage()override { return 0; }

private:

	//�C�[�W���O�^�C�}�[
	std::unique_ptr<Engine::Timer> moveTime;
	//�ړ�����
	Vector3 moveV; 

};

