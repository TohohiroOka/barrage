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

	void Update() override;

	//getter
	const XMFLOAT3& GetCameraRota() { return rotation; }

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


	void UpdateTransform();


	void Rotate();

	void Move();

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
};