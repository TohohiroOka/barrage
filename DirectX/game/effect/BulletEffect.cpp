#include "BulletEffect.h"

void BulletEffect::LoadResources()
{
	TextureManager::LoadTexture("effect", "Resources/particle/effect1.png");
}

void BulletEffect::Init()
{
	particle = ParticleManager::Create("effect");
}

void BulletEffect::Update()
{
	particle->Update();
}

void BulletEffect::Draw()
{
	if(particle->GetCreateNum() != 0){ particle->Draw(); }
}

void BulletEffect::AddBulletEffect(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT4& color, float& effectScale, DirectX::XMFLOAT4& effectColor)
{
	particle->Add(1, pos, { 0.f,0.f,0.f }, { 0.f,0.f,0.f }, effectScale, effectScale, effectColor, effectColor);
}
