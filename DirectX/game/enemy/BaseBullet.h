#pragma once
#include "BaseAction.h"
#include "Object/3d/InstanceObject.h"
#include "../game/enemy/PredictionLine.h"
#include "Math/Vector3.h"
#include "../Math/Timer.h"
#include "../game/effect/BulletEffect.h"

#include <forward_list>

class BaseBullet : public BaseAction
{
public:

	virtual ~BaseBullet() {};
	
	virtual void Update() override;

	void Draw() override;

	void FrameReset() override;

	virtual void GetAttackCollisionSphere(std::vector<Sphere>& _info) override {};

	virtual void GetAttackCollisionBox(std::vector<Box>& _info) override {};

	virtual void GetAttackCollisionCapsule(std::vector<Capsule>& _info) override {};

protected:
	static const int instanceNum = 5;
	//弾描画用
	std::array<std::unique_ptr<InstanceObject>, instanceNum> instanceObject;
	BulletEffect bulletEffect;
	//弾エフェクトの共通パラメータ
	DirectX::XMFLOAT3 B_SCALE = { 1.f,1.f,1.f };

	//弾道描画用
	std::unique_ptr<PredictionLine> predictionLine;

	//タイマー
	std::unique_ptr<Engine::Timer> timer;
	//モデル
	std::unique_ptr<Model> model;
};