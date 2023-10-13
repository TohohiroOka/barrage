#pragma once
#include "Math/Vector3.h"

/// <summary>
/// �v���C���[�U�����N���X
/// </summary>
class BasePlayerAttack
{
public:
	struct AttackCollisionData {
		DirectX::XMVECTOR center;
		float radius;
		int power;
	};

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// �`��
	/// </summary>
	virtual void Draw() = 0;

	//getter
	AttackCollisionData GetAttackCollisionData() { return attackCollisionData; }
	bool GetisCollisionValid() { return isCollisionValid; }
	bool GetIsNextAttackInput() { return isNextAttackInput; }
	bool GetIsAttackActionEnd() { return isAttackActionEnd; }

protected: //�����o�ϐ�
	//�R���W�����f�[�^
	AttackCollisionData attackCollisionData;
	//�Փ˔��肪�L����
	bool isCollisionValid = false;
	//���̍U���̓��͂��J�n���邩
	bool isNextAttackInput = false;
	//�U���s�����I�����邩
	bool isAttackActionEnd = false;
};