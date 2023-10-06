#pragma once
#include"ParticleManager.h"

class Camera;

class Emitter
{
private: // �G�C���A�X
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;

public://�ÓI�����o�֐�

	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	/// <param name="_name">�e�N�X�`����</param>
	/// <returns>�C���X�^���X</returns>
	static std::unique_ptr<Emitter> Create(const std::string& _name);

public://�����o�֐�

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Emitter() {};

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Emitter() {};

	/// <summary>
	/// �p�[�e�B�N���̔����n�_
	/// </summary>
	/// <param name="_maxFrame">��������</param>
	/// <param name="_position">�������W</param>
	/// <param name="_velocity">���x</param>
	/// <param name="_accel">�����x</param>
	/// <param name="_startScale">�����T�C�Y</param>
	/// <param name="_endScale">�ŏI�T�C�Y</param>
	/// <param name="_startColor">�����J���[</param>
	/// <param name="_endColor">�ŏI�J���[</param>
	void InEmitter(int _maxFrame, const XMFLOAT3& _position, const XMFLOAT3& _velocity,
		const XMFLOAT3& _accel, float _startScale, float _endScale, const XMFLOAT4& _startColor, const XMFLOAT4& _endColor);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �폜
	/// </summary>
	void ParticlAllDelete();

private:
	//�p�[�e�B�N���N���X
	std::unique_ptr<ParticleManager> particleManager = nullptr;

public:

	/// <summary>
	/// �u���[���Z�b�g
	/// </summary>
	void SetBloom() { particleManager->SetBloom(true); }

	/// <summary>
	/// ���݂̐�
	/// </summary>
	int GetCreateNum() {
		return particleManager->GetCreateNum();
	}

};