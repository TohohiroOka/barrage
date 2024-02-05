#pragma once
#include "Particle/ParticleManager.h"
#include <array>
#include "Math/Vector3.h"

class Boss1HalfAttackEffect
{
public:
	Boss1HalfAttackEffect(const Vector3& _pos);
	~Boss1HalfAttackEffect() {};

	void AddParticle();

	void Update();

	void Draw();

private:

	Vector3 pos;

	//�e�`��p�C���X�^���V���O�I�u�W�F�N�g
	std::array<std::unique_ptr<ParticleManager>, 3> particle;

};

