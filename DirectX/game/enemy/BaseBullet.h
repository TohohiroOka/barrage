#pragma once
#include "BaseAction.h"
#include "Object/3d/InstanceObject.h"
#include "../game/enemy/PredictionLine.h"
#include "Math/Vector3.h"
#include <forward_list>

class BaseBullet : public BaseAction
{
public:

	virtual ~BaseBullet() {};
	
	virtual void Update() override;

	void Draw() override;

	void FrameReset() override;

	virtual void GetAttackCollision(std::vector<BaseAction::AttackCollision>& _info) override{};

protected:
	//弾描画用
	std::unique_ptr<InstanceObject> instanceObject;
	//弾道描画用
	std::unique_ptr<PredictionLine> predictionLine;

	//タイマー
	float timer;
	//モデル
	std::unique_ptr<Model> model;
};