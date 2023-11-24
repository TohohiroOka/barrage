#pragma once
#include "../BaseBullet.h"

/// <summary>
/// プレイヤー対しまっすぐ進む弾（乱数により少し散らばる）
/// </summary>
class Boss1Bullet1 : public BaseBullet
{
public:

	struct BulletInfo {
		bool isAlive;//出現しているか
		Vector3 pos;//座標
		Vector3 moveVec;//移動方向
		std::unique_ptr<Engine::Timer> timer;//出現時間
		Vector3 predictionLinePoint;
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

	void AddBullet();

	void BulletUpdate(BulletInfo& _bullet);

private:

	std::forward_list<BulletInfo> bullet;

	DirectX::XMFLOAT4 bulletColor = { 1.0f,1.0f,1.0f,1.0f };
	DirectX::XMFLOAT4 effectColor = { 1.0f,0.3f,0.3f,0.3f };
};

