#pragma once
#include "Particle/ParticleManager.h"
#include <array>
#include "Singleton.h"

//�ʏ�e�̃G�t�F�N�g
class AllHitEffect : public Singleton<AllHitEffect>
{
	friend class Singleton<AllHitEffect>;

public:
	AllHitEffect();
	~AllHitEffect() {};

	void Initialize();

	void AddParticle(const DirectX::XMFLOAT3& _pos);

	void Update();

	void Draw();

	void Finalize();

private:
	//�e�`��p�C���X�^���V���O�I�u�W�F�N�g
	std::array<std::unique_ptr<ParticleManager>, 3> particle;
};