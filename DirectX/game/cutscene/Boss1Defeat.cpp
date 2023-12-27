#include "Boss1Defeat.h"
#include "GameHelper.h"

void Boss1Defeat::Update()
{
	//パーティクル更新
	if (noiseParticle->GetCreateNum() != 0) { noiseParticle->Update(); }
	if (smokeParticle->GetCreateNum() != 0) { smokeParticle->Update(); }

	//固有演出後は共通演出を行う
	if (isEnded) {
		StartCommonDirection();
		BaseDefeatDirection::Update();
		return;
	}

	//演出時間更新
	if (isDirecting && !isEnded) { frame++; }
	if (frame > DEFEAT_DIRECTION_MAXFRAME) { isEnded = true; }

	//ノイズパーティクル生成
	for (int i = 0; i < 3; i++) {
		noiseParticleStatus.pos.x = bossPosition.x + (RandomFloat(range.x) * RandomSign());
		noiseParticleStatus.pos.y = bossPosition.y + RandomFloat(range.y);
		noiseParticleStatus.pos.z = bossPosition.z + (RandomFloat(range.z) * RandomSign());
		InEmitter(noiseParticle.get(), noiseParticleStatus);
		smokeParticleStatus.pos.x = bossPosition.x + (RandomFloat(range.x) * RandomSign());
		smokeParticleStatus.pos.y = bossPosition.y + RandomFloat(range.y);
		smokeParticleStatus.pos.z = bossPosition.z + (RandomFloat(range.z) * RandomSign());
		InEmitter(smokeParticle.get(), smokeParticleStatus);
	}
	
}

void Boss1Defeat::Draw()
{
	if (!isDirecting) { return; }

	if (noiseParticle->GetCreateNum() != 0) { noiseParticle->Draw(); }
	if (smokeParticle->GetCreateNum() != 0) { smokeParticle->Draw(); }
}

void Boss1Defeat::Draw2D()
{
	//共通演出の描画
	if (isEnded) { BaseDefeatDirection::Draw2D(); }
}