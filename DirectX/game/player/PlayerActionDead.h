#pragma once
#include "PlayerActionBase.h"

/// <summary>
/// �v���C���[���S�s��
/// </summary>
class PlayerActionDead : public PlayerActionBase
{
public: //�����o�֐�
	PlayerActionDead(Player* player);
	~PlayerActionDead();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override {};
};