#pragma once
#include "Math/Vector3.h"
#include "BasePlayerAttack.h"
#include <memory> 

/// <summary>
/// �v���C���[�̍s����
/// </summary>
enum PlayerActionName
{
	MOVENORMAL,	//�ʏ�ړ�(���͂ɐ�����������Ȃ�)
	JUMP,	//�W�����v(1�t���[��)
	ATTACK,	//�U��
	AVOID,	//���
	BLINK,	//�u�����N
	KNOCKBACK,//�m�b�N�o�b�N
};

/// <summary>
/// �A�j���[�V������
/// </summary>
enum PlayerAnimationName
{
	RUN_ANIMATION,
	ROLL_ANIMATION,
	ATTACK_RIGHT_ANIMATION,
	ATTACK_LEFT_ANIMATION,
	JUMP_ANIMATION,
	STAY_ANIMATION,
};

/// <summary>
/// �v���C���[�ɕK�v�ȕϐ�
/// </summary>
class PlayerData
{
public: //�����o�֐�
	PlayerData();
	~PlayerData();

public: //�����o�ϐ� (�ύX���₷���悤��public)
	PlayerActionName action; //�v���C���[�̍s��

	Vector3 pos; //���W
	Vector3 moveVec; //�ړ��x�N�g��(����)
	Vector3 velocity; //���x
	Vector3 rota; //��]

	bool onGround ; //�n�ʂɐڒn���Ă��邩
	float fallSpeed; // �����X�s�[�h

	bool isMoveKey; //�ړ����͂�����Ă��邩(�L�[�{�[�h)
	bool isMovePad; //�ړ����͂�����Ă��邩(�Q�[���p�b�h)

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
	bool isDead; //���S�t���O
};