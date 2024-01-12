#pragma once
#include "Camera/GameCamera.h"
#include <functional>

/// <summary>
/// �^�C�g���V�[���p�J����
/// </summary>
class TitleCamera :public GameCamera
{
public: //enum
	/// <summary>
	/// �^�C�g���J�����̏��
	/// </summary>
	enum class TitleCameraPhase
	{
		NORMAL, //�ʏ���
		MOVE_PORTAL_FRONT, //�|�[�^�����ʂɈړ�
		ZOOM_PORTAL, //�|�[�^�����Y�[��
		STAY,	//��~
	};


public: //�����o�֐�
	TitleCamera();
	~TitleCamera() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// ��Ԃ̕ύX
	/// </summary>
	/// <param name="phase">�ύX��̃t�F�[�Y</param>
	void ChangePhase(TitleCameraPhase phase);

	//getter
	TitleCameraPhase GetTitleCameraPhase() { return cameraPhase; }
	bool GetIsPhaseActionEnd() { return isPhaseActionEnd; }

	//setter
	void SetPortalPos(const Vector3& portalPos) { this->portalPos = portalPos; }

private: //�����o�֐�
	/// <summary>
	/// �ʏ��Ԃ̍s��
	/// </summary>
	void NormalAction();

	/// <summary>
	/// �|�[�^���̐��ʂɈړ�����s��
	/// </summary>
	void MovePortalFrontAction();

	/// <summary>
	/// �|�[�^�����Y�[������s��
	/// </summary>
	void ZoomPortalAction();

	/// <summary>
	/// �|�[�^���O�Œ�~����s��
	/// </summary>
	void StayAction();

private: //�����o�ϐ�
	//�J�����̃t�F�[�Y���
	TitleCameraPhase cameraPhase = TitleCameraPhase::NORMAL;
	//�e�s���̓���
	std::vector<std::function<void()>> func;
	//�Ώۃ|�[�^���̍��W
	Vector3 portalPos;
	//�C�[�W���O�p�^�C�}�[
	std::unique_ptr<Engine::Timer> easeTimer;
	//�C�[�W���O�O���W
	Vector3 easeBeforePos;
	//�C�[�W���O����W
	Vector3 easeAfterPos;
	//�C�[�W���O���]�p
	Vector3 easeAfterRota;
	//��Ԃ̍s�����I������(�ʏ�s���ɏI���͂Ȃ�)
	bool isPhaseActionEnd = false;
};