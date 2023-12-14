#pragma once
#include "../BaseAction.h"
#include "Object/3d/InstanceObject.h"
#include "../Math/Timer.h"
#include <functional>

/// <summary>
/// 飛ぶ斬撃
/// </summary>
class Boss1Bullet4 : public BaseAction
{
public:

	struct ObjectInfo {
		bool isAlive;//出現しているか
		Vector3 pos;//座標
		Vector3 moveVec;//移動方向
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

	int GetDamage()override { return 5; }

	void Start();

	void Attack();

private:

	std::unique_ptr<Model> model;
	std::array<ObjectInfo,2> object;
	std::unique_ptr<InstanceObject> instanceObject;

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