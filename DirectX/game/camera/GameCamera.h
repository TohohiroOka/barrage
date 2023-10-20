#pragma once
#include "Camera/Camera.h"
#include "player/Player.h"

/// <summary>
/// �Q�[���J����
/// </summary>
class GameCamera :public Camera
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<GameCamera> Create();
	
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
	void Rotate();

	/// <summary>
	/// ���W���X�V
	/// </summary>
	void UpdatePosition();

	/// <summary>
	/// ���b�N�I�����͂̋���
	/// </summary>
	void LockonInput();

private: //�ÓI�����o�ϐ�
	static Player* player;
	static const float baseDistance;

private: //�����o�ϐ�
	//�J�������W
	Vector3 position;
	//�^�[�Q�b�g�p�x
	Vector3 rotation;
	//�^�[�Q�b�g�Ƃ̋���
	float targetDistance;
	//���[���h�ϊ��s��
	XMMATRIX matWorld = {};

	//���b�N�I���^�[�Q�b�g
	Base3D* lockonTarget = nullptr;
	//���b�N�I����Ԃ�
	bool isLockon = false;
	//���b�N�I���^�[�Q�b�g�����o���邩
	bool isLockonStart = false;
};