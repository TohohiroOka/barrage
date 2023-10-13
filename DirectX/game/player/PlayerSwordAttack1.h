#pragma once
#include "game/player/BasePlayerAttack.h"
#include "engine/Object/3d/Base3D.h"
#include "Object/3d/Object3d.h"
#include <functional>

/// <summary>
/// �v���C���[���U��1
/// </summary>
class PlayerSwordAttack1 : public BasePlayerAttack
{
public: //�X�e�[�g
	enum State
	{
		NONATTACK,
		ATTACK,
	};

public: //�����o�֐�
	PlayerSwordAttack1(Base3D* attacker);
	~PlayerSwordAttack1();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �`��
	/// </summary>
	void Draw() override;

private: //�����o�֐�
	void NonAttackAction();
	void AttackAction();

private: //�����o�ϐ�
	//�s��
	State state = State::NONATTACK;
	//�s���p�^�C�}�[
	float timer = 0;
	//�e�s���̓���
	std::vector<std::function<void()>> func_;

	//�U����������p�I�u�W�F�N�g
	std::unique_ptr<Model> model;
	std::unique_ptr<Object3d> object;
};