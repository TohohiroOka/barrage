#pragma once
#include "PlayerActionBase.h"

/// <summary>
/// �v���C���[�W�����v�s��
/// </summary>
class PlayerActionJump : public PlayerActionBase
{
public: //�����o�֐�
	PlayerActionJump(Player* player);
	~PlayerActionJump();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

private: //�ÓI�����o�ϐ�
	//�W�����v��
	static float jumpPower;
};