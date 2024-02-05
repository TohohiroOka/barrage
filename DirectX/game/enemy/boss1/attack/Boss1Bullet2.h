#pragma once
#include "../game/enemy/BaseAction.h"
#include "Object/3d/InstanceObject.h"
#include <functional>
#include "../game/effect/BulletAttack2Effect.h"

/// <summary>
/// 剣をまっすぐ飛ばす
/// </summary>
class Boss1Bullet2 : public BaseAction
{
public:

	struct BulletInfo {
		bool isAlive;//出現しているか
		Vector3 pos;//座標
		Vector3 moveVec;//移動方向
		Vector3 rota;
	};

	struct OutPosInfo {
		bool isAlive;//出現しているか
		Vector3 pos;//座標
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

	//状態
	State state;
	std::vector<std::function<void()>> func_;

	Vector3 inPlayerPos;

	static const int bulletNum=100;
	std::array<BulletInfo, bulletNum> bullet;

	static const int outPosNum = 10;
	static const std::array<Vector3, outPosNum> outPos;
	std::array<OutPosInfo, outPosNum> outPosInfo;
	int nowNum;

	//弾の移動方向
	Vector3 allMoveVec;

	//距離
	float dist;

	//弾の更新を終えたか
	bool isAttackEnd;
	bool isAllBulletAlive;

	//判定用ベクトル
	Vector3 hitVec;

};