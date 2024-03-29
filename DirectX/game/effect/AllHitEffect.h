#pragma once
#include "Particle/ParticleManager.h"
#include <array>
#include "Singleton.h"

//通常弾のエフェクト
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
	//弾描画用インスタンシングオブジェクト
	std::array<std::unique_ptr<ParticleManager>, 3> particle;
};