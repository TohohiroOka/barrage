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
	ATTACK1_ANIMATION,//1
	ATTACK2_ANIMATION,//2
	ATTACK3_ANIMATION,//3
	BLINK_ANIMATION,//4
	DEAD_ANIMATION,//5
	STAY_ANIMATION,//6
	JUMP_ANIMATION,//7
	ROLL_ANIMATION,//8
	RUN_ANIMATION,//9
	NO_DATA1,//10
	NO_DATA2,//11
	NO_DATA3,//12
	FEAR_ANIMATION,//13
	HEAVY_ATTACK1_ANIMATION,//14
	HEAVY_ATTACK2_ANIMATION,//15
	HEAVY_ATTACK3_ANIMATION,//16
	
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

	bool isEnemyAttackAvoid; //�G�̍U�����������
	bool isEnemyAttackAvoidJudge; //�G�̍U��������������肷�邩

	int maxHP; //�ő�̗�
	int HP; //�̗�

	int maxEndurance; //�ő厝�v��
	int endurance; //���v��

	bool isNoGravity; //���������t���O
	bool isNoDamage; //���G�t���O
	bool isUseEndurance; //���v�͏���t���O
	bool isDead; //���S�t���O
};