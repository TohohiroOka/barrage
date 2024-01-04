#pragma once
#include "Camera/Camera.h"
#include "player/Player.h"

/// <summary>
/// �Q�[���J����
/// </summary>
class GameCamera :public Camera
{
public: //�ÓI�����o�֐�
	
	static void SetPlayer(Player* player) { GameCamera::player = player; }

public: //�����o�֐�
	GameCamera();
	~GameCamera() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// ���b�N�I��
	/// </summary>
	/// <param name="lockonTarget">���b�N�I���^�[�Q�b�g�I�u�W�F�N�g</param>
	void Lockon(Base3D* lockonTarget);

	//getter
	const XMFLOAT3& GetCameraRota() { return rotation; }
	Base3D* GetLockonTarget() { return lockonTarget; }
	bool GetisLockon() { return isLockon; }
	bool GetisLockonStart() { return isLockonStart; }

private: //�����o�֐�
	/// <summary>
	/// ���[���h�s����X�V
	/// </summary>
	/// <param name="matTrans">���s�ړ��s��</param>
	void UpdateMatWorld(const XMMATRIX& matTrans);

	/// <summary>
	/// ���_�ƒ����_���X�V
	/// </summary>
	void UpdateEyeTarget();

	/// <summary>
	/// �g�����X�t�H�[���X�V
	/// </summary>
	void UpdateTransform();

	/// <summary>
	/// ��]���X�V
	/// </summary>
	void UpdateRotate();

	/// <summary>
	/// ���b�N�I�����̃^�[�Q�b�g�ƒ����_�Ƃ̋������X�V
	/// </summary>
	void UpdateLockonTargetDistance();

	/// <summary>
	/// ���b�N�I�����̉�]���X�V
	/// </summary>
	void UpdateLockonRotate();

	/// <summary>
	/// ���b�N�I�����̉�]�����s
	/// </summary>
	void LockonAdjastEaseRotate(float& rotation, float easeBeforeRotate, float easeAfterRotate, float easeTime);

	/// <summary>
	/// ���W���X�V
	/// </summary>
	void UpdatePosition();

	/// <summary>
	/// ���W�X�V�ɂ��n�ʂȂǂ̂߂荞�݂�����
	/// </summary>
	void PositionCollision();

	/// <summary>
	/// ���b�N�I�����͂̋���
	/// </summary>
	void LockonInput();

	/// <summary>
	/// ���b�N�I���������ɉ�]�p��߂�
	/// </summary>
	void LockonEndRotate();

private: //�ÓI�����o�ϐ�
	static Player* player;
	static const float rotateXMin;
	static const float rotateXMax;
	static const float rotateXBase;
	static const float rotateCenterDistanceNormal;
	static const Vector3 targetDistanceNormal;

private: //�����o�ϐ�
	//�J�������W
	Vector3 position;
	//�^�[�Q�b�g�p�x
	Vector3 rotation;
	//��]�̒��S�ƂȂ�^�[�Q�b�g�Ƃ̋���
	float rotateCenterDistance;
	//�^�[�Q�b�g�̍��W�ۑ��p(���t���[���x�点�ăJ������Ǐ]�����邽��)
	std::list<Vector3> targetPositionsKeep;
	//�^�[�Q�b�g�̒��S���王�_�����炷����
	Vector3 targetDistance;
	//���[���h�ϊ��s��
	XMMATRIX matWorld = {};

	//���b�N�I���^�[�Q�b�g
	Base3D* lockonTarget = nullptr;
	//�����_�ƃ��b�N�I���^�[�Q�b�g�̒��S�Ƃ̋���
	Vector3 lockonTargetDistance;
	//���b�N�I����Ԃ�
	bool isLockon = false;
	//���b�N�I���^�[�Q�b�g�����o���邩
	bool isLockonStart = false;
	//���b�N�I���������ɉ�]�p��߂���
	bool isLockonEndRotate = false;
	//���b�N�I���J�n���Ƀ��b�N�I����]�p�ɃC�[�W���O���鎞�ԃ^�C�}�[
	std::unique_ptr<Engine::Timer> lockonChangeRotaTimer;
	//���b�N�I�����Ƀv���C���[�ƃ��b�N�I���Ώۂ�y�����d�Ȃ����Ƃ���y����]���~�����邩
	bool isLockonRotYStop = false;
	//���b�N�I�����Ƀv���C���[�ƃ��b�N�I���Ώۂ�y�����d�Ȃ����Ƃ���y����]���~�����鎞�ԃ^�C�}�[
	std::unique_ptr<Engine::Timer> lockonRotYStopTimer;
	//���b�N�I������y����]���~��������A�C���Ɏg�p����O�t���[���̃v���C���[�ƃ��b�N�I���ΏۂƂ̊p�x
	float oldMoveRotaVelocity;
	//�C�[�W���O�p�ϐ�
	Vector3 easeBeforeRota;
};