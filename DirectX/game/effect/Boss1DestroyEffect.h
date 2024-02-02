#pragma once
#include "Particle/ParticleManager.h"
#include <array>
#include "Math/Vector3.h"

class Boss1DestroyEffect
{
public:
	Boss1DestroyEffect(const Vector3& _postion);
	~Boss1DestroyEffect() {};

	void AddParticle();

	void Update();

	void Draw();

	void Finalize();

private:
	//�e�`��p�C���X�^���V���O�I�u�W�F�N�g
	std::array<std::unique_ptr<ParticleManager>,2> particle;

	Vector3 pos;
};