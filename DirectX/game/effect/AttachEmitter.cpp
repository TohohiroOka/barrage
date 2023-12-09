#include "AttachEmitter.h"
#include "GameHelper.h"

AttachEmitter::AttachEmitter(const std::string& texName)
{
	//ParticleManager::LoadTexture(texName,texName);
	particle = Emitter::Create(texName);
	particle->SetBloom();
}

void AttachEmitter::Add(const Vector3& _point1, const Vector3& _point2)
{
	Vector3 vec = _point2 - _point1;
	const float len = vec.length();
	vec.normalize();

	float inLen = RandomFloat(len * 1000.0f) / 1000.0f;

	particle->InEmitter(30, _point1 + (vec * inLen), {}, {}, 1.0f, 0.0f, { 1,1,1,1 }, { 1,1,1,1 });
}

void AttachEmitter::Update()
{
	particle->Update();
}

void AttachEmitter::Draw()
{
	particle->Draw();
}
