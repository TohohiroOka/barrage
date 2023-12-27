#include "AllHitEffect.h"
#include "GameHelper.h"

AllHitEffect::AllHitEffect()
{
	for (int i = 0; i < 3; i++) {
		std::string num = std::to_string(i + 1);
		TextureManager::LoadTexture("triangle" + num, "Resources/particle/triangle" + num + ".png");
		particle[i] = ParticleManager::Create("triangle" + num);
		particle[i]->SetBloom(true);
	}
}

void AllHitEffect::AddParticle(const DirectX::XMFLOAT3& _pos)
{
	for (auto& i : particle) {
		if (i->GetCreateNum() >= 512) { continue; }
		Vector3 vec = { RandomFloat(100) - 50,RandomFloat(50) + 50.0f ,RandomFloat(100) - 50 };
		vec = vec.normalize();
		float scale_s = (RandomFloat(100) + 200.0f) / 100.0f;
		float scale_e = (RandomFloat(100) + 50.0f) / 100.0f;
		i->Add(20, _pos, vec, -vec * 0.04f, scale_s, scale_e, { 1.0f,1.0f ,1.0f ,1.0f }, { 1.0f,1.0f ,1.0f ,1.0f });
	}
}

void AllHitEffect::Update()
{
	for (auto& i : particle) {
		i->Update();
	}
}

void AllHitEffect::Draw()
{
	for (auto& i : particle) {
		i->Draw();
	}
}

void AllHitEffect::Finalize()
{
	for (auto& i : particle) {
		i.reset();
	}
}