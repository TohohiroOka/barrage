#pragma once
#include "Particle/ParticleManager.h"
#include <array>
#include "Math/Vector3.h"
#include "Math/Vector2.h"

class BulletAttack2Effect
{
public:
	BulletAttack2Effect(const Vector3& _postion, const Vector2& _range);
	~BulletAttack2Effect() {};

	void AddParticle();

	void Update();

	void Draw();

	void Finalize();

	int EffectNum() { return particle->GetCreateNum(); }

private:
	//弾描画用インスタンシングオブジェクト
	std::unique_ptr<ParticleManager> particle;

	Vector3 pos;
	Vector2 range;
};

