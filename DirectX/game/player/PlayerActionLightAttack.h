#pragma once
#include "PlayerActionBaseAttack.h"

/// <summary>
/// �v���C���[��U���s��
/// </summary>
class PlayerActionLightAttack : public PlayerActionBaseAttack
{
public: //�����o�֐�
	PlayerActionLightAttack(Player* player);
	~PlayerActionLightAttack();

private: //�����o�֐�
	/// <summary>
	/// ���̍U�������
	/// </summary>
	void NextAttack() override;
};