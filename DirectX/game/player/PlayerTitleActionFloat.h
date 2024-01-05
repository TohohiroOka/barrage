#pragma once
#include "PlayerActionBase.h"
#include "Math/Vector3.h"
#include "Math/Timer.h"

/// <summary>
/// �v���C���[�^�C�g���V�[���p���V�s��
/// </summary>
class PlayerTitleActionFloat : public PlayerActionBase
{
public: //�����o�֐�
	PlayerTitleActionFloat(Player* player, const Vector3& portalPos);
	~PlayerTitleActionFloat();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

private: //�����o�֐�
	/// <summary>
	/// ���V
	/// </summary>
	void Float();

private: //�����o�ϐ�
	//�C�[�W���O�O���W
	Vector3 easeBeforePos;
	//�C�[�W���O����W
	Vector3 easeAfterPos;
	//�C�[�W���O�O��]�p
	Vector3 easeBeforeRota;
	//�C�[�W���O���]�p
	Vector3 easeAfterRota;
	//���V�p�^�C�}�[
	std::unique_ptr<Engine::Timer> floatTimer;
};