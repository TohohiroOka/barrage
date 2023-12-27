#pragma once
#include "Particle/ParticleManager.h"
#include "engine/Math/Timer.h"

#include <memory>
#include <array>

//�ʏ�e�̃G�t�F�N�g
class BulletEffect
{
public:
	BulletEffect() = default;
	~BulletEffect() {};

	static void LoadResources();

	void Init();

	void Update();

	void Draw();

	//�e�G�t�F�N�g�ǉ��i���݂���e�̐������e���˃N���X�ŌĂяo���j
	void AddBulletEffect(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT4& color,
		float &effectScale, DirectX::XMFLOAT4 &effectColor);

private:
	//�e�`��p�C���X�^���V���O�I�u�W�F�N�g
	std::unique_ptr<ParticleManager> particle;
};