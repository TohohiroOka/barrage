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
	//íeï`âÊóp
	std::array<std::unique_ptr<InstanceObject>, instanceNum> instanceObject;
	BulletEffect bulletEffect;

	//íeìπï`âÊóp
	std::unique_ptr<PredictionLine> predictionLine;

	//É^ÉCÉ}Å[
	std::unique_ptr<Engine::Timer> timer;
	//ÉÇÉfÉã
	std::unique_ptr<Model> model;
};