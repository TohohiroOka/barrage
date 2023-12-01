#pragma once
#include "PlayerActionBase.h"

/// <summary>
/// �v���C���[�ʏ�ړ��s��
/// </summary>
class PlayerActionMoveNormal : public PlayerActionBase
{
public: //�����o�֐�
	PlayerActionMoveNormal(Player* player);
	~PlayerActionMoveNormal();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

private: //�����o�֐�
	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// �_�b�V��
	/// </summary>
	void Dash();

	/// <summary>
	/// ���̍s�����J�n
	/// </summary>
	void NextActionStart();

public: //�ÓI�����o�ϐ� (�ύX���₷���悤��public)
	//�ő�ړ��X�s�[�h
	static float moveSpeedMax;
	//�ő�_�b�V���X�s�[�h
	static float dashSpeedMax;

private: //�����o�ϐ�
	//�_�b�V�����Ă��邩
	bool isDash = false;
	bool isDashStart = true;
};