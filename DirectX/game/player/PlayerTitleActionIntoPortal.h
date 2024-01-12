#pragma once
#include "PlayerActionBase.h"
#include "Math/Vector3.h"
#include "Math/Timer.h"

/// <summary>
/// �v���C���[�^�C�g���V�[���p�|�[�^���ɓ���s��
/// </summary>
class PlayerTitleActionIntoPortal : public PlayerActionBase
{
public: //�����o�֐�
	PlayerTitleActionIntoPortal(Player* player, const Vector3& portalPos);
	~PlayerTitleActionIntoPortal();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

private: //�����o�֐�
	/// <summary>
	/// �|�[�^���ɓ���
	/// </summary>
	void IntoPortal();

private: //�����o�ϐ�
	//�C�[�W���O�O���W
	Vector3 easeBeforePos;
	//�C�[�W���O����W
	Vector3 easeAfterPos;
	//�C�[�W���O�O��]�p
	Vector3 easeBeforeRota;
	//�C�[�W���O���]�p
	Vector3 easeAfterRota;
	//�C�[�W���O�O�傫��
	DirectX::XMFLOAT3 easeBeforeScale;
	//�C�[�W���O��傫��
	DirectX::XMFLOAT3 easeAfterScale;
	//�|�[�^���ɓ���p�^�C�}�[
	std::unique_ptr<Engine::Timer> intoPortalTimer;
	//��������
	bool isInPortal = false;
};