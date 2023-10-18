#pragma once
#include "../BaseAction.h"
#include "../BaseBullet.h"
#include <forward_list>

class Boss1Bullet1 : public BaseAction
{
public:
	Boss1Bullet1();
	~Boss1Bullet1();

	void Update() override;

	void Draw() override;

	void FrameReset() override;

	void GetAttackCollision(std::vector<BaseAction::AttackCollision>& _info) override;

	void AddBullet();

private:
	//タイマー
	float timer;
	//モデル
	std::unique_ptr<Model> model;
	//弾
	std::forward_list<BaseBullet> bullet;

};

