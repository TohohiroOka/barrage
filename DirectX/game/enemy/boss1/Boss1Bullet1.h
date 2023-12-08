#pragma once
#include "../BaseBullet.h"
#include <functional>

/// <summary>
/// �v���C���[�΂��܂������i�ޒe�i�����ɂ�菭���U��΂�j
/// </summary>
class Boss1Bullet1 : public BaseBullet
{
public:

	struct BulletAddPointInfo {
		bool isAlive;//�o�����Ă��邩
		float alpha;
		Vector3 pos;//���W
		float angle;
	};

	struct BulletInfo {
		bool isAlive;//�o�����Ă��邩
		float alpha;
		Vector3 pos;//���W
		Vector3 rota;
		Vector3 moveVec;
		std::unique_ptr<Engine::Timer> timer;//�o������
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
	~Boss1Bullet1();

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

	void AddBullet(const Vector3& _pos);

	void BulletRotate(BulletAddPointInfo& _bullet);

	void BulletUpdate(BulletInfo& _bullet);

private:

	std::unique_ptr<Model> bulletModel;

	//���
	State state;
	std::vector<std::function<void()>> func_;

	//�e�̏o���|�C���g
	static const int maxBulletNum = 12;
	int addBulletNum;
	std::array<BulletAddPointInfo, maxBulletNum> bulletAddPoint;
	float angleSpeed;

	//�e�z��
	std::forward_list<BulletInfo> bullet;

	//�g�p�|�C���g�ԍ�
	std::array<int, 2> usePoint;

	DirectX::XMFLOAT4 bulletColor = { 1.0f,1.0f,1.0f,1.0f };
	DirectX::XMFLOAT4 effectColor = { 1.0f,0.3f,0.3f,0.3f };
};

