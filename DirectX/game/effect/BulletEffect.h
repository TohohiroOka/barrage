#pragma once
#include "Object/3d/InstanceObject.h"
#include "Particle/Emitter.h"
#include "engine/Math/Timer.h"

#include <memory>
#include <array>

//�ʏ�e�̃G�t�F�N�g
class BulletEffect
{
public:
	BulletEffect() = default;
	~BulletEffect() = default;

	static void LoadResources();

	void Init();

	void Update();

	void Draw();

	//�e�G�t�F�N�g�ǉ��i���݂���e�̐������e���˃N���X�ŌĂяo���j
	void AddBulletEffect(DirectX::XMFLOAT3& pos, DirectX::XMFLOAT3& scale, DirectX::XMFLOAT4& color,
		float &effectScale, DirectX::XMFLOAT4 &effectColor);

private:
	//�e���f��
	std::unique_ptr<Model> bulletModel;
	//�G�t�F�N�g���f��
	std::unique_ptr<Model> effectModel;

	//�e�`��p�C���X�^���V���O�I�u�W�F�N�g
	static const int INSTANCE_NUM = 5;
	std::array<std::unique_ptr<InstanceObject>, INSTANCE_NUM> instanceObjects;
	std::array<std::unique_ptr<InstanceObject>, INSTANCE_NUM> effectObjects;
	std::unique_ptr<Emitter> emitter;


};