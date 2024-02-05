#pragma once
#include "game/player/BasePlayerAttack.h"
#include "engine/Object/3d/Base3D.h"
#include "Object/3d/Object3d.h"
#include "Math/Timer.h"
#include <functional>

/// <summary>
/// �v���C���[���U��2
/// </summary>
class PlayerSwordAttack2 : public BasePlayerAttack
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
	PlayerSwordAttack2(Player* player);
	~PlayerSwordAttack2();

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
	bool NextAttack() override;

	/// <summary>
	/// �U�������������ꍇ�̏���
	/// </summary>
	void AttackCollision() override;

private: //�����o�֐�
	//�U������
	void AttackAction1();
	void AttackAction2();
	void AttackAction3();

	/// <summary>
	/// �v���C���[���U���ɍ��킹�ē�����
	/// </summary>
	void MovePlayer(int moveTime);

	/// <summary>
	/// �v���C���[����]���͂ŉ�
	/// </summary>
	void PlayerChangeRotate();

public: //�ÓI�����o�ϐ� (�ύX���₷���悤��public)
	//�ő�A���U����
	static const int maxAttackNum = 3;
	//���̍U���̎��v�͎g�p��
	static int attackUseEnduranceNum;
	//���̍U���̍U����
	static int attackPower;

	//1��ڂ̍U��
	//�U���ɂ����鎞��
	static const int attackTime1 =55;
	//��s���͂��J�n���鎞��
	static const int actionChangeStartTime1 = 45;
	//�Փ˔��肪�L���ɐ؂�ւ�鎞��
	static const int collisionValidStartTime1 = 40;

	//2��ڂ̍U��
	//�U���ɂ����鎞��
	static const int attackTime2 = 55;
	//��s���͂��J�n���鎞��
	static const int actionChangeStartTime2 = 45;
	//�Փ˔��肪�L���ɐ؂�ւ�鎞��
	static const int collisionValidStartTime2 = 35;

	//3��ڂ̍U��
	//�U���ɂ����鎞��
	static const int attackTime3 = 55;
	//��s���͂��J�n���鎞��
	static const int actionChangeStartTime3 = 45;
	//�Փ˔��肪�L���ɐ؂�ւ�鎞��
	static const int collisionValidStartTime3 = 40;

	//�F
	static const DirectX::XMFLOAT4 attackColor;
	static const DirectX::XMFLOAT4 nonAttackColor;
	//�U���J�n���̈ړ��X�s�[�h�ō��l
	static const float attackStartMoveSpeedMax;
	//�U���J�n���̈ړ��X�s�[�h�Œ�l
	static const float attackStartMoveSpeedMin;

private: //�����o�ϐ�
	//�s��
	State state = State::NONE;
	//�s���p�^�C�}�[
	std::unique_ptr<Engine::Timer> timer = 0;
	//�e�s���̓���
	std::vector<std::function<void()>> func_;

	//�A���U����
	int attackNum = 0;

	//�U�����q�b�g������
	bool isHitAttack = false;

	//�U���J�n���̈ړ��X�s�[�h
	float attackStartMoveSpeed;

	//�U����������p�I�u�W�F�N�g
	std::unique_ptr<Model> model;
	std::unique_ptr<Object3d> object;
};