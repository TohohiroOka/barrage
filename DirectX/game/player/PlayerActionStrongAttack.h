#pragma once
#pragma once
#include "PlayerActionBaseAttack.h"

/// <summary>
/// �v���C���[���U���s��
/// </summary>
class PlayerActionStrongAttack : public PlayerActionBaseAttack
{
public: //�����o�֐�
	PlayerActionStrongAttack(Player* player);
	~PlayerActionStrongAttack();

private: //�����o�֐�
	/// <summary>
	/// ���̍U�������
	/// </summary>
	void NextAttack() override;
};