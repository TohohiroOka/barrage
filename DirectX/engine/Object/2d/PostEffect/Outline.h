#pragma once
#include "BasePostEffect.h"

class Outline : public BasePostEffect
{
private:

	struct CONST_BUFFER_DATA {
		float outlineWidth;//�A�E�g���C���̑���
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
	Outline();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Outline() {};

	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	static std::unique_ptr<Outline> Create();

	/// <summary>
	/// �`��R�}���h�̔��s
	/// </summary>
	void Draw(const Texture* _tex);

private:

	//�A�E�g���C���̐F
	float outlineWidth;//�A�E�g���C���̑���

public:
	void SetOutlineWidth(const float _outlineWidth) { outlineWidth = _outlineWidth; }

};