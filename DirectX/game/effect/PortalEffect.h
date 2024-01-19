#pragma once
#include "Particle/ParticleManager.h"
#include <array>
#include "Math/Vector3.h"
#include "Math/Vector2.h"

class PortalEffect
{
public:
	PortalEffect(const Vector3& _postion, const Vector2& _range);
	~PortalEffect() {};

	void AddParticle();

	void Update();

	void Draw();

	void Finalize();

private:
	//弾描画用インスタンシングオブジェクト
	std::array<std::unique_ptr<ParticleManager>, 2> particle;

	Vector3 pos;
	Vector2 range;
};