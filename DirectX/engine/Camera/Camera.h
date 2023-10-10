#pragma once
#include <d3dx12.h>
#include <DirectXMath.h>
#include "Math/Vector3.h"

/// <summary>
/// �J������{�@�\
/// </summary>
class Camera
{
protected: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public://�ÓI�����o�֐�

	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	/// <returns></returns>
	/// <param name="_mode">true->3D/false->2D</param>
	static std::unique_ptr<Camera> Create(const bool _mode = true);

public: // �����o�֐�

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	/// <param name="_mode">true->3D/false->2D</param>
	Camera(const bool _mode);

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~Camera() = default;

	/// <summary>
	/// �X�V
	/// </summary>
	virtual void Update();

	/// <summary>
	/// �J�����̃V�F�C�N�J�n
	/// </summary>
	/// <param name="_strength">�V�F�C�N�̋���</param>
	void StartCameraShake(int _strength);

	/// <summary>
	/// �J�����̃V�F�C�N�I��
	/// </summary>
	void EndCameraShake();

	/// <summary>
	/// �r���[�s��̎擾
	/// </summary>
	/// <returns>�r���[�s��</returns>
	inline const XMMATRIX& GetView() { return matView; }

	/// <summary>
	/// �ˉe�s��̎擾
	/// </summary>
	/// <returns>�ˉe�s��</returns>
	inline const XMMATRIX& GetProjection() { return matProjection; }

	/// <summary>
	/// ���_���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	inline const Vector3& GetEye() { return eye; }

	/// <summary>
	/// �����_���W�̎擾
	/// </summary>
	/// <returns>���W</returns>
	inline const Vector3& GetTarget() { return target; }

	/// <summary>
	/// �J�����̌X���̎擾
	/// </summary>
	/// <returns>������x�N�g��</returns>
	inline const Vector3& GetUp() { return up; }

	/// <summary>
	/// ���_���W�Z�b�g
	/// </summary>
	/// <param name="eye">���_���W</param>
	void SetEye(const Vector3& _eye) { this->eye = _eye; }

	/// <summary>
	/// �����_���W�Z�b�g
	/// </summary>
	/// <param name="position">�����_���W</param>
	void SetTarget(const Vector3& _target) { this->target = _target; }

	/// <summary>
	/// �J�����̌X���̃Z�b�g
	/// </summary>
	/// <param name="_up">�J�����̌X��</param>
	void SetUp(const Vector3& _up) { up = _up; }

	/// <summary>
	/// �ˉe�s��̍Đݒ�
	/// </summary>
	/// <param name="_back">�ŉ�</param>
	void SetMatProjection(float _back);

	/// <summary>
	/// �x�N�g���ɂ��ړ�
	/// </summary>
	/// <param name="move"></param>
	void MoveVector(const XMVECTOR& move);

protected: // �����o�ϐ�
	// �r���[�s��
	XMMATRIX matView = DirectX::XMMatrixIdentity();
	// �ˉe�s��
	XMMATRIX matProjection = DirectX::XMMatrixIdentity();
	// ���_���W
	Vector3 eye = { 0, 0, -20 };
	// �����_���W
	Vector3 target = { 0, 0, 0 };
	// ������x�N�g��
	Vector3 up = { 0, 1, 0 };
	// �A�X�y�N�g��
	float aspectRatio = 1.0f;
	//�V�F�C�N���W
	Vector3 ShakeDifference = {};
};