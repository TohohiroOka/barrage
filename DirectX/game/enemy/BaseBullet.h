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

	virtual void Draw() override;

	virtual void FrameReset() override;

protected:

	static const DirectX::XMFLOAT3 moveMaxPos;

	static const int instanceNum = 5;
	//’e•`‰æ—p
	std::array<std::unique_ptr<InstanceObject>, instanceNum> instanceObject;
	std::unique_ptr<BulletEffect> bulletEffect;

	//’e“¹•`‰æ—p
	std::unique_ptr<PredictionLine> predictionLine;
	//ƒ‚ƒfƒ‹
	std::unique_ptr<Model> model;
};