#pragma once
#include "../game/enemy/BaseAction.h"
#include "Object/3d/InstanceObject.h"
#include "../Math/Timer.h"
#include <forward_list>
#include <functional>

/// <summary>
/// 薙ぎ払い
/// </summary>
class Boss1NearAttack1 : public BaseAction
{
private:

	enum class State {
		start,
		attack,
		finish,
		non,
	};

	struct ObjectInfo {
		bool isUp;
		std::unique_ptr<Engine::Timer> timer;
		DirectX::XMFLOAT3 pos;
		float alpha;
	};

public:
	Boss1NearAttack1();
	~Boss1NearAttack1() {};

	void Update() override;

	void Draw() override;

	void FrameReset() override;

	void GetAttackCollisionSphere(std::vector<Sphere>& _info) override {};

	void GetAttackCollisionBox(std::vector<Box>& _info) override;

	void GetAttackCollisionCapsule(std::vector<Capsule>& _info) override {};

	void DeleteBullet(std::vector<int> _deleteNum) override {};

private:

	void StartMove();

	void Attack();

	void Finish();

	void ObjectUpdate();

private:
	std::unique_ptr<Model> model;
	std::forward_list<ObjectInfo> object;
	std::array<std::unique_ptr<InstanceObject>, 5> instanceObject;

	//現在の動き
	std::vector<std::function<void()>> func_;
	//状態
	State state;
	//全体タイマー
	std::unique_ptr<Engine::Timer> timer;
	float oldtime;
	//出現する場所（中心からの距離）
	float dist;
};

