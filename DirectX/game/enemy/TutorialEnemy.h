#pragma once
#include "Object/3d/Object3d.h"
#include "TutorialEnemyBullet.h"
#include "Object/3d/collider/SphereCollider.h"
#include <forward_list>
#include "Math/Timer.h"
#include <vector>

class PlayerData;

/// <summary>
/// �`���[�g���A���p�G
/// </summary>
class TutorialEnemy
{
public: //�����o�֐�
	TutorialEnemy(const DirectX::XMFLOAT3& position, PlayerData* _playerData);
	~TutorialEnemy();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �_���[�W��H�炤
	/// </summary>
	void Damage();

	/// <summary>
	/// 
	/// </summary>
	void AddBullet();

	void DeleteBullet(std::vector<int> _deleteNum);

	void FrameReset();

	void GetAttackCollision(std::vector<Sphere>& _info);

	void SetPlayerDirection();

	//getter
	Object3d* GetObject3d() { return object.get(); }
	bool GetIsDamage() { return isDamage; }
	bool GetIsBulletShot() { return isBulletShot; }

	//setter
	void SetIsBulletShot(bool isBulletShot) { 
		this->isBulletShot = isBulletShot;
	}

private: //�����o�ϐ�
	//�I�u�W�F�N�g3d
	std::unique_ptr<Object3d> object;
	std::unique_ptr<Model> model;

	//�_���[�W�t���O
	bool isDamage = false;

	bool isBulletShot = false;
	std::forward_list<TutorialEnemyBullet> bullet;

	std::unique_ptr<Engine::Timer> bulletShotTimer;

	PlayerData* playerData;
};