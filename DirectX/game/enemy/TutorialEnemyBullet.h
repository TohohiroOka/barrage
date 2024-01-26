#pragma once
#include "Object/3d/InstanceObject.h"
#include "../game/enemy/PredictionLine.h"
#include "Math/Vector3.h"
#include "../Math/Timer.h"
#include "../game/effect/BulletEffect.h"
#include "Math/Timer.h"

#include <forward_list>

class TutorialEnemyBullet
{
public:

	TutorialEnemyBullet(const Vector3& _pos, const Vector3& _moveVec);
	~TutorialEnemyBullet() {};

	static void StaticInitialize();

	void Update();

	static void Draw();

	static void FrameReset();

	bool GetIsAlive() { return isAlive; }
	void SetIsAlive(const bool _isAlive) { isAlive = _isAlive; }

	Vector3 GetPos() { return pos; }

private:

	static const DirectX::XMFLOAT3 moveMaxPos;

	//’e•`‰æ—p
	static std::unique_ptr<Model> bulletModel;
	static std::unique_ptr<InstanceObject> object;

	//’e“¹•`‰æ—p
	static std::unique_ptr<PredictionLine> predictionLine;

	bool isAlive;

	Vector3 pos;
	Vector3 moveVec;

	Vector3 oldPos;
};

