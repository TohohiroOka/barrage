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
		NONATTACK1,
		ATTACK1,
		NONATTACK2,
		ATTACK2,
		NONATTACK3,
		ATTACK3,
		NONE,
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

	/// <summary>
	/// ���̍U���ɑJ��
	/// </summary>
	bool NextAttack(int endurance) override;

private: //�����o�֐�
	void NonAttackAction1();
	void AttackAction1();
	void NonAttackAction2();
	void AttackAction2();
	void NonAttackAction3();
	void AttackAction3();

private: //�ÓI�����o�ϐ�
	//���̍U���̎��v�͎g�p��
	static const int attackUseEnduranceNum = 20;
	//�ő�A���U����
	static const int maxAttackNum = 3;
	//�F
	static const DirectX::XMFLOAT4 attackColor;
	static const DirectX::XMFLOAT4 nonAttackColor;

private: //�����o�ϐ�
	//�s��
	State state = State::NONE;
	//�s���p�^�C�}�[
	float timer = 0;
	//�e�s���̓���
	std::vector<std::function<void()>> func_;

	//�A���U����
	int attackNum = 0;

	//�U����������p�I�u�W�F�N�g
	std::unique_ptr<Model> model;
	std::unique_ptr<Object3d> object;

	//���U���p�C�[�W���O�ϐ�
	Vector3 easeBeforePos;
	Vector3 easeAfterPos;
};