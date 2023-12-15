#pragma once
#include "Particle/Emitter.h"
#include <array>
#include "Singleton.h"

//�ʏ�e�̃G�t�F�N�g
class AllHitEffect : public Singleton<AllHitEffect>
{
	friend class Singleton<AllHitEffect>;

public:
	AllHitEffect();
	~AllHitEffect() {};

	void AddParticle(const DirectX::XMFLOAT3& _pos);

	void Update();

	void Draw();

private:
	//�e�`��p�C���X�^���V���O�I�u�W�F�N�g
	std::array<std::unique_ptr<Emitter>, 3> emitter;
};