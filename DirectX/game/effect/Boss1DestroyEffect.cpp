#include "Boss1DestroyEffect.h"
#include "GameHelper.h"

Boss1DestroyEffect::Boss1DestroyEffect(const Vector3& _postion)
{
	pos = _postion;
	TextureManager::LoadTexture("SquareEffect", "Resources/particle/gridSmokeEffect2.png");
	for (int i = 0; i < 2; i++) {
		TextureManager::LoadTexture("SquareEffect", "Resources/particle/gridSmokeEffect2.png");
		particle[i] = ParticleManager::Create("SquareEffect");
		particle[i]->SetBloom(true);
	}
}

void Boss1DestroyEffect::AddParticle()
{
	for (int num = 0; num < 3; num++) {
		for (auto& i : particle) {
			if (!i->GetCheckDraw()) { continue; }
			float scale_s = (RandomFloat(100) + 200.0f) / 100.0f;
			float scale_e = (RandomFloat(100) + 50.0f) / 100.0f;

			Vector3 color = {
				(RandomFloat(20.0f) / 10.0f) + 0.1f,
				(RandomFloat(20.0f) / 10.0f) + 0.1f,
				(RandomFloat(20.0f) / 10.0f) + 0.1f
			};

			Vector3 velo = { (RandomFloat(1000.0f) - 500.0f) / 10000.0f,
						RandomFloat(100.0f) / 200.0f ,
						(RandomFloat(1000.0f) - 500.0f) / 10000.0f };
			Vector3 acc = { (RandomFloat(1000.0f)-500.0f) / 20000.0f ,
						0.0f ,
						 (RandomFloat(1000.0f) - 500.0f) / 20000.0f };

			i->Add(30, pos,
				velo, acc, scale_s, scale_e,
				{ color.x,color.y,color.z ,1.0f },
				{ color.x,color.y,color.z ,1.0f });
		}
	}
}

void Boss1DestroyEffect::Update()
{
	for (auto& i : particle) {
		i->Update();
	}
}

void Boss1DestroyEffect::Draw()
{
	for (auto& i : particle) {
		i->Draw();
	}
}

void Boss1DestroyEffect::Finalize()
{
	for (auto& i : particle) {
		i.reset();
	}
}