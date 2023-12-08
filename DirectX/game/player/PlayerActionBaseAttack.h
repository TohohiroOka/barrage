#pragma once
#include "PlayerActionBase.h"
#include "BasePlayerAttack.h"
#include <memory>

/// <summary>
/// �v���C���[�U���s�����N���X
/// </summary>
class PlayerActionBaseAttack : public PlayerActionBase
{
public: //�����o�֐�
	PlayerActionBaseAttack(Player* player);
	~PlayerActionBaseAttack();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

protected: //�����o�֐�
	/// <summary>
	/// ���̍U�������
	/// </summary>
	virtual void NextAttack() = 0;
};