#pragma once
#include "Camera/Camera.h"
#include "Math/Vector2.h"
#include "Math/Vector3.h"

/// <summary>
/// �f�o�b�O�J����
/// </summary>
class DebugCamera : public Camera
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	/// <returns></returns>
	static std::unique_ptr<DebugCamera> Create(const Vector3& targetPos);

public: //�����o�֐�
	DebugCamera(const Vector3& targetPos);
	~DebugCamera() override;

	void Update() override;

private: //�����o�֐�
	void MoveCamera();
	void ChangeTargetDistance();
	void UpdateAngle3d(const XMFLOAT2& angle);

	void Move1();

private: //�ÓI�����o�ϐ�
	static const float eyeTargetDistance;

private: //�����o�ϐ�
	// �J���������_�܂ł̋���
	float distance = 200;
	//��ʃT�C�Y�̃X�P�[��
	XMFLOAT2 scale;
	// ��]�s��
	XMMATRIX matRot = DirectX::XMMatrixIdentity();

	//�����p
	Vector2 oka_moveRota;
	Vector3 pos;

};