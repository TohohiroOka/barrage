#include "PortalEffect.h"
#include "GameHelper.h"

PortalEffect::PortalEffect(const Vector3& _postion, const Vector2& _range)
{
	pos = _postion;
	pos.x += 6.0f;
	range = _range;
	for (int i = 0; i < 2; i++) {
		TextureManager::LoadTexture("SquareEffect", "Resources/particle/gridSmokeEffect2.png");
		particle[i] = ParticleManager::Create("SquareEffect");
		particle[i]->SetBloom(true);
	}
}

void PortalEffect::AddParticle()
{
	for (int num = 0; num < 8; num++) {
		for (auto& i : particle) {
			if (!i->GetCheckDraw()) { continue; }
			Vector2 dist = { RandomFloat(range.x) - range.x,RandomFloat(range.y) - range.y / 2.0f };
			float scale_s = (RandomFloat(100) + 200.0f) / 100.0f;
			float scale_e = (RandomFloat(100) + 50.0f) / 100.0f;

			Vector3 color = {
				(RandomFloat(20.0f) / 10.0f) + 0.1f,
				(RandomFloat(20.0f) / 10.0f) + 0.1f,
				(RandomFloat(20.0f) / 10.0f) + 0.1f
			};

			i->Add(20, pos + Vector3(dist.x, dist.y, 0.0f),
				{}, {}, scale_s, scale_e,
				{ color.x,color.y,color.z ,1.0f },
				{ color.x,color.y,color.z ,1.0f });
		}
	}
}

void PortalEffect::Update()
{
	for (auto& i : particle) {
		i->Update();
	}
}

void PortalEffect::Draw()
{
	for (auto& i : particle) {
		i->Draw();
	}
}

void PortalEffect::Finalize()
{
	for (auto& i : particle) {
		i.reset();
	}
}