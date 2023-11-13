#include "BulletEffect.h"

void BulletEffect::LoadResources()
{
	ParticleManager::LoadTexture("effect", "Resources/particle/effect1.png");
}

void BulletEffect::Init()
{
	emitter = Emitter::Create("effect");
}

void BulletEffect::Update()
{
	emitter->Update();
}

void BulletEffect::Draw()
{
	if(emitter->GetCreateNum() != 0){ emitter->Draw(); }
}

void BulletEffect::AddBulletEffect(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT4& color, float& effectScale, DirectX::XMFLOAT4& effectColor)
{
	emitter->InEmitter(1, pos, { 0.f,0.f,0.f }, { 0.f,0.f,0.f }, effectScale, effectScale, effectColor, effectColor);
}
