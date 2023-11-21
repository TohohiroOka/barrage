#pragma once
#include "PlayerActionBase.h"
#include "BasePlayerAttack.h"
#include <memory>

/// <summary>
/// �v���C���[�U���s��
/// </summary>
class PlayerActionAttack : public PlayerActionBase
{
public: //�����o�֐�
	PlayerActionAttack(Player* player);
	~PlayerActionAttack();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

private: //�����o�֐�
	/// <summary>
	/// �U�����ɂ̊p�x��ύX
	/// </summary>
	void ChangeRotate();

	/// <summary>
	/// ���̍U�������
	/// </summary>
	void NextAttack();
};