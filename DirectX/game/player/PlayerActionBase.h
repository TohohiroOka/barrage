#pragma once
#include "PlayerData.h"

/// <summary>
/// �v���C���[�N���X�̑O���錾
/// </summary>
class Player;


/// <summary>
/// �v���C���[�̍s�����N���X
/// </summary>
class PlayerActionBase
{
public:
	PlayerActionBase(Player* player);
	~PlayerActionBase();

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	bool GetIsActionEnd() { return isActionEnd; }

protected: //�����o�֐�
	/// <summary>
	/// �W�����v�ł����Ԃ�
	/// </summary>
	/// <returns></returns>
	bool JumpStart();

	/// <summary>
	/// �U���J�n�ł����Ԃ�
	/// </summary>
	/// <returns></returns>
	bool AttackStart();

	/// <summary>
	/// ����J�n�ł����Ԃ�
	/// </summary>
	/// <returns></returns>
	bool AvoidStart();

	/// <summary>
	/// �u�����N�J�n�ł����Ԃ�
	/// </summary>
	/// <returns></returns>
	bool BlinkStart();

protected: //�ÓI�����o�ϐ�
	//�e�s���Ŏg�p���鎝�v��
	static const int dashUseEndurance = 1;
	static const int avoidUseEndurance = 10;
	static const int jumpUseEndurance = 0;
	static const int blinkUseEndurance = 0;

protected: //�����o�ϐ�
	//�s��������Ώۂ̃v���C���[
	Player* player;
	//���݂̍s�����I�����邩
	bool isActionEnd = false;
	//�s���I���������ꍇ�Ɏ��ɂǂ̍s�������邩
	PlayerActionName nextAction;
};