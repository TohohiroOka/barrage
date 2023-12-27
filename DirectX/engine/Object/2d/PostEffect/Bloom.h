#pragma once
#include "BasePostEffect.h"

class Bloom : public BasePostEffect
{
private:

	struct CONST_BUFFER_DATA {
		float strength;//�u���[���̋���
		XMFLOAT4 pad;
	};

private:

	/// <summary>
	/// �萔�o�b�t�@����
	/// </summary>
	void CreateConstBuffer();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

public:

	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	Bloom();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Bloom() {};

	/// <summary>
	/// ����
	/// </summary>
	/// <param name="_texName">�g�p�摜��</param>
	/// <returns></returns>
	static std::unique_ptr<Bloom> Create(const std::string& _texName);

	/// <summary>
	/// �`��R�}���h�̔��s
	/// </summary>
	void Draw();

	void SetStrength(const float _strength) { strength = _strength; }

private:

	//�u���[���̋���
	float strength;
};