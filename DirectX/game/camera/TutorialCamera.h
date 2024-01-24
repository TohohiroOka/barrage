#pragma once
#include "Camera/GameCamera.h"
#include <functional>

/// <summary>
/// �`���[�g���A���V�[���p�J����
/// </summary>
class TutorialCamera :public GameCamera
{
public: //enum
	/// <summary>
	/// �`���[�g���A���J�����̏��
	/// </summary>
	enum class TutorialCameraPhase
	{
		NORMAL, //�ʏ���
		ZOOM_ENEMY_BORN, //�G�������Y�[��
		ZOOM_END_RETURN, //�Y�[�����������A���ɖ߂�
	};


public: //�����o�֐�
	TutorialCamera();
	~TutorialCamera() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// ��Ԃ̕ύX
	/// </summary>
	/// <param name="phase">�ύX��̃t�F�[�Y</param>
	void ChangePhase(TutorialCameraPhase phase);

	//getter
	TutorialCameraPhase GetTutorialCameraPhase() { return cameraPhase; }
	bool GetIsPhaseActionEnd() { return isPhaseActionEnd; }

	//setter
	void SetZoomTargetPos(const Vector3& zoomTargetPos) { this->zoomTargetPos = zoomTargetPos; }

private: //�����o�֐�
	/// <summary>
	/// �ʏ��Ԃ̍s��
	/// </summary>
	void NormalAction();

	/// <summary>
	/// �G�������Y�[������s��
	/// </summary>
	void ZoomEnemyBornAction();

	/// <summary>
	/// �Y�[�����������A���ɖ߂��s��
	/// </summary>
	void ZoomEndReturnAction();

private: //�����o�ϐ�
	//�J�����̃t�F�[�Y���
	TutorialCameraPhase cameraPhase = TutorialCameraPhase::NORMAL;
	//�e�s���̓���
	std::vector<std::function<void()>> func;
	//�Y�[���Ώۂ̍��W
	Vector3 zoomTargetPos;
	//�C�[�W���O�p�^�C�}�[
	std::unique_ptr<Engine::Timer> easeTimer;
	//�C�[�W���O�O���W
	Vector3 easeBeforePos;
	//�C�[�W���O����W
	Vector3 easeAfterPos;
	//�C�[�W���O���]�p
	Vector3 easeAfterRota;
	//�Y�[���O���W
	Vector3 zoomBeforePos;
	//�Y�[���O��]�p
	Vector3 zoomBeforeRota;
	//��Ԃ̍s�����I������(�ʏ�s���ɏI���͂Ȃ�)
	bool isPhaseActionEnd = false;
};