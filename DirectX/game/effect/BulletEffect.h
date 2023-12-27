#pragma once
#include "Particle/ParticleManager.h"
#include "engine/Math/Timer.h"

#include <memory>
#include <array>

//通常弾のエフェクト
class BulletEffect
{
public:
	BulletEffect() = default;
	~BulletEffect() {};

	static void LoadResources();

	void Init();

	void Update();

	void Draw();

	//弾エフェクト追加（存在する弾の数だけ弾発射クラスで呼び出す）
	void AddBulletEffect(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT4& color,
		float &effectScale, DirectX::XMFLOAT4 &effectColor);

private:
	//弾描画用インスタンシングオブジェクト
	std::unique_ptr<ParticleManager> particle;
};