#pragma once
#include "Object/3d/InstanceObject.h"
#include "PredictionLine.h"
#include "Math/Vector3.h"

/// <summary>
/// íeàÍå¬ÇÃèÓïÒ
/// </summary>
class BaseBullet
{
public:

	BaseBullet(const Vector3& _pos, const Vector3& _scale, const Vector3& _rota, const Vector3& _moveVec);
	~BaseBullet() {};

	void Update();

	void Draw();

	static void StaticDraw();

	static void FrameReset();

	static void Reset();

	static void CreateInstance(Model* _model);

	bool GetIsAlive() { return isAlive; }

private:
	
	static std::unique_ptr<InstanceObject> instanceObject;

	static std::unique_ptr<PredictionLine> predictionLine;

	bool isAlive;
	Vector3 pos;
	Vector3 scale;
	Vector3 rota;
	Vector3 moveVec;

	//èoåªéûä‘
	float timer;
};