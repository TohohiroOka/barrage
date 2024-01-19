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
	//�e�`��p�C���X�^���V���O�I�u�W�F�N�g
	std::array<std::unique_ptr<ParticleManager>, 2> particle;

	Vector3 pos;
	Vector2 range;
};