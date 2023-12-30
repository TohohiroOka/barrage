#pragma once
#include "Object/3d/Fbx.h"
#include "effect/SlashEffect.h"

/// <summary>
/// �v���C���[�p��
/// </summary>
class PlayerSword
{
public: //�����o�֐�
	PlayerSword(Fbx* parent);
	~PlayerSword();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �G�t�F�N�g�`��
	/// </summary>
	void DrawEffect();

	//getter
	SlashEffect* GetSlashEffect() { return slashEffect.get(); }

private: //�����o�ϐ�
	//������������e�I�u�W�F�N�g
	Fbx* parent = nullptr;
	//���̃��f��
	std::unique_ptr<Model> swordModel;
	//�a�����o
	std::unique_ptr<SlashEffect> slashEffect;
};