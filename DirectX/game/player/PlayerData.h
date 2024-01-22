#pragma once
#include "Math/Vector3.h"
#include "BasePlayerAttack.h"
#include <memory> 

/// <summary>
/// �v���C���[�̍s����
/// </summary>
enum PlayerActionName
{
	MOVE_NORMAL,	//�ʏ�ړ�(���͂ɐ�����������Ȃ�)
	JUMP,	//�W�����v(1�t���[��)
	LIGHT_ATTACK,	//��U��
	STRONG_ATTACK,	//���U��
	AVOID,	//���
	BLINK,	//�u�����N
	KNOCKBACK,//�m�b�N�o�b�N
	DEAD,	//���S
	TITLE_FLOAT,	//�^�C�g���V�[���p���V
	TITLE_INTO_PORTAL,	//�^�C�g���V�[���p�|�[�^���ɓ���
};

/// <summary>
/// �A�j���[�V������
/// </summary>
enum PlayerAnimationName
{
	ATTACK1_ANIMATION,
	ATTACK2_ANIMATION,
	ATTACK3_ANIMATION,
	BLINK_ANIMATION,
	DEAD_ANIMATION,
	STAY_ANIMATION,
	JUMP_ANIMATION,
	ROLL_ANIMATION,
	RUN_ANIMATION,
	HEAVY_ATTACK1_ANIMATION,
	HEAVY_ATTACK2_ANIMATION,
	HEAVY_ATTACK3_ANIMATION,
};

/// <summary>
/// �v���C���[�ɕK�v�ȕϐ�
/// </summary>
class PlayerData
{
public: //struct
	/// <summary>
	/// ���͂ōs�����\��
	/// </summary>
	struct PlayerActionInput
	{
		bool isMove;
		bool isJump;
		bool isLightAttack;
		bool isStrongAttack;
		bool isAvoid;
		bool isBlink;
	};

public: //�����o�֐�
	PlayerData();
	~PlayerData();

	//setter
	void SetActionInput(bool& actionInputFlag, bool isInput) { actionInputFlag = isInput; }
	void SetAllActionInput(bool isInput) {
		actionInput.isMove = isInput;
		actionInput.isJump = isInput;
		actionInput.isLightAttack = isInput;
		actionInput.isStrongAttack = isInput;
		actionInput.isAvoid = isInput;
		actionInput.isBlink = isInput;
	}

public: //�ÓI�����o�ϐ� (�ύX���₷���悤��public)
	//�e�s���Ŏg�p���鎝�v��
	static int dashUseEndurance;
	static int avoidUseEndurance;
	static int jumpUseEndurance;
	static int blinkUseEndurance;

public: //�����o�ϐ� (�ύX���₷���悤��public)
	PlayerActionName action; //�v���C���[�̍s��

	Vector3 pos; //���W
	Vector3 moveVec; //�ړ��x�N�g��(����)
	Vector3 velocity; //���x
	Vector3 rota; //��]

	bool onGround ; //�n�ʂɐڒn���Ă��邩
	float fallSpeed; // �����X�s�[�h

	PlayerActionInput actionInput; //���͂ōs�����\��

	bool isMoveKey; //�ړ����͂�����Ă��邩(�L�[�{�[�h)
	bool isMovePad; //�ړ����͂�����Ă��邩(�Q�[���p�b�h)
	
	bool isDash; //�_�b�V������
	float moveSpeed; //�ړ��X�s�[�h

	int jumpMaxNum; //�W�����v�\��
	int jumpCount; //�W�����v�񐔃J�E���g
	bool isInputJump; //�W�����v�͒����p�A�{�^������������

	std::unique_ptr<BasePlayerAttack> attackAction; //�U���s�����e

	Vector3 avoidBlinkMoveVec; //����u�����N�p�ړ��x�N�g��
	int blinkCount; //�u�����N��

	int maxHP; //�ő�̗�
	int HP; //�̗�

	int maxEndurance; //�ő厝�v��
	int endurance; //���v��

	bool isNoGravity; //���������t���O
	bool isNoDamage; //���G�t���O
	bool isUseEndurance; //���v�͏���t���O
	bool isDead; //���S�t���O
};