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

	/// <summary>
	/// �������_�ł̕`��
	/// </summary>
	virtual void DrawLightView() = 0;

	/// <summary>
	/// ���̍U���ɑJ��
	/// </summary>
	virtual bool NextAttack(int endurance) = 0;

	//getter
	int GetUseEndranceNum() { return useEnduranceNum; }
	AttackCollisionData GetAttackCollisionData() { return attackCollisionData; }
	bool GetisCollisionValid() { return isCollisionValid; }
	bool GetIsNextAttackInput() { return isNextAttackInput; }
	bool GetIsAttackActionEnd() { return isAttackActionEnd; }

protected: //�����o�ϐ�
	//�A���U����
	int attackNum = 0;
	//�g�p���鎝�v��
	int useEnduranceNum;
	//�R���W�����f�[�^
	AttackCollisionData attackCollisionData;
	//�Փ˔��肪�L����
	bool isCollisionValid = false;
	//���̍U���̓��͂��J�n���邩
	bool isNextAttackInput = false;
	//�U���s�����I�����邩
	bool isAttackActionEnd = false;
};