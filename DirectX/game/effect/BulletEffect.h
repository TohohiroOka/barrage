#pragma once
#include "Object/3d/InstanceObject.h"
#include "Particle/Emitter.h"
#include "engine/Math/Timer.h"

#include <memory>
#include <array>

//通常弾のエフェクト
class BulletEffect
{
public:
	BulletEffect() = default;
	~BulletEffect() = default;

	static void LoadResources();

	void Init();

	void Update();

	void Draw();

	//弾エフェクト追加（存在する弾の数だけ弾発射クラスで呼び出す）
	void AddBulletEffect(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& scale, DirectX::XMFLOAT4& color,
		float &effectScale, DirectX::XMFLOAT4 &effectColor);

private:
	//弾モデル
	std::unique_ptr<Model> bulletModel;
	//エフェクトモデル
	std::unique_ptr<Model> effectModel;

	//弾描画用インスタンシングオブジェクト
	static const int INSTANCE_NUM = 5;
	std::array<std::unique_ptr<InstanceObject>, INSTANCE_NUM> instanceObjects;
	std::array<std::unique_ptr<InstanceObject>, INSTANCE_NUM> effectObjects;
	std::unique_ptr<Emitter> emitter;


};