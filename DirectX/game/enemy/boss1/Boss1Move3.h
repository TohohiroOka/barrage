#pragma once
#include "../BaseAction.h"
#include <functional>
#include "Object/3d/Object3d.h"

class Boss1Move3 : public BaseAction
{
private:

	enum class State {
		move,
		attack,
		size,
	};


public:
	Boss1Move3();
	~Boss1Move3() {};

	void Update() override;

	void Draw() override;

	void FrameReset() override {};

	void GetAttackCollisionSphere(std::vector<Sphere>& _info) override {};

	void GetAttackCollisionBox(std::vector<Box>& _info) override;

	void GetAttackCollisionCapsule(std::vector<Capsule>& _info) override {};

	void DeleteBullet(std::vector<int> _deleteNum) override {};

	int GetDamage()override { return 10; }

	void Move();

	void Attack();

private:

	//開始地点
	DirectX::XMFLOAT3 startPos;
	//移動後地点
	DirectX::XMFLOAT3 endPos;
	//イージングタイマー
	std::unique_ptr<Engine::Timer> timer;
	//現在の動き
	State state;
	std::vector<std::function<void()>> func_;


	std::unique_ptr<Model> cube;
	std::unique_ptr<Object3d> object;
};

