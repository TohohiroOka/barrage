#include "AttachEmitter.h"
#include "GameHelper.h"

AttachEmitter::AttachEmitter(const std::string& texName)
{
	//ParticleManager::LoadTexture(texName,texName);
	particle = Emitter::Create(texName);
	//particle->SetBloom();
}

void AttachEmitter::Add(const Vector3& _point1, const Vector3& _point2)
{
	Vector3 vec = _point2 - _point1;
	const float len = vec.length();
	vec.normalize();

	for (int i = 0; i < 20; i++) {
		float inLen = float((RandomInt(1000) % 19) + 1) / 20.0f;

		particle->InEmitter(30, _point1 + (vec * len * inLen), {}, {}, 0.5f, 0.0f, { 0.7f,0.7f,0.7f,0.8f }, { 0.1f,0.1f,0.1f,0.2f });
	}
}

void AttachEmitter::Update()
{
	particle->Update();
}

void AttachEmitter::Draw()
{
	particle->Draw();
}
