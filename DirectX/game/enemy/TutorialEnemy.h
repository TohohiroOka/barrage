#pragma once
#include "Object/3d/Object3d.h"
#include "TutorialEnemyBullet.h"
#include "Object/3d/collider/SphereCollider.h"
#include <forward_list>
#include "Math/Timer.h"
#include <vector>

class PlayerData;

/// <summary>
/// チュートリアル用敵
/// </summary>
class TutorialEnemy
{
public: //メンバ関数
	TutorialEnemy(const DirectX::XMFLOAT3& position, PlayerData* _playerData);
	~TutorialEnemy();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ダメージを食らう
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

private: //メンバ変数
	//オブジェクト3d
	std::unique_ptr<Object3d> object;
	std::unique_ptr<Model> model;

	//ダメージフラグ
	bool isDamage = false;

	bool isBulletShot = false;
	std::forward_list<TutorialEnemyBullet> bullet;

	std::unique_ptr<Engine::Timer> bulletShotTimer;

	PlayerData* playerData;
};