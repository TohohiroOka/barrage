#pragma once
#include "game/player/BasePlayerAttack.h"
#include "engine/Object/3d/Base3D.h"
#include "Object/3d/Object3d.h"
#include "Math/Timer.h"
#include <functional>

/// <summary>
/// �v���C���[���U��1
/// </summary>
class PlayerSwordAttack1 : public BasePlayerAttack
{
public: //�X�e�[�g
	enum State
	{
		ATTACK1,
		ATTACK2,
		ATTACK3,
		NONE,
	};

public: //�����o�֐�
	PlayerSwordAttack1(std::function<DirectX::XMFLOAT3()> getSwordPos);
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
	/// �������_�ł̕`��
	/// </summary>
	void DrawLightView() override;

	/// <summary>
	/// ���̍U���ɑJ��
	/// </summary>
	bool NextAttack(int endurance) override;

	/// <summary>
	/// �U�������������ꍇ�̏���
	/// </summary>
	void AttackCollision() override;

private: //�����o�֐�
	void AttackAction1();
	void AttackAction2();
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
	std::unique_ptr<Engine::Timer> timer = 0;
	//�e�s���̓���
	std::vector<std::function<void()>> func_;

	//���̍��W�擾�p�֐��|�C���^
	std::function<DirectX::XMFLOAT3()> getSwordPos_;

	//�A���U����
	int attackNum = 0;

	//�U����������p�I�u�W�F�N�g
	std::unique_ptr<Model> model;
	std::unique_ptr<Object3d> object;
};