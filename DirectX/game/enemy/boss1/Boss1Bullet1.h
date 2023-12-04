#pragma once
#include "../BaseBullet.h"
#include <functional>

/// <summary>
/// プレイヤー対しまっすぐ進む弾（乱数により少し散らばる）
/// </summary>
class Boss1Bullet1 : public BaseBullet
{
public:

	struct BulletInfo {
		bool isAlive;//出現しているか
		bool isShot;//発射されているか
		Vector3 pos;//座標
		float angle;
		Vector3 moveVec;//移動方向
		std::unique_ptr<Engine::Timer> timer;
		int nowIntTime;
		std::array<Vector3, 3> predictionLinePoint;
	};

private:

	enum class State {
		start,
		attack,
		end,
		non,
	};

public:
	Boss1Bullet1();
	~Boss1Bullet1() {};

	void Update() override;

	void GetAttackCollisionSphere(std::vector<Sphere>& _info) override;

	void GetAttackCollisionBox(std::vector<Box>& _info) override {};

	void GetAttackCollisionCapsule(std::vector<Capsule>& _info) override {};

	void DeleteBullet(std::vector<int> _deleteNum) override;

	int GetDamage()override { return 5; }

private:

	void Start();

	void Attack();

	void End();

	void BulletRotate(BulletInfo& _bullet);

	void BulletUpdate(BulletInfo& _bullet);

private:

	//状態
	State state;
	std::vector<std::function<void()>> func_;

	static const int maxBulletNum = 20;
	int addBulletNum;
	std::array<BulletInfo, maxBulletNum> bullet;

	DirectX::XMFLOAT4 bulletColor = { 1.0f,1.0f,1.0f,1.0f };
	DirectX::XMFLOAT4 effectColor = { 1.0f,0.3f,0.3f,0.3f };
};

