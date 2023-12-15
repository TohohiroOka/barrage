#pragma once
#include "Math/Vector3.h"

/// <summary>
/// �v���C���[�N���X�̑O���錾
/// </summary>
class Player;

/// <summary>
/// �v���C���[�U�����N���X
/// </summary>
class BasePlayerAttack
{
public:
	struct AttackCollisionData {
		Vector3 startPosition;
		Vector3 endPosition;
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
	/// ���̍U���ɑJ��
	/// </summary>
	virtual bool NextAttack() = 0;

	/// <summary>
	/// �U�������������ꍇ�̏���
	/// </summary>
	virtual void AttackCollision() = 0;

	//getter
	int GetUseEndranceNum() { return useEnduranceNum; }
	AttackCollisionData GetAttackCollisionData() { return attackCollisionData; }
	bool GetIsCollisionValid() { return isCollisionValid; }
	bool GetIsNextActionInput() { return isNextActionInput; }
	bool GetIsAttackActionEnd() { return isAttackActionEnd; }

	//setter
	void SetIsCollisionValid(bool isCollisionValid) { this->isCollisionValid = isCollisionValid; }

protected: //�����o�ϐ�
	//�v���C���[
	Player* player;
	//�A���U����
	int attackNum = 0;
	//�g�p���鎝�v��
	int useEnduranceNum;
	//�R���W�����f�[�^
	AttackCollisionData attackCollisionData;
	//�Փ˔��肪�L����
	bool isCollisionValid = false;
	//���̍s���̓��͂��J�n���邩
	bool isNextActionInput = false;
	//�U���s�����I�����邩
	bool isAttackActionEnd = false;
};