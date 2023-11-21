#pragma once
#include "PlayerActionBase.h"
#include "Math/Vector3.h"

/// <summary>
/// �v���C���[�m�b�N�o�b�N�s��
/// </summary>
class PlayerActionKnockback : public PlayerActionBase
{
public: //�����o�֐�
	PlayerActionKnockback(Player* player, const Vector3& subjectPos, int power);
	~PlayerActionKnockback();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

private: //�����o�֐�
	/// <summary>
	/// �m�b�N�o�b�N
	/// </summary>
	void Knockback();

private: //�����o�ϐ�
	//�m�b�N�o�b�N�x�N�g��
	Vector3 knockbackVec;
	//�m�b�N�o�b�N�̋���
	float knockbackPower;
};