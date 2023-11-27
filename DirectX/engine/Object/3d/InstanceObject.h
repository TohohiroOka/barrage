#pragma once
#include "Base3D.h"

class Camera;
class LightGroup;

class InstanceObject : public Base3D
{
private:
	//���_�f�[�^3D
	struct VERTEX {
		XMFLOAT3 pos;
		XMFLOAT3 uv;
		XMFLOAT3 normal;
	};

	//��x�ɕ`��ł����
	static const int draw_max_num = 512;

	struct OBJECT_INFO
	{
		XMFLOAT4 baseColor[draw_max_num];//�x�[�X�J���[
		XMMATRIX matWorld[draw_max_num];//world�s��
	};

	// �萔�o�b�t�@�p�f�[�^�\����B0
	struct CONST_BUFFER_DATA_B0
	{
		XMMATRIX viewproj; // �r���[�v���W�F�N�V�����s��
		XMFLOAT3 cameraPos; // �J�������W�i���[���h���W�j
		XMMATRIX lightViewproj;	//���C�g�r���[�v���W�F�N�V�����s��
		unsigned int isShadowMap;	//�e��t���邩
		unsigned int isBloom;//�u���[���̗L��
		unsigned int isToon;//�g�D�[���̗L��
		unsigned int isOutline;//�A�E�g���C���̗L��
		unsigned int isLight;//���C�e�B���O�̗L��
		XMFLOAT3 outlineColor;
	};

	//�e�p
	struct  CONST_BUFFER_DATA_LIGHTVIEW_B0
	{
		XMMATRIX viewproj;	//�r���[�v���W�F�N�V�����s��
		XMFLOAT3 cameraPos;	//�J�������W(���[���h���W)
	};

	//�e�p
	struct  OBJECT_INFO_LIGHTVIEW
	{
		XMMATRIX matWorld[draw_max_num];//world�s��
	};

public://�����o�֐�

	/// <summary>
	/// 
	/// </summary>
	/// <param name="_model">���f��</param>
	/// <returns>�C���X�^���X</returns>
	static std::unique_ptr<InstanceObject> Create(Model* _model);

private://�����o�֐�

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="_model">���f��</param>
	void Initialize(Model* _model);

public:

	InstanceObject();
	~InstanceObject();

	/// <summary>
	/// �`��Z�b�g
	/// </summary>
	/// <param name="_pos">���W</param>
	/// <param name="_scale">�傫��</param>
	/// <param name="_rotation">��]�p</param>
	/// <param name="_color">�F</param>
	void DrawInstance(const XMFLOAT3& _pos, const XMFLOAT3& _scale,
		const XMFLOAT3& _rotation, const XMFLOAT4& _color);

	/// <summary>
	/// �`��Z�b�g
	/// </summary>
	/// <param name="_worldMat">���[���h�s��</param>
	/// <param name="_color">�F</param>
	void DrawInstance(const XMMATRIX& _worldMat, const XMFLOAT4& _color);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw(const DrawMode _drawMode = DrawMode::alpha);

	/// <summary>
	/// �e�p�������C�g���猩�����_�ł̕`��
	/// </summary>
	void DrawLightView();

	/// <summary>
	/// �C���X�^���V���O�`��ő�`�搔�ɂȂ��Ă��Ȃ����̃`�F�b�N
	/// true�Ȃ�`��\
	/// </summary>
	/// <returns></returns>
	bool GetInstanceDrawCheck() { return instanceDrawNum < draw_max_num; }

	/// <summary>
	/// �C���X�^���V���O�`���
	/// </summary>
	/// <returns></returns>
	int GetInstanceDrawNum() { return instanceDrawNum; }

	/// <summary>
	/// 
	/// </summary>
	void FrameReset();

private:

	//�p�C�v���C�����
	static std::vector<GraphicsPipelineManager::DrawSet> pipeline;
	static std::vector<GraphicsPipelineManager::DrawSet> lightviewPipeline;

	//���f��
	Model* model;
	//�I�u�W�F�N�g���z��
	OBJECT_INFO objInform;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB0;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffB1;
	//�e�p�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffLightViewB0;
	//�e�p�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuffLightViewB1;
	//�C���X�^���V���O�`���
	int instanceDrawNum = 0;

public:

	static void SetPipeline(const std::vector<GraphicsPipelineManager::DrawSet>& _pipeline) { InstanceObject::pipeline = _pipeline; }
	static void SetLightviewPipeline(const std::vector<GraphicsPipelineManager::DrawSet>& _pipeline) { InstanceObject::lightviewPipeline = _pipeline; }
};