#pragma once
#include "Particle/Emitter.h"
#include <array>

//通常弾のエフェクト
class AllHitEffect
{
public:
	AllHitEffect();
	~AllHitEffect() {};

	void AddParticle(const DirectX::XMFLOAT3& _pos);

	void Update();

	void Draw();

private:
	//弾描画用インスタンシングオブジェクト
	std::array<std::unique_ptr<Emitter>, 3> emitter;
};