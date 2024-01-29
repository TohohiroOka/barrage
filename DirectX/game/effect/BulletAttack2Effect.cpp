#include "BulletAttack2Effect.h"
#include "GameHelper.h"

BulletAttack2Effect::BulletAttack2Effect(const Vector3& _postion, const Vector2& _range)
{
	pos = _postion;
	pos.x += 6.0f;
	range = _range;
	TextureManager::LoadTexture("SquareEffect", "Resources/particle/gridSmokeEffect2.png");
	particle = ParticleManager::Create("SquareEffect");
	particle->SetBloom(true);
}

void BulletAttack2Effect::AddParticle()
{
	for (int num = 0; num < 2; num++) {
		if (!particle->GetCheckDraw()) { continue; }
		Vector2 dist = { RandomFloat(range.x) - range.x,RandomFloat(range.y) - range.y / 2.0f };
		float scale_s = (RandomFloat(100) + 200.0f) / 100.0f;
		float scale_e = (RandomFloat(100) + 50.0f) / 100.0f;

		Vector3 color = {
			(RandomFloat(1500) / 10.0f) + 90.0f,
			(RandomFloat(500.0f) / 10.0f) + 10.0f,
			(RandomFloat(500.0f) / 10.0f) + 200.0f
		};

		color = color / 255.0f;

		particle->Add(20, pos + Vector3(dist.x, dist.y, 0.0f),
			{}, {}, scale_s, scale_e,
			{ color.x,color.y,color.z ,1.0f },
			{ color.x,color.y,color.z ,1.0f });
	}
}

void BulletAttack2Effect::Update()
{
	particle->Update();
}

void BulletAttack2Effect::Draw()
{
	particle->Draw();
}

void BulletAttack2Effect::Finalize()
{
	particle.reset();
}