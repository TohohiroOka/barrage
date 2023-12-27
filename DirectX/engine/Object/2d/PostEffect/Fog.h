#pragma once
#include "BasePostEffect.h"

class Fog : public BasePostEffect
{
private:

	struct CONST_BUFFER_DATA {
		float strength;//�t�H�O�̋���
		float blendRate;//�J���[�̊���
		XMFLOAT2 pad1;
		XMFLOAT3 color1;//�t�H�O�̐F�i�����j
		float pad2;
		XMFLOAT3 color2;//�t�H�O�̐F�i�[�j
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
	static std::unique_ptr<Fog> Create(const std::string& _texName);

	/// <summary>
	/// �`��R�}���h�̔��s
	/// </summary>
	void Draw();

	void SetStrength(const float _strength) { strength = _strength; }
	void SetBlendRate(const float _blendRate) { blendRate = _blendRate; }
	void SetColor1(const XMFLOAT3& _color) { color1 = _color; }
	void SetColor2(const XMFLOAT3& _color) { color2 = _color; }

private:

	//�t�H�O�̋���
	float strength;
	//�J���[�̊���
	float blendRate;
	//�t�H�O�̐F
	XMFLOAT3 color1;
	//�t�H�O�̐F
	XMFLOAT3 color2;
};