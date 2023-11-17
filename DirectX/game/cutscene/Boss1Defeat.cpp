#include "Boss1Defeat.h"
#include "GameHelper.h"

void Boss1Defeat::Init()
{
	ParticleManager::LoadTexture("noisetex", "Resources/SubTexture/white1x1.png");
	noiseParticle = Emitter::Create("noisetex");
	BaseDefeatDirection::Init();
}

void Boss1Defeat::Update()
{
	//�p�[�e�B�N���X�V
	if (noiseParticle->GetCreateNum() != 0) { noiseParticle->Update(); }

	//�ŗL���o��͋��ʉ��o���s��
	if (isEnded) {
		StartCommonDirection();
		BaseDefeatDirection::Update();
		return;
	}

	//���o���ԍX�V
	if (isDirecting && !isEnded) { frame++; }
	if (frame > DEFEAT_DIRECTION_MAXFRAME) { isEnded = true; }

	//�m�C�Y�p�[�e�B�N������
	for (int i = 0; i < 3; i++) {
		noiseParticleStatus.pos.x = bossPosition.x + (RandomFloat(range.x) * RandomSign());
		noiseParticleStatus.pos.y = bossPosition.y + RandomFloat(range.y);
		noiseParticleStatus.pos.z = bossPosition.z + (RandomFloat(range.z) * RandomSign());
		InEmitter(noiseParticle.get(), noiseParticleStatus);
	}
}

void Boss1Defeat::Draw()
{
	if (!isDirecting) { return; }

	if (noiseParticle->GetCreateNum() != 0) { noiseParticle->Draw(); }
}

void Boss1Defeat::Draw2D()
{
	//���ʉ��o�̕`��
	if (isEnded) { BaseDefeatDirection::Draw2D(); }
}