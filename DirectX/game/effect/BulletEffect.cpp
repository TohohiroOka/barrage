#include "BulletEffect.h"

void BulletEffect::LoadResources()
{
	ParticleManager::LoadTexture("effect", "Resources/particle/effect1.png");
}

void BulletEffect::Init()
{
	bulletModel = Model::CreateFromOBJ("bullet");
	effectModel = Model::CreateFromOBJ("bullet");
	for (auto& i : instanceObjects) {
		i = InstanceObject::Create(bulletModel.get());
	}

	emitter = Emitter::Create("effect");
}

void BulletEffect::Update()
{
	emitter->Update();
}

void BulletEffect::Draw()
{
	for (int i = 0; i < INSTANCE_NUM; i++) {
		if (instanceObjects[i]->GetInstanceDrawNum() == 0) { continue; }
		instanceObjects[i]->Draw();
		instanceObjects[i]->FrameReset();
	}

	if(emitter->GetCreateNum() != 0){ emitter->Draw(); }

}

void BulletEffect::AddBulletEffect(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& scale, DirectX::XMFLOAT4& color, float& effectScale, DirectX::XMFLOAT4& effectColor)
{
	for (int i = 0; i < INSTANCE_NUM; i++) {
		if (!instanceObjects[i]->GetInstanceDrawCheck()) { continue; }
		instanceObjects[i]->DrawInstance(pos, scale, { 0.f,0.f,0.f }, color);
	}

	emitter->InEmitter(1, pos, { 0.f,0.f,0.f }, { 0.f,0.f,0.f }, effectScale, effectScale, effectColor, effectColor);
}
