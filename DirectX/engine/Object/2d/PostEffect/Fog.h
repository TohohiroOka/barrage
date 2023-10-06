#pragma once
#include "BasePostEffect.h"

class Fog : public BasePostEffect
{
private:

	struct CONST_BUFFER_DATA {
		float strength;//�t�H�O�̋���
		XMFLOAT3 color;//�t�H�O�̐F
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
	Fog();

	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	~Fog() {};

	/// <summary>
	/// �C���X�^���X�̐���
	/// </summary>
	static std::unique_ptr<Fog> Create();

	/// <summary>
	/// �`��R�}���h�̔��s
	/// </summary>
	void Draw(const Texture* _tex);

private:

	//�t�H�O�̋���
	float strength;
	//�t�H�O�̐F
	XMFLOAT3 color;

public:
	void SetStrength(const float _strength) { strength = _strength; }
	void SetColor(const XMFLOAT3& _color) { color = _color; }
};

