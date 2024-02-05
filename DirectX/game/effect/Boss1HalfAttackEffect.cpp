#include "Boss1HalfAttackEffect.h"
#include "GameHelper.h"

Boss1HalfAttackEffect::Boss1HalfAttackEffect(const Vector3& _pos)
{
	pos = _pos;

	for (int i = 0; i < 3; i++) {
		std::string num = std::to_string(i + 1);
		TextureManager::LoadTexture("triangle" + num, "Resources/particle/triangle" + num + ".png");
		particle[i] = ParticleManager::Create("triangle" + num);
		particle[i]->SetBloom(true);
	}
}

void Boss1HalfAttackEffect::AddParticle()
{
	const float maxFrame = 30.0f;
	for (int num = 0; num < 8; num++) {
		for (auto& i : particle) {
			if (!i->GetCheckDraw()) { continue; }
			Vector3 dist = { RandomFloat(1000) - 500.0f,RandomFloat(200.0f) - 100.0f, RandomFloat(1000) - 500.0f };
			float scale_s = (RandomFloat(100) + 200.0f) / 100.0f;
			float scale_e = (RandomFloat(100) + 50.0f) / 100.0f;

			Vector3 velocity = -dist / maxFrame;

			Vector3 color = {
				(RandomFloat(20.0f) / 10.0f) + 0.1f,
				(RandomFloat(20.0f) / 10.0f) + 0.1f,
				(RandomFloat(20.0f) / 10.0f) + 0.1f
			};

			i->Add(maxFrame, pos + dist,
				velocity, {}, scale_s, scale_e,
				{ color.x,color.y,color.z ,1.0f },
				{ color.x,color.y,color.z ,1.0f });
		}
	}
}

void Boss1HalfAttackEffect::Update()
{
	for (auto& i : particle) {
		i->Update();
	}
}

void Boss1HalfAttackEffect::Draw()
{
	for (auto& i : particle) {
		i->Draw();
	}
}
