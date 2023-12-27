#pragma once

#include "BaseDefeatDirection.h"
#include "Math/Vector3.h"


#include <memory>

class Boss1Defeat : public BaseDefeatDirection
{
public:
	Boss1Defeat() = default;
	~Boss1Defeat() = default;

	void Init() override
	{
		noiseParticle = ParticleManager::Create("white");
		TextureManager::LoadTexture("gridSmoke", "Resources/particle/gridSmokeEffect.png");
		smokeParticle = ParticleManager::Create("gridSmoke");
		BaseDefeatDirection::Init();
	}

	void Update() override;

	void Draw() override;

	void Draw2D() override;

private:
	//撃破パーティクル
	std::unique_ptr<ParticleManager> noiseParticle;
	ParticleGrainState noiseParticleStatus{ 30,{},{0,1.0f,0},{0,1.0f / 30.f,0},1.0f,1.0f,{1.0f,1.0f,1.0f,1.0f},{1.0f,1.0f,1.0f,0.0f}, };
	std::unique_ptr<ParticleManager> smokeParticle;
	ParticleGrainState smokeParticleStatus{ 30,{},{0,1.0f,0},{0,1.0f / 30.f,0},10.0f,0.0f,{1.0f,1.0f,1.0f,0.0f},{1.0f,1.0f,1.0f,1.0f}, };


	const Vector3 range = Vector3(50.f, 30.f, 50.f);


	//再生時間
	const int DEFEAT_DIRECTION_MAXFRAME = 150;

};